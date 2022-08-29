	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 12
	.globl	_cread_alt
	.align	4, 0x90
_cread_alt:                             ## @cread_alt
## BB#0:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%eax
	movl	8(%ebp), %eax
	movl	$0, -4(%ebp)
	testl	%eax, %eax
	leal	-4(%ebp), %ecx
	cmovnel	%eax, %ecx
	movl	(%ecx), %eax
	addl	$4, %esp
	popl	%ebp
	retl


.subsections_via_symbols
