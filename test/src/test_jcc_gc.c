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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "jcc_gc.h"

/* ---- Fixtures for the typed-object (trace/finalize) scenario ---- */

typedef struct {
  void *child;
} rec_t;

static int finalize_count;

static void rec_trace(void *obj, jcc_gc_mark_fn mark) {
  rec_t *r = (rec_t *) obj;
  mark(r->child);
}

static void rec_finalize(void *obj) {
  finalize_count++;
  free(obj);
}

static const jcc_gc_type_t REC_TYPE = { "rec", rec_trace, rec_finalize };

/* ---- Scenarios ---- */

// Init defaults: threshold defaults to 100 when <= 0; all counters start at 0.
static void test_init_defaults(void) {
  jcc_gc_stats_t s;

  jcc_gc_init(0, 0);
  jcc_gc_stats(&s);
  assert_equals_I64_I64(100, s.threshold);
  assert_equals_I64_I64(0, s.registered);
  assert_equals_I64_I64(0, s.live);
  assert_equals_I64_I64(0, s.collections);
  assert_equals_I64_I64(0, s.freed);

  jcc_gc_init(50, 0);
  jcc_gc_stats(&s);
  assert_equals_I64_I64(50, s.threshold);
}

// Reachability: a rooted object survives collection; an unrooted one is freed.
static void test_reachability(void) {
  jcc_gc_stats_t s;
  void *slot = NULL;

  jcc_gc_init(100, 0);
  jcc_gc_push_frame();
  jcc_gc_add_root(&slot);

  slot = jcc_gc_register(malloc(8));  // rooted via slot
  jcc_gc_register(malloc(8));         // unrooted garbage

  jcc_gc_collect();
  jcc_gc_stats(&s);
  assert_equals_I64_I64(2, s.registered);
  assert_equals_I64_I64(1, s.live);
  assert_equals_I64_I64(1, s.freed);

  jcc_gc_pop_frame();
}

// Ignored slot values: NULL and unregistered pointers are skipped, no crash.
static void test_ignored_slot_values(void) {
  jcc_gc_stats_t s;
  int on_stack = 42;
  void *slot_null = NULL;
  void *slot_bogus = &on_stack;  // a valid address the GC never registered

  jcc_gc_init(100, 0);
  jcc_gc_push_frame();
  jcc_gc_add_root(&slot_null);
  jcc_gc_add_root(&slot_bogus);

  jcc_gc_register(malloc(8));  // unrooted

  jcc_gc_collect();  // must not crash on NULL / non-heap slot values
  jcc_gc_stats(&s);
  assert_equals_I64_I64(0, s.live);
  assert_equals_I64_I64(1, s.freed);

  jcc_gc_pop_frame();
}

// Global roots: both a scalar range {&slot,1} and an array range {base,count}.
static void test_global_roots(void) {
  jcc_gc_stats_t s;
  void *g0 = NULL;
  void *arr[3] = { NULL, NULL, NULL };
  jcc_gc_root_range_t ranges[] = {
    { &g0, 1 },
    { arr, 3 },
    { NULL, 0 },
  };

  jcc_gc_init(100, 0);
  jcc_gc_set_global_roots(ranges);

  g0 = jcc_gc_register(malloc(8));
  arr[1] = jcc_gc_register(malloc(8));
  jcc_gc_register(malloc(8));  // unrooted

  jcc_gc_collect();
  jcc_gc_stats(&s);
  assert_equals_I64_I64(2, s.live);
  assert_equals_I64_I64(1, s.freed);
}

// Frames: a nested frame's roots are dropped by pop_frame.
static void test_frames(void) {
  jcc_gc_stats_t s;
  void *outer = NULL;
  void *inner = NULL;

  jcc_gc_init(100, 0);

  jcc_gc_push_frame();
  jcc_gc_add_root(&outer);
  outer = jcc_gc_register(malloc(8));

  jcc_gc_push_frame();
  jcc_gc_add_root(&inner);
  inner = jcc_gc_register(malloc(8));

  jcc_gc_collect();
  jcc_gc_stats(&s);
  assert_equals_I64_I64(2, s.live);  // both frames' roots live

  jcc_gc_pop_frame();  // drop inner's frame
  jcc_gc_collect();
  jcc_gc_stats(&s);
  assert_equals_I64_I64(1, s.live);  // inner now unreachable
  assert_true_Bool(s.freed >= 1);

  jcc_gc_pop_frame();
}

// Trigger (D9): a registration at the threshold collects BEFORE inserting,
// so the block it registers is never reclaimed by its own collection.
static void test_trigger(void) {
  jcc_gc_stats_t s;
  char *p;
  int i;

  jcc_gc_init(4, 0);
  for (i = 0; i < 4; i++) {
    jcc_gc_register(malloc(8));  // unrooted; fills live up to the threshold
  }

  p = (char *) jcc_gc_register(malloc(8));  // live == 4 == threshold -> collect first
  assert_not_null(p);
  p[0] = 'x';  // p must still be valid: it was not freed by its own trigger

  jcc_gc_stats(&s);
  assert_true_Bool(s.collections >= 1);
  assert_equals_I64_I64(1, s.live);  // the 4 unrooted blocks swept, p inserted
}

