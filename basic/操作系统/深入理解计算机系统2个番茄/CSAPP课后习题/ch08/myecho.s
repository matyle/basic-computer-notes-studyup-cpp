	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 12
	.globl	_main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp0:
	.cfi_def_cfa_offset 16
Ltmp1:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp2:
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movl	$0, -4(%rbp)
	movl	%edi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	cmpl	$3, -8(%rbp)
	jne	LBB0_10
## BB#1:
	leaq	L_.str(%rip), %rdi
	movb	$0, %al
	callq	_printf
	movl	$0, -28(%rbp)
	movl	%eax, -32(%rbp)         ## 4-byte Spill
LBB0_2:                                 ## =>This Inner Loop Header: Depth=1
	movslq	-28(%rbp), %rax
	movq	-16(%rbp), %rcx
	cmpq	$0, (%rcx,%rax,8)
	je	LBB0_5
## BB#3:                                ##   in Loop: Header=BB0_2 Depth=1
	leaq	L_.str.1(%rip), %rdi
	movl	-28(%rbp), %esi
	movslq	-28(%rbp), %rax
	movq	-16(%rbp), %rcx
	movq	(%rcx,%rax,8), %rdx
	movb	$0, %al
	callq	_printf
	movl	%eax, -36(%rbp)         ## 4-byte Spill
## BB#4:                                ##   in Loop: Header=BB0_2 Depth=1
	movl	-28(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -28(%rbp)
	jmp	LBB0_2
LBB0_5:
	leaq	L_.str.2(%rip), %rdi
	movb	$0, %al
	callq	_printf
	movl	$0, -28(%rbp)
	movl	%eax, -40(%rbp)         ## 4-byte Spill
LBB0_6:                                 ## =>This Inner Loop Header: Depth=1
	movslq	-28(%rbp), %rax
	movq	-24(%rbp), %rcx
	cmpq	$0, (%rcx,%rax,8)
	je	LBB0_9
## BB#7:                                ##   in Loop: Header=BB0_6 Depth=1
	leaq	L_.str.3(%rip), %rdi
	movl	-28(%rbp), %esi
	movslq	-28(%rbp), %rax
	movq	-24(%rbp), %rcx
	movq	(%rcx,%rax,8), %rdx
	movb	$0, %al
	callq	_printf
	movl	%eax, -44(%rbp)         ## 4-byte Spill
## BB#8:                                ##   in Loop: Header=BB0_6 Depth=1
	movl	-28(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -28(%rbp)
	jmp	LBB0_6
LBB0_9:
	jmp	LBB0_11
LBB0_10:
	leaq	L_.str.4(%rip), %rdi
	movb	$0, %al
	callq	_printf
	movl	%eax, -48(%rbp)         ## 4-byte Spill
LBB0_11:
	xorl	%eax, %eax
	addq	$48, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"Command line arguments:\n"

L_.str.1:                               ## @.str.1
	.asciz	"argv[%2d]: %s\n"

L_.str.2:                               ## @.str.2
	.asciz	"\nEnvironment variables:\n"

L_.str.3:                               ## @.str.3
	.asciz	"envp[%2d]: %s\n"

L_.str.4:                               ## @.str.4
	.asciz	"No enough command line arguments.\n"


.subsections_via_symbols
