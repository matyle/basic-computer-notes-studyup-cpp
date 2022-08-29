	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 12
	.globl	_rSum
	.p2align	4, 0x90
_rSum:                                  ## @rSum
## BB#0:
	pushl	%ebp
	movl	%esp, %ebp
	movl	12(%ebp), %ecx
	xorl	%eax, %eax
	testl	%ecx, %ecx
	jle	LBB0_3
## BB#1:
	movl	8(%ebp), %edx
	incl	%ecx
	xorl	%eax, %eax
	.p2align	4, 0x90
LBB0_2:                                 ## =>This Inner Loop Header: Depth=1
	addl	(%edx), %eax
	addl	$4, %edx
	decl	%ecx
	cmpl	$1, %ecx
	jg	LBB0_2
LBB0_3:
	popl	%ebp
	retl


.subsections_via_symbols
