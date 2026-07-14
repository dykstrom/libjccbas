/*
 * Copyright (C) 2026 Johan Dykstrom
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <inttypes.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jcc_gc.h"

/*
 * Precise, non-moving mark-and-sweep collector. See jcc_gc.h for the API
 * contract. Internal state is a set of file-static variables; the design
 * is single-threaded and not async-signal-safe by contract.
 */

/* ---- Allocation table: open-addressing hash set keyed on pointer ---- */

typedef struct {
    void                *ptr;   /* NULL slot = empty (no tombstones)        */
    const jcc_gc_type_t *type;  /* NULL = leaf object, freed with free()    */
    int                  mark;  /* mark bit, cleared at the start of a sweep */
} gc_entry_t;

#define GC_INITIAL_CAPACITY 16  /* must be a power of two */

static gc_entry_t *table_entries;
static size_t      table_capacity;
static size_t      table_count;   /* occupied slots == live objects */

/* ---- Roots ---- */

static const jcc_gc_root_range_t *global_roots;  /* caller's array, not copied */

static void  ***local_roots;      /* growable array of slot addresses */
static size_t   local_count;
static size_t   local_capacity;

static size_t  *frames;           /* watermark stack of local_count values */
static size_t   frame_count;
static size_t   frame_capacity;

/* ---- Statistics and configuration ---- */

static int64_t stat_registered;
static int64_t stat_collections;
static int64_t stat_freed;
static int64_t threshold;

static int   initialized;
static int   debug;
static FILE *out;             /* debug destination */
static int   out_is_file;     /* out must be fclose()d on reset/shutdown */

static int   atexit_installed;

/* ---- Debug output ---- */

static void gc_log(const char *fmt, ...) {
    va_list ap;
    if (!debug || out == NULL) {
        return;
    }
    va_start(ap, fmt);
    vfprintf(out, fmt, ap);
    va_end(ap);
    fflush(out);
}

/* ---- Hashing and table operations ---- */

static size_t hash_ptr(const void *p, size_t capacity) {
    /* Drop the low bits (alignment) and spread the rest. */
    uintptr_t h = (uintptr_t) p >> 3;
    h *= (uintptr_t) 0x9E3779B97F4A7C15ULL;
    return (size_t) h & (capacity - 1);
}

/* Insert ptr/type into a table with a free slot available. No resize. */
static void table_put(gc_entry_t *entries, size_t capacity, void *ptr,
                      const jcc_gc_type_t *type, int mark) {
    size_t i = hash_ptr(ptr, capacity);
    while (entries[i].ptr != NULL) {
        i = (i + 1) & (capacity - 1);
    }
    entries[i].ptr = ptr;
    entries[i].type = type;
    entries[i].mark = mark;
}

/* Look up ptr; returns its entry or NULL if not registered. */
static gc_entry_t *table_find(void *ptr) {
    size_t i;
    if (table_entries == NULL || ptr == NULL) {
        return NULL;
    }
    i = hash_ptr(ptr, table_capacity);
    while (table_entries[i].ptr != NULL) {
        if (table_entries[i].ptr == ptr) {
            return &table_entries[i];
        }
        i = (i + 1) & (table_capacity - 1);
    }
    return NULL;
}

/* Grow the table to the next power of two and rehash. */
static void table_grow(void) {
    size_t new_capacity = table_capacity * 2;
    gc_entry_t *new_entries = calloc(new_capacity, sizeof(gc_entry_t));
    size_t i;
    if (new_entries == NULL) {
        return; /* out of memory: leave table as-is, best effort */
    }
    for (i = 0; i < table_capacity; i++) {
        if (table_entries[i].ptr != NULL) {
            table_put(new_entries, new_capacity, table_entries[i].ptr,
                      table_entries[i].type, table_entries[i].mark);
        }
    }
    free(table_entries);
    table_entries = new_entries;
    table_capacity = new_capacity;
}

