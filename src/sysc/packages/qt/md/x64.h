/*
 * QuickThreads -- Threads-building toolkit.
 * Copyright (c) 1993 by David Keppel
 *
 * Permission to use, copy, modify and distribute this software and
 * its documentation for any purpose and without fee is hereby
 * granted, provided that the above copyright notice and this notice
 * appear in all copies.  This software is provided as a
 * proof-of-concept and for demonstration purposes; there is no
 * representation about the suitability of this software for any
 * purpose.
 */

#ifndef QUICKTHREADS_X64_H
#define QUICKTHREADS_X64_H

typedef unsigned long long qt_word_t;

/* Thread's initial stack layout on the Windows x64:

   non-varargs:

   +---
   | arg[2]	=== `userf' on startup
   | arg[1]	=== `pt' on startup
   | arg[0]	=== `pu' on startup
   +---
   | ret pc === qt_error
   +---
   | ret pc	=== `only' on startup
   +---
   | %ebp
   | %esi
   | %edi
   | %ebx				<--- qt_t.sp
   +---

   When a non-varargs thread is started, it ``returns'' directly to
   the client's `only' function.

   varargs:

   +---
   | arg[n-1]
   | ..
   | arg[0]
   +---
   | ret pc	=== `qt_vstart'
   +---
   | %ebp	=== `startup'
   | %esi	=== `cleanup'
   | %edi	=== `pt'
   | %ebx	=== `vuserf'		<--- qt_t.sp
   +---

   When a varargs thread is started, it ``returns'' to the `qt_vstart'
   startup code.  The startup code calls the appropriate functions. */


/* What to do to start a varargs thread running. */
extern void qt_vstart (void);


/* Hold four return pcs (qt_error, qt_start and twice qt_align)
   plus ten args and two qt_word_t's for correct alignment. */
#define QUICKTHREADS_STKBASE	(16 * sizeof(long long))

/* Hold 4 saved regs plus one return pc (qt_vstart). */
#define QUICKTHREADS_VSTKBASE	(5 * sizeof(long long))


/* Stack must be 16-byte aligned at function call instr. (SSE data support) */
#define QUICKTHREADS_STKALIGN	(16)


/* Where to place various arguments. */
#define QUICKTHREADS_ONLY_INDEX	(QUICKTHREADS_PC)
#define QUICKTHREADS_USER_INDEX	(QUICKTHREADS_ARG2)
#define QUICKTHREADS_ARGT_INDEX	(QUICKTHREADS_ARG1)
#define QUICKTHREADS_ARGU_INDEX	(QUICKTHREADS_ARG0)

/* Stack layout offsets relative to stack at initial stack setup. */

/* Stack alignment         17 */
#define QUICKTHREADS_RPC   16
#define QUICKTHREADS_POP0  15
#define QUICKTHREADS_PC    14
#define QUICKTHREADS_POP1  13
#define QUICKTHREADS_RBP   12
/* Stack alignment         11 */
#define QUICKTHREADS_R12   10
#define QUICKTHREADS_R13    9
#define QUICKTHREADS_R14    8
#define QUICKTHREADS_R15    7
#define QUICKTHREADS_RBX    6
#define QUICKTHREADS_RSI    5
#define QUICKTHREADS_RDI    4
#define QUICKTHREADS_R9     3
#define QUICKTHREADS_R8     2
#define QUICKTHREADS_RCX    1
#define QUICKTHREADS_RDX    0


/* Arguments to save stack function. */

#define QUICKTHREADS_ARG0 QUICKTHREADS_RCX
#define QUICKTHREADS_ARG1 QUICKTHREADS_RDX
#define QUICKTHREADS_ARG2 QUICKTHREADS_R8


/* Stack grows down.  The top of the stack is the first thing to
   pop off (preincrement, postdecrement). */
#define QUICKTHREADS_GROW_DOWN

extern void qt_error (void);
extern void qt_align (void); /* For correct stack alignment */

/* Push on the error return address, force Frame Pointer to 0 and
   push stack alignment trampoline function.  */
#define QUICKTHREADS_ARGS_MD(sto) \
  (QUICKTHREADS_SPUT (sto, QUICKTHREADS_RBP, 0), \
   QUICKTHREADS_SPUT (sto, QUICKTHREADS_POP0, qt_align), \
   QUICKTHREADS_SPUT (sto, QUICKTHREADS_POP1, qt_align), \
   QUICKTHREADS_SPUT (sto, QUICKTHREADS_RPC, qt_error))


/* When varargs are pushed, allocate space for all the args. */
#define QUICKTHREADS_VARGS_MD0(sto, nbytes) \
  ((qt_t *)(((char *)(sto)) - QUICKTHREADS_STKROUNDUP(nbytes)))

#define QUICKTHREADS_VARGS_MD1(sto) \
  (QUICKTHREADS_SPUT (sto, QUICKTHREADS_PC, qt_vstart))

#define QUICKTHREADS_VARGS_DEFAULT

#endif /* QUICKTHREADS_X64_H */
