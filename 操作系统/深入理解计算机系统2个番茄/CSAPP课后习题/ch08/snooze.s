	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 12
	.globl	_handler
	.p2align	4, 0x90
_handler:                               ## @handler
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
	movl	%edi, -4(%rbp)
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_snooze
	.p2align	4, 0x90
_snooze:                                ## @snooze
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp3:
	.cfi_def_cfa_offset 16
Ltmp4:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp5:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %edi
	callq	_sleep
	leaq	L_.str(%rip), %rdi
	movl	%eax, -8(%rbp)
	movl	-4(%rbp), %eax
	subl	-8(%rbp), %eax
	movl	-4(%rbp), %edx
	movl	%eax, %esi
	movb	$0, %al
	callq	_printf
	movl	-8(%rbp), %edx
	movl	%eax, -12(%rbp)         ## 4-byte Spill
	movl	%edx, %eax
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp6:
	.cfi_def_cfa_offset 16
Ltmp7:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp8:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movl	$0, -4(%rbp)
	movl	%edi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	cmpl	$2, -8(%rbp)
	je	LBB2_2
## BB#1:
	leaq	L_.str.1(%rip), %rsi
	movq	___stderrp@GOTPCREL(%rip), %rax
	movq	(%rax), %rdi
	movq	-16(%rbp), %rax
	movq	(%rax), %rdx
	movb	$0, %al
	callq	_fprintf
	xorl	%edi, %edi
	movl	%eax, -20(%rbp)         ## 4-byte Spill
	callq	_exit
LBB2_2:
	movl	$2, %edi
	leaq	_handler(%rip), %rsi
	callq	_signal
	movq	$-1, %rsi
	cmpq	%rsi, %rax
	jne	LBB2_4
## BB#3:
	leaq	L_.str.2(%rip), %rdi
	callq	_unix_error
LBB2_4:
	movq	-16(%rbp), %rax
	movq	8(%rax), %rdi
	callq	_atoi
	movl	%eax, %edi
	callq	_snooze
	xorl	%edi, %edi
	movl	%eax, -24(%rbp)         ## 4-byte Spill
	callq	_exit
	.cfi_endproc

	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"Slept for %u of %u secs.\n"

L_.str.1:                               ## @.str.1
	.asciz	"usage: %s <secs>\n"

L_.str.2:                               ## @.str.2
	.asciz	"signal error\n"


.subsections_via_symbols