static void table_insert(void *ptr, const jcc_gc_type_t *type) {
    /* Grow before the load factor exceeds 0.75. */
    if ((table_count + 1) * 4 >= table_capacity * 3) {
        table_grow();
    }
    table_put(table_entries, table_capacity, ptr, type, 0);
    table_count++;
}

/* ---- Reset ---- */

/* Free all tracked objects and release all internal buffers. */
static void reset_state(void) {
    size_t i;
    if (table_entries != NULL) {
        for (i = 0; i < table_capacity; i++) {
            void *p = table_entries[i].ptr;
            if (p != NULL) {
                const jcc_gc_type_t *t = table_entries[i].type;
                if (t != NULL && t->finalize != NULL) {
                    t->finalize(p);
                } else {
                    free(p);
                }
            }
        }
        free(table_entries);
    }
    table_entries = NULL;
    table_capacity = 0;
    table_count = 0;

    free(local_roots);
    local_roots = NULL;
    local_count = 0;
    local_capacity = 0;

    free(frames);
    frames = NULL;
    frame_count = 0;
    frame_capacity = 0;

    global_roots = NULL;

    stat_registered = 0;
    stat_collections = 0;
    stat_freed = 0;

    if (out_is_file && out != NULL) {
        fclose(out);
    }
    out = NULL;
    out_is_file = 0;
}

/* ---- Initialization ---- */

void jcc_gc_init(int64_t initial_threshold, int64_t flags) {
    const char *dbg_env;
    const char *log_path;

    /* Idempotent reset: a second call clears all prior state. Real programs
     * call this once; tests reuse it for per-scenario isolation. */
    if (initialized) {
        reset_state();
    }

    table_entries = calloc(GC_INITIAL_CAPACITY, sizeof(gc_entry_t));
    table_capacity = GC_INITIAL_CAPACITY;
    table_count = 0;

    threshold = (initial_threshold <= 0) ? 100 : initial_threshold;

    dbg_env = getenv("JCC_GC_DEBUG");
    debug = ((flags & JCC_GC_DEBUG) != 0) || (dbg_env != NULL && dbg_env[0] != '\0');

    out = NULL;
    out_is_file = 0;
    if (debug) {
        log_path = getenv("JCC_GC_LOG");
        if (log_path != NULL && log_path[0] != '\0') {
            out = fopen(log_path, "a");
            out_is_file = (out != NULL);
        }
        if (out == NULL) {
            out = stdout;
            out_is_file = 0;
        }
    }

    initialized = 1;

    if (!atexit_installed) {
        atexit(jcc_gc_shutdown);
        atexit_installed = 1;
    }

    gc_log("jcc_gc: init: threshold=%" PRId64 "\n", threshold);
}

/* ---- Global roots ---- */

void jcc_gc_set_global_roots(const jcc_gc_root_range_t *ranges) {
    global_roots = ranges;
}

/* ---- Shadow-stack frames and local roots ---- */

void jcc_gc_push_frame(void) {
    if (frame_count == frame_capacity) {
        size_t new_capacity = (frame_capacity == 0) ? 16 : frame_capacity * 2;
        size_t *grown = realloc(frames, new_capacity * sizeof(size_t));
        if (grown == NULL) {
            return; /* best effort under OOM */
        }
        frames = grown;
        frame_capacity = new_capacity;
    }
    frames[frame_count++] = local_count;
}

void jcc_gc_pop_frame(void) {
    if (frame_count > 0) {
        local_count = frames[--frame_count];
    }
}

void jcc_gc_add_root(void **slot) {
    if (local_count == local_capacity) {
        size_t new_capacity = (local_capacity == 0) ? 16 : local_capacity * 2;
        void ***grown = realloc(local_roots, new_capacity * sizeof(void **));
        if (grown == NULL) {
            return; /* best effort under OOM */
        }
        local_roots = grown;
        local_capacity = new_capacity;
    }
    local_roots[local_count++] = slot;
}

/* ---- Mark ---- */

/* Mark one referenced object and, for typed objects, trace its interior
 * pointers. Matches the jcc_gc_mark_fn signature so it doubles as the
 * callback handed to trace functions. */
