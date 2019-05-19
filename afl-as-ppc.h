/*
   american fuzzy lop - injectable parts
   -------------------------------------
   cf. afl-as-ppc.h
   (c) Duverger stephane, Airbus 2019
*/
#ifndef _HAVE_AFL_AS_PPC_H
#define _HAVE_AFL_AS_PPC_H

#include "config.h"
#include "types.h"

static const u8* trampoline_fmt_64 = ".NOT IMPLEMENTED";
static const u8* main_payload_64 = ".NOT IMPLEMENTED";

static const u8* trampoline_fmt_32 =
   "\n"
   "/* --- AFL TRAMPOLINE (32-BIT) --- */\n"
   "\n"
   ".align 4\n"
   "  stwu    %r1,-64(%r1)\n"
   "  stw     %r0,  4(%r1)\n"
   "  stw     %r2,  8(%r1)\n"
   "  stw     %r3, 12(%r1)\n"
   "  stw     %r4, 16(%r1)\n"
   "  stw     %r5, 20(%r1)\n"
   "  mfcr    %r0\n"
   "  stw     %r0, 24(%r1)\n"
   "  mflr    %r0\n"
   "  stw     %r0, 28(%r1)\n"
   "  lis     %r3, 0x%1$08x@h\n"
   "  ori     %r3, %r3, 0x%1$08x@l\n"
   "  bl      __afl_maybe_log\n"
   "  lwz     %r0, 28(%r1)\n"
   "  mtlr    %r0\n"
   "  lwz     %r0, 24(%r1)\n"
   "  mtcr    %r0\n"
   "  lwz     %r0,  4(%r1)\n"
   "  lwz     %r2,  8(%r1)\n"
   "  lwz     %r3, 12(%r1)\n"
   "  lwz     %r4, 16(%r1)\n"
   "  lwz     %r5, 20(%r1)\n"
   "  addi    %r1, %r1, 64\n"
   "/* --- END --- */\n"
   "\n";

static const u8* main_payload_32 =
   "/* --- AFL MAIN PAYLOAD (32-BIT) --- */\n"
   "\n"
   ".text\n"
   ".align 4\n"
   "\n"
   "__afl_maybe_log:\n"
   "\n"
   "  lis     %r2, 0xe0000000@h\n"
   "  ori     %r2, %r2, 0xe0000000@l\n"
   "\n"
/* shm_trace_map[cur_loc ^ prev_loc]++ */
   "__afl_store:\n"
#ifndef COVERAGE_ONLY
// loc = id ^ prev
// prev = id>>1
   "  lis     %r5, __afl_prev_loc@h\n"
   "  ori     %r5, %r5, __afl_prev_loc@l\n"
   "  lwz     %r4, 0(%r5)\n"
   "  srwi    %r0, %r3, 1\n"
   "  xor     %r3, %r3, %r4\n"
   "  stw     %r0, 0(%r5)\n"
#else
// loc = id
#endif /* ^!COVERAGE_ONLY */
   "\n"
   "  add     %r2, %r2, %r3\n"
   "  lbz     %r3, 0(%r2)\n"
#ifdef SKIP_COUNTS
   "  ori     %r3, %r3, 1\n"
#else
   "  addi    %r3, %r3, 1\n"
#endif /* ^SKIP_COUNTS */
   "  stb     %r3, 0(%r2)\n"
   "\n"
   "__afl_return:\n"
   "\n"
   // restore flags
   "  blr\n"
   "\n"
   ".AFL_VARS:\n"
   "\n"
#ifndef COVERAGE_ONLY
   "  .comm   __afl_prev_loc, 4, 32\n"
#endif /* !COVERAGE_ONLY */
   "\n"
   "/* --- END --- */\n"
   "\n";

#endif
