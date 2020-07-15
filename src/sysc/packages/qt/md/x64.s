/* x64.s -- assembly support. */

/*
// QuickThreads -- Threads-building toolkit.
// Copyright (c) 1993 by David Keppel
//
// Permission to use, copy, modify and distribute this software and
// its documentation for any purpose and without fee is hereby
// granted, provided that the above copyright notice and this notice
// appear in all copies.  This software is provided as a
// proof-of-concept and for demonstration purposes; there is no
// representation about the suitability of this software for any
// purpose. */

/* 64-bit Intel Architecture Support
// written by Andy Goodrich, Forte Design Systms, Inc.  */

/* NOTE: double-labeled `_name' and `name' for System V compatability.  */
/* NOTE: Mixed C/C++-style comments used. Sorry! */

    .text
    .align 2

    .globl _qt_abort
    .globl qt_abort
    .globl _qt_block
    .globl qt_block
    .globl _qt_blocki
    .globl qt_blocki
    .globl _qt_align
    .globl qt_align

_qt_abort:
qt_abort:
_qt_block:
qt_block:
_qt_blocki:
qt_blocki:
	                 /* 13 (return address.) */
	pushq %rbp       /* 12 (push stack frame top.) */
	movq  %rsp, %rbp /* set new stack frame top. */
	                 /* save registers. */
	subq $8, %rsp    /*  11 (Stack alignment) */
	pushq %r12       /*  10 ... */
	pushq %r13       /*  9 ... */
	pushq %r14       /*  8 ... */
	pushq %r15       /*  7 ... */
	pushq %rbx       /*  6 ... */
	pushq %rsi       /*  5 ... */
	pushq %rdi       /*  4 ... */
	pushq %r9        /*  3 ... (new stack address) */
	pushq %r8        /*  2 ... (arg) */
	pushq %rcx       /*  1 ... (address of save function.) */
	pushq %rdx       /*  0 ... (cor) */

    movq %rcx, %rax		/* get address of save function. */
    movq %rsp, %rcx		/* set current stack as save argument. */
	movq %r9, %rsp		/* swap stacks. */
	movq %r9, %rbp		/* adjust stack frame pointer. */
	addq $12*8, %rbp	/* ... */
	subq $0x20, %rsp	/* account for home space */
    call *%rax			/* call function to save stack pointer. */
	addq $0x20, %rsp	/* compensate previous adjustment that was done for home space */

	                /* restore registers. */
	popq %rdx       /* ... */
	popq %rcx       /* ... */
	popq %r8        /* ... */
	popq %r9        /* ... */
	popq %rdi       /* ... */
	popq %rsi       /* ... */
	popq %rbx       /* ... */
	popq %r15       /* restore registers from new stack. */
	popq %r14       /* ... */
	popq %r13       /* ... */
	popq %r12       /* ... */
	leave           /* unwind stack. */
_qt_align:
qt_align:
	ret             /* return. */
