/*
 * jcc_gc.h - JCC runtime garbage collector.
 *
 * Precise mark-sweep collector with compiler-managed roots (shadow stack).
 * The compiler registers the ADDRESSES of pointer slots (globals, allocas);
 * the collector reads each slot's current value at mark time. Slot values
 * that are NULL or do not refer to a registered allocation (e.g. string
 * literals) are ignored, so a slot may legally hold any of these.
 *
 * Ownership: jcc_gc_register(p) transfers ownership of a malloc'd block p
 * to the GC; unreachable blocks are reclaimed with free() (or the type's
 * finalize callback). Runtime string functions keep allocating with malloc
 * and are unaware of the GC.
 *
 * Canonical copy: libjccbas. libjcccol vendors an identical copy.
 * Single-threaded. Not async-signal-safe.
 */
#ifndef JCC_GC_H_
#define JCC_GC_H_

#include <stdint.h>

/* ---- Type descriptors (future closures/records; strings use NULL) ---- */

/* Mark callback handed to trace functions: marks one referenced object. */
typedef void (*jcc_gc_mark_fn)(void *obj);

typedef struct jcc_gc_type {
    const char *name;                              /* for debug output        */
    void (*trace)(void *obj, jcc_gc_mark_fn mark); /* mark interior refs;
                                                      NULL = leaf object      */
    void (*finalize)(void *obj);                   /* NULL = plain free(obj)  */
} jcc_gc_type_t;

/* ---- Initialization ---- */

/* Flags for jcc_gc_init. */
#define JCC_GC_DEBUG ((int64_t) 1)  /* enable debug output */

/*
 * Initialize the collector. Called once, first in main, before any other
 * jcc_gc_* call. initial_threshold <= 0 selects the default (100).
 * Installs jcc_gc_shutdown with atexit(). Debug output is enabled by the
 * JCC_GC_DEBUG flag or by a non-empty JCC_GC_DEBUG environment variable;
 * it is written to stdout, or appended to the file named by the
 * JCC_GC_LOG environment variable when set.
 */
void jcc_gc_init(int64_t initial_threshold, int64_t flags);

/* ---- Global roots ---- */

/*
 * A range of consecutive pointer slots: a scalar global is {&slot, 1},
 * a string array's element region is {base, element_count}.
 */
typedef struct jcc_gc_root_range {
    void   **slots;
    int64_t  count;
} jcc_gc_root_range_t;

/*
 * Register the program's global roots: 'ranges' points to an array of
 * ranges terminated by {NULL, 0}. The array is read at every collection
 * (not copied), so it must stay valid for the program's lifetime.
 * Called once from main, after jcc_gc_init.
 */
void jcc_gc_set_global_roots(const jcc_gc_root_range_t *ranges);

/* ---- Shadow-stack frames and local roots ---- */

/* Open a frame on function entry. Cheap: records a watermark. */
void jcc_gc_push_frame(void);

/* Close the current frame, dropping all roots added since push_frame.
 * Called before ret, and before a musttail call. */
void jcc_gc_pop_frame(void);

/*
 * Add a pointer slot (the address of an alloca or global) to the current
 * frame. The slot must contain NULL or a valid pointer whenever a
 * collection may run, so initialize non-parameter slots with NULL first.
 */
void jcc_gc_add_root(void **slot);

/* ---- Allocation registration ---- */

/*
 * Transfer ownership of malloc'd block p to the GC and return p.
 * May run a collection BEFORE p is inserted, so p is never reclaimed by
 * the collection it triggers itself. CONTRACT: the caller must store p
 * into a registered root slot before the next allocation/registration.
 * p must not already be registered. NULL is returned unchanged.
 */
void *jcc_gc_register(void *p);

/*
 * As jcc_gc_register, for objects with interior pointers (future:
 * closures, records). type == NULL is equivalent to jcc_gc_register.
 */
void *jcc_gc_register_object(void *p, const jcc_gc_type_t *type);

/* ---- Collection, stats, shutdown ---- */

/* Run a full mark-sweep collection now. */
void jcc_gc_collect(void);

typedef struct jcc_gc_stats {
    int64_t registered;   /* registrations since init            */
    int64_t live;         /* currently live objects              */
    int64_t collections;  /* collections run                     */
    int64_t freed;        /* objects reclaimed                   */
    int64_t threshold;    /* current collection threshold        */
} jcc_gc_stats_t;

void jcc_gc_stats(jcc_gc_stats_t *out);

/*
 * Free all remaining objects and print exit stats when debug is enabled:
 *   jcc_gc: exit: registered=N collections=M freed=K live=L
 * Installed via atexit by jcc_gc_init; safe to call multiple times.
 */
void jcc_gc_shutdown(void);

#endif /* JCC_GC_H_ */
