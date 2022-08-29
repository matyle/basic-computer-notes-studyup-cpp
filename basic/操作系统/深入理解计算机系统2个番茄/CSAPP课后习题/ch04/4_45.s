	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 12
	.globl	_bubble_p
	.p2align	4, 0x90
_bubble_p:                              ## @bubble_p
## BB#0:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$20, %esp
	movl	12(%ebp), %eax
	movl	8(%ebp), %ecx
	movl	%ecx, -4(%ebp)
	movl	%eax, -8(%ebp)
	movl	$1, -16(%ebp)
LBB0_1:                                 ## =>This Loop Header: Depth=1
                                        ##     Child Loop BB0_3 Depth 2
	movl	-16(%ebp), %eax
	cmpl	-8(%ebp), %eax
	jge	LBB0_10
## BB#2:                                ##   in Loop: Header=BB0_1 Depth=1
	movl	-16(%ebp), %eax
	subl	$1, %eax
	movl	%eax, -12(%ebp)
LBB0_3:                                 ##   Parent Loop BB0_1 Depth=1
                                        ## =>  This Inner Loop Header: Depth=2
	cmpl	$0, -12(%ebp)
	jl	LBB0_8
## BB#4:                                ##   in Loop: Header=BB0_3 Depth=2
	movl	-4(%ebp), %eax
	movl	-12(%ebp), %ecx
	movl	4(%eax,%ecx,4), %eax
	movl	-4(%ebp), %ecx
	movl	-12(%ebp), %edx
	cmpl	(%ecx,%edx,4), %eax
	jge	LBB0_6
## BB#5:                                ##   in Loop: Header=BB0_3 Depth=2
	movl	-4(%ebp), %eax
	movl	-12(%ebp), %ecx
	movl	4(%eax,%ecx,4), %eax
	movl	%eax, -20(%ebp)
	movl	-4(%ebp), %eax
	movl	-12(%ebp), %ecx
	movl	(%eax,%ecx,4), %eax
	movl	-4(%ebp), %ecx
	movl	-12(%ebp), %edx
	movl	%eax, 4(%ecx,%edx,4)
	movl	-20(%ebp), %eax
	movl	-4(%ebp), %ecx
	movl	-12(%ebp), %edx
	movl	%eax, (%ecx,%edx,4)
LBB0_6:                                 ##   in Loop: Header=BB0_3 Depth=2
	jmp	LBB0_7
LBB0_7:                                 ##   in Loop: Header=BB0_3 Depth=2
	movl	-12(%ebp), %eax
	addl	$-1, %eax
	movl	%eax, -12(%ebp)
	jmp	LBB0_3
LBB0_8:                                 ##   in Loop: Header=BB0_1 Depth=1
	jmp	LBB0_9
LBB0_9:                                 ##   in Loop: Header=BB0_1 Depth=1
	movl	-16(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -16(%ebp)
	jmp	LBB0_1
LBB0_10:
	addl	$20, %esp
	popl	%ebp
	retl


.subsections_via_symbols