// Threshold doubling: with everything rooted, collections free nothing, so the
// threshold doubles while live stays above threshold/2.
static void test_threshold_doubling(void) {
  jcc_gc_stats_t s;
  void *slots[100];
  jcc_gc_root_range_t ranges[] = { { slots, 100 }, { NULL, 0 } };
  int i;

  memset(slots, 0, sizeof(slots));
  jcc_gc_init(4, 0);
  jcc_gc_set_global_roots(ranges);

  for (i = 0; i < 100; i++) {
    slots[i] = jcc_gc_register(malloc(8));  // stored into a root immediately
  }

  jcc_gc_stats(&s);
  assert_equals_I64_I64(100, s.live);
  assert_true_Bool(s.threshold > 4);  // grew because live kept exceeding half
}

// Typed objects: trace marks interior refs; finalize reclaims unmarked objects.
static void test_trace_and_finalize(void) {
  jcc_gc_stats_t s;
  void *root = NULL;
  char *child;
  rec_t *rec;

  jcc_gc_init(100, 0);
  finalize_count = 0;
  jcc_gc_push_frame();
  jcc_gc_add_root(&root);

  child = (char *) jcc_gc_register(malloc(8));  // reachable only via rec->child
  rec = (rec_t *) malloc(sizeof(rec_t));
  rec->child = child;
  jcc_gc_register_object(rec, &REC_TYPE);
  root = rec;

  jcc_gc_collect();
  jcc_gc_stats(&s);
  assert_equals_I64_I64(2, s.live);  // rec kept by root, child kept by trace
  assert_equals_I64_I64(0, finalize_count);

  root = NULL;  // rec (and thus child) now unreachable
  jcc_gc_collect();
  jcc_gc_stats(&s);
  assert_equals_I64_I64(0, s.live);
  assert_equals_I64_I64(1, finalize_count);  // rec finalized; child plain-freed

  jcc_gc_pop_frame();
}

// Load: 1000 string slots as a global root, 100_000 allocations each stored
// into a random slot. Overwriting a slot orphans its old string, so the GC
// must collect steadily while every currently-referenced string survives.
static void test_load(void) {
  enum { SLOTS = 1000, ALLOCS = 100000 };
  static void *arr[SLOTS];
  jcc_gc_root_range_t ranges[] = { { arr, SLOTS }, { NULL, 0 } };
  jcc_gc_stats_t s;
  int64_t non_null;
  int i;

  memset(arr, 0, sizeof(arr));
  jcc_gc_init(256, 0);
  jcc_gc_set_global_roots(ranges);

  for (i = 0; i < ALLOCS; i++) {
    int r = rand() % SLOTS;
    char *str = (char *) jcc_gc_register(malloc(16));
    snprintf(str, 16, "s%d", i);
    arr[r] = str;  // stored into a root before the next allocation (D9 contract)
  }

  jcc_gc_collect();  // final sweep: only strings still held by a slot survive

  non_null = 0;
  for (i = 0; i < SLOTS; i++) {
    if (arr[i] != NULL) {
      non_null++;
    }
  }

  jcc_gc_stats(&s);
  assert_equals_I64_I64(ALLOCS, s.registered);
  assert_true_Bool(s.collections >= 1);       // the threshold must have fired
  assert_equals_I64_I64(non_null, s.live);    // live == distinct rooted strings
  assert_true_Bool(s.live <= SLOTS);          // can never exceed the slot count
  assert_equals_I64_I64(s.registered, s.freed + s.live);  // nothing leaked/lost
}

// Debug output: with debug enabled and JCC_GC_LOG set, shutdown appends the
// exact exit-stats line JCC's integration tests assert on. Runs last so the
// atexit-installed shutdown is a no-op afterwards.
static void test_debug_exit_stats(void) {
  static char env_buf[512];
  const char *log_path = "test_jcc_gc_debug.log";
  void *root = NULL;
  FILE *f;
  char content[2048];
  size_t n;

  remove(log_path);
  snprintf(env_buf, sizeof(env_buf), "JCC_GC_LOG=%s", log_path);
  putenv(env_buf);

  jcc_gc_init(100, JCC_GC_DEBUG);
  jcc_gc_push_frame();
  jcc_gc_add_root(&root);
  root = jcc_gc_register(malloc(8));  // rooted, survives
  jcc_gc_register(malloc(8));         // unrooted, reclaimed
  jcc_gc_collect();                   // registered=2 collections=1 freed=1 live=1
  jcc_gc_pop_frame();
  jcc_gc_shutdown();                  // writes the exit line, frees the survivor

  f = fopen(log_path, "r");
  assert_not_null(f);
  n = fread(content, 1, sizeof(content) - 1, f);
  content[n] = '\0';
  fclose(f);

  assert_not_null(strstr(content,
      "jcc_gc: exit: registered=2 collections=1 freed=1 live=1"));

  remove(log_path);
}

int main(void) {
  test_init_defaults();
  test_reachability();
  test_ignored_slot_values();
  test_global_roots();
  test_frames();
  test_trigger();
  test_threshold_doubling();
  test_trace_and_finalize();
  test_load();
  test_debug_exit_stats();
  return 0;
}