static void mark_value(void *v) {
    gc_entry_t *e = table_find(v);
    if (e == NULL || e->mark) {
        return; /* NULL, string literal / not registered, or already marked */
    }
    e->mark = 1;
    if (e->type != NULL && e->type->trace != NULL) {
        e->type->trace(v, mark_value);
    }
}

/* ---- Collection ---- */

void jcc_gc_collect(void) {
    size_t i;
    size_t new_capacity;
    gc_entry_t *survivors;
    int64_t freed_now = 0;
    int64_t live_before = (int64_t) table_count;

    if (!initialized) {
        return;
    }

    /* Clear marks. */
    for (i = 0; i < table_capacity; i++) {
        table_entries[i].mark = 0;
    }

    /* Mark from global root ranges (terminated by {NULL, 0}). */
    if (global_roots != NULL) {
        const jcc_gc_root_range_t *r;
        for (r = global_roots; !(r->slots == NULL && r->count == 0); r++) {
            int64_t j;
            for (j = 0; j < r->count; j++) {
                mark_value(r->slots[j]);
            }
        }
    }

    /* Mark from local roots: each entry is the address of a pointer slot. */
    for (i = 0; i < local_count; i++) {
        void **slot = local_roots[i];
        if (slot != NULL) {
            mark_value(*slot);
        }
    }

    /* Sweep: rebuild the table with survivors only; reclaim the rest. */
    new_capacity = table_capacity;
    survivors = calloc(new_capacity, sizeof(gc_entry_t));
    if (survivors == NULL) {
        return; /* OOM: skip this collection rather than corrupt state */
    }
    for (i = 0; i < table_capacity; i++) {
        void *p = table_entries[i].ptr;
        if (p == NULL) {
            continue;
        }
        if (table_entries[i].mark) {
            table_put(survivors, new_capacity, p, table_entries[i].type, 0);
        } else {
            const jcc_gc_type_t *t = table_entries[i].type;
            if (t != NULL && t->finalize != NULL) {
                t->finalize(p);
            } else {
                free(p);
            }
            freed_now++;
        }
    }
    free(table_entries);
    table_entries = survivors;
    table_count = (size_t) (live_before - freed_now);

    stat_freed += freed_now;
    stat_collections++;

    gc_log("jcc_gc: collect: live %" PRId64 " -> %" PRId64 " (freed %" PRId64 ")\n",
           live_before, (int64_t) table_count, freed_now);
}

/* ---- Allocation registration ---- */

void *jcc_gc_register_object(void *p, const jcc_gc_type_t *type) {
    if (p == NULL) {
        return NULL;
    }

    /* Trigger (D9): when live objects reach the threshold, collect BEFORE
     * inserting p, so the collection p triggers can never reclaim p. */
    if ((int64_t) table_count >= threshold) {
        jcc_gc_collect();
        while ((int64_t) table_count > threshold / 2) {
            int64_t old = threshold;
            threshold *= 2;
            gc_log("jcc_gc: threshold: %" PRId64 " -> %" PRId64 "\n", old, threshold);
        }
    }

    table_insert(p, type);
    stat_registered++;
    return p;
}

void *jcc_gc_register(void *p) {
    return jcc_gc_register_object(p, NULL);
}

/* ---- Stats ---- */

void jcc_gc_stats(jcc_gc_stats_t *out_stats) {
    if (out_stats == NULL) {
        return;
    }
    out_stats->registered = stat_registered;
    out_stats->live = (int64_t) table_count;
    out_stats->collections = stat_collections;
    out_stats->freed = stat_freed;
    out_stats->threshold = threshold;
}

/* ---- Shutdown ---- */

void jcc_gc_shutdown(void) {
    if (!initialized) {
        return; /* safe to call multiple times */
    }

    /* Print exit stats before freeing so that registered == freed + live. */
    gc_log("jcc_gc: exit: registered=%" PRId64 " collections=%" PRId64
           " freed=%" PRId64 " live=%" PRId64 "\n",
           stat_registered, stat_collections, stat_freed, (int64_t) table_count);

    reset_state();
    initialized = 0;
}
