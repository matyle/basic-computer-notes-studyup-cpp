	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 12
	.globl	_unix_error
	.p2align	4, 0x90
_unix_error:                            ## @unix_error
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
	subq	$32, %rsp
	movq	___stderrp@GOTPCREL(%rip), %rax
	movq	%rdi, -8(%rbp)
	movq	(%rax), %rdi
	movq	-8(%rbp), %rdx
	movq	%rdi, -16(%rbp)         ## 8-byte Spill
	movq	%rdx, -24(%rbp)         ## 8-byte Spill
	callq	___error
	movl	(%rax), %edi
	callq	_strerror
	leaq	L_.str(%rip), %rsi
	movq	-16(%rbp), %rdi         ## 8-byte Reload
	movq	-24(%rbp), %rdx         ## 8-byte Reload
	movq	%rax, %rcx
	movb	$0, %al
	callq	_fprintf
	xorl	%edi, %edi
	movl	%eax, -28(%rbp)         ## 4-byte Spill
	callq	_exit
	.cfi_endproc

	.globl	_posix_error
	.p2align	4, 0x90
_posix_error:                           ## @posix_error
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
	subq	$48, %rsp
	movq	___stderrp@GOTPCREL(%rip), %rax
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movq	(%rax), %rdi
	movq	-16(%rbp), %rdx
	movl	-4(%rbp), %ecx
	movq	%rdi, -24(%rbp)         ## 8-byte Spill
	movl	%ecx, %edi
	movq	%rdx, -32(%rbp)         ## 8-byte Spill
	callq	_strerror
	leaq	L_.str(%rip), %rsi
	movq	-24(%rbp), %rdi         ## 8-byte Reload
	movq	-32(%rbp), %rdx         ## 8-byte Reload
	movq	%rax, %rcx
	movb	$0, %al
	callq	_fprintf
	xorl	%edi, %edi
	movl	%eax, -36(%rbp)         ## 4-byte Spill
	callq	_exit
	.cfi_endproc

	.globl	_dns_error
	.p2align	4, 0x90
_dns_error:                             ## @dns_error
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
	subq	$16, %rsp
	leaq	L_.str.1(%rip), %rsi
	movq	_h_errno@GOTPCREL(%rip), %rax
	movq	___stderrp@GOTPCREL(%rip), %rcx
	movq	%rdi, -8(%rbp)
	movq	(%rcx), %rdi
	movq	-8(%rbp), %rdx
	movl	(%rax), %ecx
	movb	$0, %al
	callq	_fprintf
	xorl	%edi, %edi
	movl	%eax, -12(%rbp)         ## 4-byte Spill
	callq	_exit
	.cfi_endproc

	.globl	_app_error
	.p2align	4, 0x90
_app_error:                             ## @app_error
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp9:
	.cfi_def_cfa_offset 16
Ltmp10:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp11:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	leaq	L_.str.2(%rip), %rsi
	movq	___stderrp@GOTPCREL(%rip), %rax
	movq	%rdi, -8(%rbp)
	movq	(%rax), %rdi
	movq	-8(%rbp), %rdx
	movb	$0, %al
	callq	_fprintf
	xorl	%edi, %edi
	movl	%eax, -12(%rbp)         ## 4-byte Spill
	callq	_exit
	.cfi_endproc

	.globl	_Fork
	.p2align	4, 0x90
_Fork:                                  ## @Fork
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp12:
	.cfi_def_cfa_offset 16
Ltmp13:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp14:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	callq	_fork
	movl	%eax, -4(%rbp)
	cmpl	$0, %eax
	jge	LBB4_2
## BB#1:
	leaq	L_.str.3(%rip), %rdi
	callq	_unix_error
LBB4_2:
	movl	-4(%rbp), %eax
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Execve
	.p2align	4, 0x90
_Execve:                                ## @Execve
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp15:
	.cfi_def_cfa_offset 16
Ltmp16:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp17:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	movq	-24(%rbp), %rdx
	callq	_execve
	cmpl	$0, %eax
	jge	LBB5_2
## BB#1:
	leaq	L_.str.4(%rip), %rdi
	callq	_unix_error
LBB5_2:
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Wait
	.p2align	4, 0x90
_Wait:                                  ## @Wait
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp18:
	.cfi_def_cfa_offset 16
Ltmp19:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp20:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rdi
	callq	_wait
	movl	%eax, -12(%rbp)
	cmpl	$0, %eax
	jge	LBB6_2
## BB#1:
	leaq	L_.str.5(%rip), %rdi
	callq	_unix_error
LBB6_2:
	movl	-12(%rbp), %eax
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Waitpid
	.p2align	4, 0x90
_Waitpid:                               ## @Waitpid
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp21:
	.cfi_def_cfa_offset 16
Ltmp22:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp23:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movl	%edx, -20(%rbp)
	movl	-4(%rbp), %edi
	movq	-16(%rbp), %rsi
	movl	-20(%rbp), %edx
	callq	_waitpid
	movl	%eax, -24(%rbp)
	cmpl	$0, %eax
	jge	LBB7_2
## BB#1:
	leaq	L_.str.6(%rip), %rdi
	callq	_unix_error
LBB7_2:
	movl	-24(%rbp), %eax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Kill
	.p2align	4, 0x90
_Kill:                                  ## @Kill
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp24:
	.cfi_def_cfa_offset 16
Ltmp25:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp26:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	-4(%rbp), %edi
	movl	-8(%rbp), %esi
	callq	_kill
	movl	%eax, -12(%rbp)
	cmpl	$0, %eax
	jge	LBB8_2
## BB#1:
	leaq	L_.str.7(%rip), %rdi
	callq	_unix_error
LBB8_2:
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Pause
	.p2align	4, 0x90
_Pause:                                 ## @Pause
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp27:
	.cfi_def_cfa_offset 16
Ltmp28:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp29:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	callq	_pause
	movl	%eax, -4(%rbp)          ## 4-byte Spill
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Sleep
	.p2align	4, 0x90
_Sleep:                                 ## @Sleep
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp30:
	.cfi_def_cfa_offset 16
Ltmp31:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp32:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %edi
	callq	_sleep
	movl	%eax, -8(%rbp)
	cmpl	$0, %eax
	jae	LBB10_2
## BB#1:
	leaq	L_.str.8(%rip), %rdi
	callq	_unix_error
LBB10_2:
	movl	-8(%rbp), %eax
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Alarm
	.p2align	4, 0x90
_Alarm:                                 ## @Alarm
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp33:
	.cfi_def_cfa_offset 16
Ltmp34:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp35:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %edi
	callq	_alarm
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Setpgid
	.p2align	4, 0x90
_Setpgid:                               ## @Setpgid
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp36:
	.cfi_def_cfa_offset 16
Ltmp37:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp38:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	-4(%rbp), %edi
	movl	-8(%rbp), %esi
	callq	_setpgid
	movl	%eax, -12(%rbp)
	cmpl	$0, %eax
	jge	LBB12_2
## BB#1:
	leaq	L_.str.9(%rip), %rdi
	callq	_unix_error
LBB12_2:
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Getpgrp
	.p2align	4, 0x90
_Getpgrp:                               ## @Getpgrp
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp39:
	.cfi_def_cfa_offset 16
Ltmp40:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp41:
	.cfi_def_cfa_register %rbp
	callq	_getpgrp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Signal
	.p2align	4, 0x90
_Signal:                                ## @Signal
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp42:
	.cfi_def_cfa_offset 16
Ltmp43:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp44:
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	leaq	-32(%rbp), %rax
	leaq	-48(%rbp), %rdx
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-16(%rbp), %rsi
	movq	%rsi, -32(%rbp)
	movl	$0, -24(%rbp)
	movl	$2, -20(%rbp)
	movl	-4(%rbp), %edi
	movq	%rax, %rsi
	callq	_sigaction
	cmpl	$0, %eax
	jge	LBB14_2
## BB#1:
	leaq	L_.str.10(%rip), %rdi
	callq	_unix_error
LBB14_2:
	movq	-48(%rbp), %rax
	addq	$48, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Sigprocmask
	.p2align	4, 0x90
_Sigprocmask:                           ## @Sigprocmask
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp45:
	.cfi_def_cfa_offset 16
Ltmp46:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp47:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movl	-4(%rbp), %edi
	movq	-16(%rbp), %rsi
	movq	-24(%rbp), %rdx
	callq	_sigprocmask
	cmpl	$0, %eax
	jge	LBB15_2
## BB#1:
	leaq	L_.str.11(%rip), %rdi
	callq	_unix_error
LBB15_2:
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Sigemptyset
	.p2align	4, 0x90
_Sigemptyset:                           ## @Sigemptyset
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp48:
	.cfi_def_cfa_offset 16
Ltmp49:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp50:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	xorl	%eax, %eax
	movb	%al, %cl
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rdi
	movl	$0, (%rdi)
	testb	$1, %cl
	jne	LBB16_1
	jmp	LBB16_2
LBB16_1:
	leaq	L_.str.12(%rip), %rdi
	callq	_unix_error
LBB16_2:
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Sigfillset
	.p2align	4, 0x90
_Sigfillset:                            ## @Sigfillset
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp51:
	.cfi_def_cfa_offset 16
Ltmp52:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp53:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	xorl	%eax, %eax
	movb	%al, %cl
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rdi
	movl	$-1, (%rdi)
	testb	$1, %cl
	jne	LBB17_1
	jmp	LBB17_2
LBB17_1:
	leaq	L_.str.13(%rip), %rdi
	callq	_unix_error
LBB17_2:
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Sigaddset
	.p2align	4, 0x90
_Sigaddset:                             ## @Sigaddset
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp54:
	.cfi_def_cfa_offset 16
Ltmp55:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp56:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -16(%rbp)
	movl	%esi, -20(%rbp)
	movl	-20(%rbp), %esi
	movl	%esi, -4(%rbp)
	cmpl	$32, -4(%rbp)
	jle	LBB18_2
## BB#1:
	xorl	%eax, %eax
	movl	%eax, -24(%rbp)         ## 4-byte Spill
	jmp	LBB18_3
LBB18_2:
	movl	$1, %eax
	movl	-4(%rbp), %ecx
	subl	$1, %ecx
                                        ## kill: %CL<def> %ECX<kill>
	shll	%cl, %eax
	movl	%eax, -24(%rbp)         ## 4-byte Spill
LBB18_3:
	movl	-24(%rbp), %eax         ## 4-byte Reload
	xorl	%ecx, %ecx
	movb	%cl, %dl
	movq	-16(%rbp), %rsi
	orl	(%rsi), %eax
	movl	%eax, (%rsi)
	testb	$1, %dl
	jne	LBB18_4
	jmp	LBB18_5
LBB18_4:
	leaq	L_.str.14(%rip), %rdi
	callq	_unix_error
LBB18_5:
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Sigdelset
	.p2align	4, 0x90
_Sigdelset:                             ## @Sigdelset
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp57:
	.cfi_def_cfa_offset 16
Ltmp58:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp59:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -16(%rbp)
	movl	%esi, -20(%rbp)
	movl	-20(%rbp), %esi
	movl	%esi, -4(%rbp)
	cmpl	$32, -4(%rbp)
	jle	LBB19_2
## BB#1:
	xorl	%eax, %eax
	movl	%eax, -24(%rbp)         ## 4-byte Spill
	jmp	LBB19_3
LBB19_2:
	movl	$1, %eax
	movl	-4(%rbp), %ecx
	subl	$1, %ecx
                                        ## kill: %CL<def> %ECX<kill>
	shll	%cl, %eax
	movl	%eax, -24(%rbp)         ## 4-byte Spill
LBB19_3:
	movl	-24(%rbp), %eax         ## 4-byte Reload
	xorl	%ecx, %ecx
	movb	%cl, %dl
	xorl	$-1, %eax
	movq	-16(%rbp), %rsi
	andl	(%rsi), %eax
	movl	%eax, (%rsi)
	testb	$1, %dl
	jne	LBB19_4
	jmp	LBB19_5
LBB19_4:
	leaq	L_.str.15(%rip), %rdi
	callq	_unix_error
LBB19_5:
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Sigismember
	.p2align	4, 0x90
_Sigismember:                           ## @Sigismember
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp60:
	.cfi_def_cfa_offset 16
Ltmp61:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp62:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -16(%rbp)
	movl	%esi, -20(%rbp)
	movq	-16(%rbp), %rdi
	movl	(%rdi), %esi
	movl	-20(%rbp), %eax
	movl	%eax, -4(%rbp)
	cmpl	$32, -4(%rbp)
	movl	%esi, -28(%rbp)         ## 4-byte Spill
	jle	LBB20_2
## BB#1:
	xorl	%eax, %eax
	movl	%eax, -32(%rbp)         ## 4-byte Spill
	jmp	LBB20_3
LBB20_2:
	movl	$1, %eax
	movl	-4(%rbp), %ecx
	subl	$1, %ecx
                                        ## kill: %CL<def> %ECX<kill>
	shll	%cl, %eax
	movl	%eax, -32(%rbp)         ## 4-byte Spill
LBB20_3:
	movl	-32(%rbp), %eax         ## 4-byte Reload
	movl	-28(%rbp), %ecx         ## 4-byte Reload
	andl	%eax, %ecx
	cmpl	$0, %ecx
	setne	%dl
	andb	$1, %dl
	movzbl	%dl, %eax
	movl	%eax, -24(%rbp)
	cmpl	$0, %eax
	jge	LBB20_5
## BB#4:
	leaq	L_.str.16(%rip), %rdi
	callq	_unix_error
LBB20_5:
	movl	-24(%rbp), %eax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Open
	.p2align	4, 0x90
_Open:                                  ## @Open
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp63:
	.cfi_def_cfa_offset 16
Ltmp64:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp65:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movw	%dx, %ax
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movw	%ax, -14(%rbp)
	movq	-8(%rbp), %rdi
	movl	-12(%rbp), %esi
	movzwl	-14(%rbp), %edx
	movb	$0, %al
	callq	_open
	movl	%eax, -20(%rbp)
	cmpl	$0, %eax
	jge	LBB21_2
## BB#1:
	leaq	L_.str.17(%rip), %rdi
	callq	_unix_error
LBB21_2:
	movl	-20(%rbp), %eax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Read
	.p2align	4, 0x90
_Read:                                  ## @Read
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp66:
	.cfi_def_cfa_offset 16
Ltmp67:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp68:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movl	-4(%rbp), %edi
	movq	-16(%rbp), %rsi
	movq	-24(%rbp), %rdx
	callq	_read
	movq	%rax, -32(%rbp)
	cmpq	$0, %rax
	jge	LBB22_2
## BB#1:
	leaq	L_.str.18(%rip), %rdi
	callq	_unix_error
LBB22_2:
	movq	-32(%rbp), %rax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Write
	.p2align	4, 0x90
_Write:                                 ## @Write
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp69:
	.cfi_def_cfa_offset 16
Ltmp70:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp71:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movl	-4(%rbp), %edi
	movq	-16(%rbp), %rsi
	movq	-24(%rbp), %rdx
	callq	_write
	movq	%rax, -32(%rbp)
	cmpq	$0, %rax
	jge	LBB23_2
## BB#1:
	leaq	L_.str.19(%rip), %rdi
	callq	_unix_error
LBB23_2:
	movq	-32(%rbp), %rax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Lseek
	.p2align	4, 0x90
_Lseek:                                 ## @Lseek
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp72:
	.cfi_def_cfa_offset 16
Ltmp73:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp74:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movl	%edx, -20(%rbp)
	movl	-4(%rbp), %edi
	movq	-16(%rbp), %rsi
	movl	-20(%rbp), %edx
	callq	_lseek
	movq	%rax, -32(%rbp)
	cmpq	$0, %rax
	jge	LBB24_2
## BB#1:
	leaq	L_.str.20(%rip), %rdi
	callq	_unix_error
LBB24_2:
	movq	-32(%rbp), %rax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Close
	.p2align	4, 0x90
_Close:                                 ## @Close
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp75:
	.cfi_def_cfa_offset 16
Ltmp76:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp77:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %edi
	callq	_close
	movl	%eax, -8(%rbp)
	cmpl	$0, %eax
	jge	LBB25_2
## BB#1:
	leaq	L_.str.21(%rip), %rdi
	callq	_unix_error
LBB25_2:
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Select
	.p2align	4, 0x90
_Select:                                ## @Select
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp78:
	.cfi_def_cfa_offset 16
Ltmp79:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp80:
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	%rcx, -32(%rbp)
	movq	%r8, -40(%rbp)
	movl	-4(%rbp), %edi
	movq	-16(%rbp), %rsi
	movq	-24(%rbp), %rdx
	movq	-32(%rbp), %rcx
	movq	-40(%rbp), %r8
	callq	_select$1050
	movl	%eax, -44(%rbp)
	cmpl	$0, %eax
	jge	LBB26_2
## BB#1:
	leaq	L_.str.22(%rip), %rdi
	callq	_unix_error
LBB26_2:
	movl	-44(%rbp), %eax
	addq	$48, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Dup2
	.p2align	4, 0x90
_Dup2:                                  ## @Dup2
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp81:
	.cfi_def_cfa_offset 16
Ltmp82:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp83:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	-4(%rbp), %edi
	movl	-8(%rbp), %esi
	callq	_dup2
	movl	%eax, -12(%rbp)
	cmpl	$0, %eax
	jge	LBB27_2
## BB#1:
	leaq	L_.str.23(%rip), %rdi
	callq	_unix_error
LBB27_2:
	movl	-12(%rbp), %eax
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Stat
	.p2align	4, 0x90
_Stat:                                  ## @Stat
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp84:
	.cfi_def_cfa_offset 16
Ltmp85:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp86:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	callq	_stat$INODE64
	cmpl	$0, %eax
	jge	LBB28_2
## BB#1:
	leaq	L_.str.24(%rip), %rdi
	callq	_unix_error
LBB28_2:
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Fstat
	.p2align	4, 0x90
_Fstat:                                 ## @Fstat
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp87:
	.cfi_def_cfa_offset 16
Ltmp88:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp89:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movl	-4(%rbp), %edi
	movq	-16(%rbp), %rsi
	callq	_fstat$INODE64
	cmpl	$0, %eax
	jge	LBB29_2
## BB#1:
	leaq	L_.str.25(%rip), %rdi
	callq	_unix_error
LBB29_2:
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Mmap
	.p2align	4, 0x90
_Mmap:                                  ## @Mmap
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp90:
	.cfi_def_cfa_offset 16
Ltmp91:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp92:
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movl	%edx, -20(%rbp)
	movl	%ecx, -24(%rbp)
	movl	%r8d, -28(%rbp)
	movq	%r9, -40(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	movl	-20(%rbp), %edx
	movl	-24(%rbp), %ecx
	movl	-28(%rbp), %r8d
	movq	-40(%rbp), %r9
	callq	_mmap
	movq	$-1, %rsi
	movq	%rax, -48(%rbp)
	cmpq	%rsi, %rax
	jne	LBB30_2
## BB#1:
	leaq	L_.str.26(%rip), %rdi
	callq	_unix_error
LBB30_2:
	movq	-48(%rbp), %rax
	addq	$48, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Munmap
	.p2align	4, 0x90
_Munmap:                                ## @Munmap
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp93:
	.cfi_def_cfa_offset 16
Ltmp94:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp95:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	callq	_munmap
	cmpl	$0, %eax
	jge	LBB31_2
## BB#1:
	leaq	L_.str.27(%rip), %rdi
	callq	_unix_error
LBB31_2:
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Malloc
	.p2align	4, 0x90
_Malloc:                                ## @Malloc
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp96:
	.cfi_def_cfa_offset 16
Ltmp97:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp98:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rdi
	callq	_malloc
	movq	%rax, -16(%rbp)
	cmpq	$0, %rax
	jne	LBB32_2
## BB#1:
	leaq	L_.str.28(%rip), %rdi
	callq	_unix_error
LBB32_2:
	movq	-16(%rbp), %rax
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Realloc
	.p2align	4, 0x90
_Realloc:                               ## @Realloc
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp99:
	.cfi_def_cfa_offset 16
Ltmp100:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp101:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	callq	_realloc
	movq	%rax, -24(%rbp)
	cmpq	$0, %rax
	jne	LBB33_2
## BB#1:
	leaq	L_.str.29(%rip), %rdi
	callq	_unix_error
LBB33_2:
	movq	-24(%rbp), %rax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Calloc
	.p2align	4, 0x90
_Calloc:                                ## @Calloc
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp102:
	.cfi_def_cfa_offset 16
Ltmp103:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp104:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	callq	_calloc
	movq	%rax, -24(%rbp)
	cmpq	$0, %rax
	jne	LBB34_2
## BB#1:
	leaq	L_.str.30(%rip), %rdi
	callq	_unix_error
LBB34_2:
	movq	-24(%rbp), %rax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Free
	.p2align	4, 0x90
_Free:                                  ## @Free
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp105:
	.cfi_def_cfa_offset 16
Ltmp106:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp107:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rdi
	callq	_free
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Fclose
	.p2align	4, 0x90
_Fclose:                                ## @Fclose
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp108:
	.cfi_def_cfa_offset 16
Ltmp109:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp110:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rdi
	callq	_fclose
	cmpl	$0, %eax
	je	LBB36_2
## BB#1:
	leaq	L_.str.31(%rip), %rdi
	callq	_unix_error
LBB36_2:
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Fdopen
	.p2align	4, 0x90
_Fdopen:                                ## @Fdopen
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp111:
	.cfi_def_cfa_offset 16
Ltmp112:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp113:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movl	-4(%rbp), %edi
	movq	-16(%rbp), %rsi
	callq	_fdopen
	movq	%rax, -24(%rbp)
	cmpq	$0, %rax
	jne	LBB37_2
## BB#1:
	leaq	L_.str.32(%rip), %rdi
	callq	_unix_error
LBB37_2:
	movq	-24(%rbp), %rax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Fgets
	.p2align	4, 0x90
_Fgets:                                 ## @Fgets
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp114:
	.cfi_def_cfa_offset 16
Ltmp115:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp116:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-8(%rbp), %rdi
	movl	-12(%rbp), %esi
	movq	-24(%rbp), %rdx
	callq	_fgets
	movq	%rax, -32(%rbp)
	cmpq	$0, %rax
	jne	LBB38_3
## BB#1:
	movq	-24(%rbp), %rdi
	callq	_ferror
	cmpl	$0, %eax
	je	LBB38_3
## BB#2:
	leaq	L_.str.33(%rip), %rdi
	callq	_app_error
LBB38_3:
	movq	-32(%rbp), %rax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Fopen
	.p2align	4, 0x90
_Fopen:                                 ## @Fopen
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp117:
	.cfi_def_cfa_offset 16
Ltmp118:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp119:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	callq	_fopen
	movq	%rax, -24(%rbp)
	cmpq	$0, %rax
	jne	LBB39_2
## BB#1:
	leaq	L_.str.34(%rip), %rdi
	callq	_unix_error
LBB39_2:
	movq	-24(%rbp), %rax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Fputs
	.p2align	4, 0x90
_Fputs:                                 ## @Fputs
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp120:
	.cfi_def_cfa_offset 16
Ltmp121:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp122:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	callq	_fputs
	cmpl	$-1, %eax
	jne	LBB40_2
## BB#1:
	leaq	L_.str.35(%rip), %rdi
	callq	_unix_error
LBB40_2:
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Fread
	.p2align	4, 0x90
_Fread:                                 ## @Fread
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp123:
	.cfi_def_cfa_offset 16
Ltmp124:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp125:
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	%rcx, -32(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	movq	-24(%rbp), %rdx
	movq	-32(%rbp), %rcx
	callq	_fread
	movq	%rax, -40(%rbp)
	cmpq	-24(%rbp), %rax
	jae	LBB41_3
## BB#1:
	movq	-32(%rbp), %rdi
	callq	_ferror
	cmpl	$0, %eax
	je	LBB41_3
## BB#2:
	leaq	L_.str.36(%rip), %rdi
	callq	_unix_error
LBB41_3:
	movq	-40(%rbp), %rax
	addq	$48, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Fwrite
	.p2align	4, 0x90
_Fwrite:                                ## @Fwrite
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp126:
	.cfi_def_cfa_offset 16
Ltmp127:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp128:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	%rcx, -32(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	movq	-24(%rbp), %rdx
	movq	-32(%rbp), %rcx
	callq	_fwrite
	cmpq	-24(%rbp), %rax
	jae	LBB42_2
## BB#1:
	leaq	L_.str.37(%rip), %rdi
	callq	_unix_error
LBB42_2:
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Socket
	.p2align	4, 0x90
_Socket:                                ## @Socket
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp129:
	.cfi_def_cfa_offset 16
Ltmp130:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp131:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	%edx, -12(%rbp)
	movl	-4(%rbp), %edi
	movl	-8(%rbp), %esi
	movl	-12(%rbp), %edx
	callq	_socket
	movl	%eax, -16(%rbp)
	cmpl	$0, %eax
	jge	LBB43_2
## BB#1:
	leaq	L_.str.38(%rip), %rdi
	callq	_unix_error
LBB43_2:
	movl	-16(%rbp), %eax
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Setsockopt
	.p2align	4, 0x90
_Setsockopt:                            ## @Setsockopt
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp132:
	.cfi_def_cfa_offset 16
Ltmp133:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp134:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	%edx, -12(%rbp)
	movq	%rcx, -24(%rbp)
	movl	%r8d, -28(%rbp)
	movl	-4(%rbp), %edi
	movl	-8(%rbp), %esi
	movl	-12(%rbp), %edx
	movq	-24(%rbp), %rcx
	movl	-28(%rbp), %r8d
	callq	_setsockopt
	movl	%eax, -32(%rbp)
	cmpl	$0, %eax
	jge	LBB44_2
## BB#1:
	leaq	L_.str.39(%rip), %rdi
	callq	_unix_error
LBB44_2:
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Bind
	.p2align	4, 0x90
_Bind:                                  ## @Bind
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp135:
	.cfi_def_cfa_offset 16
Ltmp136:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp137:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movl	%edx, -20(%rbp)
	movl	-4(%rbp), %edi
	movq	-16(%rbp), %rsi
	movl	-20(%rbp), %edx
	callq	_bind
	movl	%eax, -24(%rbp)
	cmpl	$0, %eax
	jge	LBB45_2
## BB#1:
	leaq	L_.str.40(%rip), %rdi
	callq	_unix_error
LBB45_2:
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Listen
	.p2align	4, 0x90
_Listen:                                ## @Listen
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp138:
	.cfi_def_cfa_offset 16
Ltmp139:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp140:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	-4(%rbp), %edi
	movl	-8(%rbp), %esi
	callq	_listen
	movl	%eax, -12(%rbp)
	cmpl	$0, %eax
	jge	LBB46_2
## BB#1:
	leaq	L_.str.41(%rip), %rdi
	callq	_unix_error
LBB46_2:
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Accept
	.p2align	4, 0x90
_Accept:                                ## @Accept
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp141:
	.cfi_def_cfa_offset 16
Ltmp142:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp143:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movl	-4(%rbp), %edi
	movq	-16(%rbp), %rsi
	movq	-24(%rbp), %rdx
	callq	_accept
	movl	%eax, -28(%rbp)
	cmpl	$0, %eax
	jge	LBB47_2
## BB#1:
	leaq	L_.str.42(%rip), %rdi
	callq	_unix_error
LBB47_2:
	movl	-28(%rbp), %eax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Connect
	.p2align	4, 0x90
_Connect:                               ## @Connect
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp144:
	.cfi_def_cfa_offset 16
Ltmp145:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp146:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movl	%edx, -20(%rbp)
	movl	-4(%rbp), %edi
	movq	-16(%rbp), %rsi
	movl	-20(%rbp), %edx
	callq	_connect
	movl	%eax, -24(%rbp)
	cmpl	$0, %eax
	jge	LBB48_2
## BB#1:
	leaq	L_.str.43(%rip), %rdi
	callq	_unix_error
LBB48_2:
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Gethostbyname
	.p2align	4, 0x90
_Gethostbyname:                         ## @Gethostbyname
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp147:
	.cfi_def_cfa_offset 16
Ltmp148:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp149:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rdi
	callq	_gethostbyname
	movq	%rax, -16(%rbp)
	cmpq	$0, %rax
	jne	LBB49_2
## BB#1:
	leaq	L_.str.44(%rip), %rdi
	callq	_dns_error
LBB49_2:
	movq	-16(%rbp), %rax
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Gethostbyaddr
	.p2align	4, 0x90
_Gethostbyaddr:                         ## @Gethostbyaddr
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp150:
	.cfi_def_cfa_offset 16
Ltmp151:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp152:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movl	%edx, -16(%rbp)
	movq	-8(%rbp), %rdi
	movl	-12(%rbp), %esi
	movl	-16(%rbp), %edx
	callq	_gethostbyaddr
	movq	%rax, -24(%rbp)
	cmpq	$0, %rax
	jne	LBB50_2
## BB#1:
	leaq	L_.str.45(%rip), %rdi
	callq	_dns_error
LBB50_2:
	movq	-24(%rbp), %rax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Pthread_create
	.p2align	4, 0x90
_Pthread_create:                        ## @Pthread_create
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp153:
	.cfi_def_cfa_offset 16
Ltmp154:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp155:
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	%rcx, -32(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	movq	-24(%rbp), %rdx
	movq	-32(%rbp), %rcx
	callq	_pthread_create
	movl	%eax, -36(%rbp)
	cmpl	$0, %eax
	je	LBB51_2
## BB#1:
	leaq	L_.str.46(%rip), %rsi
	movl	-36(%rbp), %edi
	callq	_posix_error
LBB51_2:
	addq	$48, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Pthread_cancel
	.p2align	4, 0x90
_Pthread_cancel:                        ## @Pthread_cancel
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp156:
	.cfi_def_cfa_offset 16
Ltmp157:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp158:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rdi
	callq	_pthread_cancel
	movl	%eax, -12(%rbp)
	cmpl	$0, %eax
	je	LBB52_2
## BB#1:
	leaq	L_.str.47(%rip), %rsi
	movl	-12(%rbp), %edi
	callq	_posix_error
LBB52_2:
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Pthread_join
	.p2align	4, 0x90
_Pthread_join:                          ## @Pthread_join
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp159:
	.cfi_def_cfa_offset 16
Ltmp160:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp161:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	callq	_pthread_join
	movl	%eax, -20(%rbp)
	cmpl	$0, %eax
	je	LBB53_2
## BB#1:
	leaq	L_.str.48(%rip), %rsi
	movl	-20(%rbp), %edi
	callq	_posix_error
LBB53_2:
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Pthread_detach
	.p2align	4, 0x90
_Pthread_detach:                        ## @Pthread_detach
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp162:
	.cfi_def_cfa_offset 16
Ltmp163:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp164:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rdi
	callq	_pthread_detach
	movl	%eax, -12(%rbp)
	cmpl	$0, %eax
	je	LBB54_2
## BB#1:
	leaq	L_.str.49(%rip), %rsi
	movl	-12(%rbp), %edi
	callq	_posix_error
LBB54_2:
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Pthread_exit
	.p2align	4, 0x90
_Pthread_exit:                          ## @Pthread_exit
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp165:
	.cfi_def_cfa_offset 16
Ltmp166:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp167:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rdi
	callq	_pthread_exit
	.cfi_endproc

	.globl	_Pthread_self
	.p2align	4, 0x90
_Pthread_self:                          ## @Pthread_self
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp168:
	.cfi_def_cfa_offset 16
Ltmp169:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp170:
	.cfi_def_cfa_register %rbp
	callq	_pthread_self
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Pthread_once
	.p2align	4, 0x90
_Pthread_once:                          ## @Pthread_once
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp171:
	.cfi_def_cfa_offset 16
Ltmp172:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp173:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	callq	_pthread_once
	movl	%eax, -20(%rbp)         ## 4-byte Spill
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Sem_init
	.p2align	4, 0x90
_Sem_init:                              ## @Sem_init
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp174:
	.cfi_def_cfa_offset 16
Ltmp175:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp176:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movl	%edx, -16(%rbp)
	movq	-8(%rbp), %rdi
	movl	-12(%rbp), %esi
	movl	-16(%rbp), %edx
	callq	_sem_init
	cmpl	$0, %eax
	jge	LBB58_2
## BB#1:
	leaq	L_.str.50(%rip), %rdi
	callq	_unix_error
LBB58_2:
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_P
	.p2align	4, 0x90
_P:                                     ## @P
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp177:
	.cfi_def_cfa_offset 16
Ltmp178:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp179:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rdi
	callq	_sem_wait
	cmpl	$0, %eax
	jge	LBB59_2
## BB#1:
	leaq	L_.str.51(%rip), %rdi
	callq	_unix_error
LBB59_2:
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_V
	.p2align	4, 0x90
_V:                                     ## @V
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp180:
	.cfi_def_cfa_offset 16
Ltmp181:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp182:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rdi
	callq	_sem_post
	cmpl	$0, %eax
	jge	LBB60_2
## BB#1:
	leaq	L_.str.52(%rip), %rdi
	callq	_unix_error
LBB60_2:
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_rio_readn
	.p2align	4, 0x90
_rio_readn:                             ## @rio_readn
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp183:
	.cfi_def_cfa_offset 16
Ltmp184:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp185:
	.cfi_def_cfa_register %rbp
	subq	$64, %rsp
	movl	%edi, -12(%rbp)
	movq	%rsi, -24(%rbp)
	movq	%rdx, -32(%rbp)
	movq	-32(%rbp), %rdx
	movq	%rdx, -40(%rbp)
	movq	-24(%rbp), %rdx
	movq	%rdx, -56(%rbp)
LBB61_1:                                ## =>This Inner Loop Header: Depth=1
	cmpq	$0, -40(%rbp)
	jbe	LBB61_11
## BB#2:                                ##   in Loop: Header=BB61_1 Depth=1
	movl	-12(%rbp), %edi
	movq	-56(%rbp), %rsi
	movq	-40(%rbp), %rdx
	callq	_read
	movq	%rax, -48(%rbp)
	cmpq	$0, %rax
	jge	LBB61_7
## BB#3:                                ##   in Loop: Header=BB61_1 Depth=1
	callq	___error
	cmpl	$4, (%rax)
	jne	LBB61_5
## BB#4:                                ##   in Loop: Header=BB61_1 Depth=1
	movq	$0, -48(%rbp)
	jmp	LBB61_6
LBB61_5:
	movq	$-1, -8(%rbp)
	jmp	LBB61_12
LBB61_6:                                ##   in Loop: Header=BB61_1 Depth=1
	jmp	LBB61_10
LBB61_7:                                ##   in Loop: Header=BB61_1 Depth=1
	cmpq	$0, -48(%rbp)
	jne	LBB61_9
## BB#8:
	jmp	LBB61_11
LBB61_9:                                ##   in Loop: Header=BB61_1 Depth=1
	jmp	LBB61_10
LBB61_10:                               ##   in Loop: Header=BB61_1 Depth=1
	movq	-48(%rbp), %rax
	movq	-40(%rbp), %rcx
	subq	%rax, %rcx
	movq	%rcx, -40(%rbp)
	movq	-48(%rbp), %rax
	addq	-56(%rbp), %rax
	movq	%rax, -56(%rbp)
	jmp	LBB61_1
LBB61_11:
	movq	-32(%rbp), %rax
	subq	-40(%rbp), %rax
	movq	%rax, -8(%rbp)
LBB61_12:
	movq	-8(%rbp), %rax
	addq	$64, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_rio_writen
	.p2align	4, 0x90
_rio_writen:                            ## @rio_writen
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp186:
	.cfi_def_cfa_offset 16
Ltmp187:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp188:
	.cfi_def_cfa_register %rbp
	subq	$64, %rsp
	movl	%edi, -12(%rbp)
	movq	%rsi, -24(%rbp)
	movq	%rdx, -32(%rbp)
	movq	-32(%rbp), %rdx
	movq	%rdx, -40(%rbp)
	movq	-24(%rbp), %rdx
	movq	%rdx, -56(%rbp)
LBB62_1:                                ## =>This Inner Loop Header: Depth=1
	cmpq	$0, -40(%rbp)
	jbe	LBB62_8
## BB#2:                                ##   in Loop: Header=BB62_1 Depth=1
	movl	-12(%rbp), %edi
	movq	-56(%rbp), %rsi
	movq	-40(%rbp), %rdx
	callq	_write
	movq	%rax, -48(%rbp)
	cmpq	$0, %rax
	jg	LBB62_7
## BB#3:                                ##   in Loop: Header=BB62_1 Depth=1
	callq	___error
	cmpl	$4, (%rax)
	jne	LBB62_5
## BB#4:                                ##   in Loop: Header=BB62_1 Depth=1
	movq	$0, -48(%rbp)
	jmp	LBB62_6
LBB62_5:
	movq	$-1, -8(%rbp)
	jmp	LBB62_9
LBB62_6:                                ##   in Loop: Header=BB62_1 Depth=1
	jmp	LBB62_7
LBB62_7:                                ##   in Loop: Header=BB62_1 Depth=1
	movq	-48(%rbp), %rax
	movq	-40(%rbp), %rcx
	subq	%rax, %rcx
	movq	%rcx, -40(%rbp)
	movq	-48(%rbp), %rax
	addq	-56(%rbp), %rax
	movq	%rax, -56(%rbp)
	jmp	LBB62_1
LBB62_8:
	movq	-32(%rbp), %rax
	movq	%rax, -8(%rbp)
LBB62_9:
	movq	-8(%rbp), %rax
	addq	$64, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_rio_readinitb
	.p2align	4, 0x90
_rio_readinitb:                         ## @rio_readinitb
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp189:
	.cfi_def_cfa_offset 16
Ltmp190:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp191:
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movl	-12(%rbp), %esi
	movq	-8(%rbp), %rdi
	movl	%esi, (%rdi)
	movq	-8(%rbp), %rdi
	movl	$0, 4(%rdi)
	movq	-8(%rbp), %rdi
	addq	$16, %rdi
	movq	-8(%rbp), %rax
	movq	%rdi, 8(%rax)
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_rio_readnb
	.p2align	4, 0x90
_rio_readnb:                            ## @rio_readnb
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp192:
	.cfi_def_cfa_offset 16
Ltmp193:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp194:
	.cfi_def_cfa_register %rbp
	subq	$64, %rsp
	movq	%rdi, -16(%rbp)
	movq	%rsi, -24(%rbp)
	movq	%rdx, -32(%rbp)
	movq	-32(%rbp), %rdx
	movq	%rdx, -40(%rbp)
	movq	-24(%rbp), %rdx
	movq	%rdx, -56(%rbp)
LBB64_1:                                ## =>This Inner Loop Header: Depth=1
	cmpq	$0, -40(%rbp)
	jbe	LBB64_8
## BB#2:                                ##   in Loop: Header=BB64_1 Depth=1
	movq	-16(%rbp), %rdi
	movq	-56(%rbp), %rsi
	movq	-40(%rbp), %rdx
	callq	_rio_read
	movq	%rax, -48(%rbp)
	cmpq	$0, %rax
	jge	LBB64_4
## BB#3:
	movq	$-1, -8(%rbp)
	jmp	LBB64_9
LBB64_4:                                ##   in Loop: Header=BB64_1 Depth=1
	cmpq	$0, -48(%rbp)
	jne	LBB64_6
## BB#5:
	jmp	LBB64_8
LBB64_6:                                ##   in Loop: Header=BB64_1 Depth=1
	jmp	LBB64_7
LBB64_7:                                ##   in Loop: Header=BB64_1 Depth=1
	movq	-48(%rbp), %rax
	movq	-40(%rbp), %rcx
	subq	%rax, %rcx
	movq	%rcx, -40(%rbp)
	movq	-48(%rbp), %rax
	addq	-56(%rbp), %rax
	movq	%rax, -56(%rbp)
	jmp	LBB64_1
LBB64_8:
	movq	-32(%rbp), %rax
	subq	-40(%rbp), %rax
	movq	%rax, -8(%rbp)
LBB64_9:
	movq	-8(%rbp), %rax
	addq	$64, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.p2align	4, 0x90
_rio_read:                              ## @rio_read
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp195:
	.cfi_def_cfa_offset 16
Ltmp196:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp197:
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -16(%rbp)
	movq	%rsi, -24(%rbp)
	movq	%rdx, -32(%rbp)
LBB65_1:                                ## =>This Inner Loop Header: Depth=1
	movq	-16(%rbp), %rax
	cmpl	$0, 4(%rax)
	jg	LBB65_11
## BB#2:                                ##   in Loop: Header=BB65_1 Depth=1
	movl	$8192, %eax             ## imm = 0x2000
	movl	%eax, %edx
	movq	-16(%rbp), %rcx
	movl	(%rcx), %edi
	movq	-16(%rbp), %rcx
	addq	$16, %rcx
	movq	%rcx, %rsi
	callq	_read
	movl	%eax, %edi
	movq	-16(%rbp), %rax
	movl	%edi, 4(%rax)
	movq	-16(%rbp), %rax
	cmpl	$0, 4(%rax)
	jge	LBB65_6
## BB#3:                                ##   in Loop: Header=BB65_1 Depth=1
	callq	___error
	cmpl	$4, (%rax)
	je	LBB65_5
## BB#4:
	movq	$-1, -8(%rbp)
	jmp	LBB65_14
LBB65_5:                                ##   in Loop: Header=BB65_1 Depth=1
	jmp	LBB65_10
LBB65_6:                                ##   in Loop: Header=BB65_1 Depth=1
	movq	-16(%rbp), %rax
	cmpl	$0, 4(%rax)
	jne	LBB65_8
## BB#7:
	movq	$0, -8(%rbp)
	jmp	LBB65_14
LBB65_8:                                ##   in Loop: Header=BB65_1 Depth=1
	movq	-16(%rbp), %rax
	addq	$16, %rax
	movq	-16(%rbp), %rcx
	movq	%rax, 8(%rcx)
## BB#9:                                ##   in Loop: Header=BB65_1 Depth=1
	jmp	LBB65_10
LBB65_10:                               ##   in Loop: Header=BB65_1 Depth=1
	jmp	LBB65_1
LBB65_11:
	movq	-32(%rbp), %rax
	movl	%eax, %ecx
	movl	%ecx, -36(%rbp)
	movq	-16(%rbp), %rax
	movslq	4(%rax), %rax
	cmpq	-32(%rbp), %rax
	jae	LBB65_13
## BB#12:
	movq	-16(%rbp), %rax
	movl	4(%rax), %ecx
	movl	%ecx, -36(%rbp)
LBB65_13:
	movq	$-1, %rcx
	movq	-24(%rbp), %rdi
	movq	-16(%rbp), %rax
	movq	8(%rax), %rsi
	movslq	-36(%rbp), %rdx
	callq	___memcpy_chk
	movl	-36(%rbp), %r8d
	movq	-16(%rbp), %rcx
	movq	8(%rcx), %rdx
	movslq	%r8d, %rsi
	addq	%rsi, %rdx
	movq	%rdx, 8(%rcx)
	movl	-36(%rbp), %r8d
	movq	-16(%rbp), %rcx
	movl	4(%rcx), %r9d
	subl	%r8d, %r9d
	movl	%r9d, 4(%rcx)
	movslq	-36(%rbp), %rcx
	movq	%rcx, -8(%rbp)
	movq	%rax, -48(%rbp)         ## 8-byte Spill
LBB65_14:
	movq	-8(%rbp), %rax
	addq	$48, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_rio_readlineb
	.p2align	4, 0x90
_rio_readlineb:                         ## @rio_readlineb
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp198:
	.cfi_def_cfa_offset 16
Ltmp199:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp200:
	.cfi_def_cfa_register %rbp
	subq	$64, %rsp
	movq	%rdi, -16(%rbp)
	movq	%rsi, -24(%rbp)
	movq	%rdx, -32(%rbp)
	movq	-24(%rbp), %rdx
	movq	%rdx, -56(%rbp)
	movl	$1, -36(%rbp)
LBB66_1:                                ## =>This Inner Loop Header: Depth=1
	movslq	-36(%rbp), %rax
	cmpq	-32(%rbp), %rax
	jae	LBB66_13
## BB#2:                                ##   in Loop: Header=BB66_1 Depth=1
	leaq	-41(%rbp), %rsi
	movl	$1, %eax
	movl	%eax, %edx
	movq	-16(%rbp), %rdi
	callq	_rio_read
	movl	%eax, %ecx
	movl	%ecx, -40(%rbp)
	cmpl	$1, %ecx
	jne	LBB66_6
## BB#3:                                ##   in Loop: Header=BB66_1 Depth=1
	movb	-41(%rbp), %al
	movq	-56(%rbp), %rcx
	movq	%rcx, %rdx
	addq	$1, %rdx
	movq	%rdx, -56(%rbp)
	movb	%al, (%rcx)
	movsbl	-41(%rbp), %esi
	cmpl	$10, %esi
	jne	LBB66_5
## BB#4:
	movl	-36(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -36(%rbp)
	jmp	LBB66_13
LBB66_5:                                ##   in Loop: Header=BB66_1 Depth=1
	jmp	LBB66_11
LBB66_6:
	cmpl	$0, -40(%rbp)
	jne	LBB66_10
## BB#7:
	cmpl	$1, -36(%rbp)
	jne	LBB66_9
## BB#8:
	movq	$0, -8(%rbp)
	jmp	LBB66_14
LBB66_9:
	jmp	LBB66_13
LBB66_10:
	movq	$-1, -8(%rbp)
	jmp	LBB66_14
LBB66_11:                               ##   in Loop: Header=BB66_1 Depth=1
	jmp	LBB66_12
LBB66_12:                               ##   in Loop: Header=BB66_1 Depth=1
	movl	-36(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -36(%rbp)
	jmp	LBB66_1
LBB66_13:
	movq	-56(%rbp), %rax
	movb	$0, (%rax)
	movl	-36(%rbp), %ecx
	subl	$1, %ecx
	movslq	%ecx, %rax
	movq	%rax, -8(%rbp)
LBB66_14:
	movq	-8(%rbp), %rax
	addq	$64, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Rio_readn
	.p2align	4, 0x90
_Rio_readn:                             ## @Rio_readn
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp201:
	.cfi_def_cfa_offset 16
Ltmp202:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp203:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movl	-4(%rbp), %edi
	movq	-16(%rbp), %rsi
	movq	-24(%rbp), %rdx
	callq	_rio_readn
	movq	%rax, -32(%rbp)
	cmpq	$0, %rax
	jge	LBB67_2
## BB#1:
	leaq	L_.str.53(%rip), %rdi
	callq	_unix_error
LBB67_2:
	movq	-32(%rbp), %rax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Rio_writen
	.p2align	4, 0x90
_Rio_writen:                            ## @Rio_writen
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp204:
	.cfi_def_cfa_offset 16
Ltmp205:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp206:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movl	-4(%rbp), %edi
	movq	-16(%rbp), %rsi
	movq	-24(%rbp), %rdx
	callq	_rio_writen
	cmpq	-24(%rbp), %rax
	je	LBB68_2
## BB#1:
	leaq	L_.str.54(%rip), %rdi
	callq	_unix_error
LBB68_2:
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Rio_readinitb
	.p2align	4, 0x90
_Rio_readinitb:                         ## @Rio_readinitb
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp207:
	.cfi_def_cfa_offset 16
Ltmp208:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp209:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movq	-8(%rbp), %rdi
	movl	-12(%rbp), %esi
	callq	_rio_readinitb
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Rio_readnb
	.p2align	4, 0x90
_Rio_readnb:                            ## @Rio_readnb
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp210:
	.cfi_def_cfa_offset 16
Ltmp211:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp212:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	movq	-24(%rbp), %rdx
	callq	_rio_readnb
	movq	%rax, -32(%rbp)
	cmpq	$0, %rax
	jge	LBB70_2
## BB#1:
	leaq	L_.str.55(%rip), %rdi
	callq	_unix_error
LBB70_2:
	movq	-32(%rbp), %rax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Rio_readlineb
	.p2align	4, 0x90
_Rio_readlineb:                         ## @Rio_readlineb
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp213:
	.cfi_def_cfa_offset 16
Ltmp214:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp215:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	movq	-24(%rbp), %rdx
	callq	_rio_readlineb
	movq	%rax, -32(%rbp)
	cmpq	$0, %rax
	jge	LBB71_2
## BB#1:
	leaq	L_.str.56(%rip), %rdi
	callq	_unix_error
LBB71_2:
	movq	-32(%rbp), %rax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_open_clientfd
	.p2align	4, 0x90
_open_clientfd:                         ## @open_clientfd
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp216:
	.cfi_def_cfa_offset 16
Ltmp217:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp218:
	.cfi_def_cfa_register %rbp
	subq	$80, %rsp
	movl	$2, %eax
	movl	$1, %ecx
	xorl	%edx, %edx
	movq	___stack_chk_guard@GOTPCREL(%rip), %r8
	movq	(%r8), %r8
	movq	%r8, -8(%rbp)
	movq	%rdi, -40(%rbp)
	movl	%esi, -44(%rbp)
	movl	%eax, %edi
	movl	%ecx, %esi
	callq	_socket
	movl	%eax, -48(%rbp)
	cmpl	$0, %eax
	jge	LBB72_2
## BB#1:
	movl	$-1, -28(%rbp)
	jmp	LBB72_7
LBB72_2:
	movq	-40(%rbp), %rdi
	callq	_gethostbyname
	movq	%rax, -56(%rbp)
	cmpq	$0, %rax
	jne	LBB72_4
## BB#3:
	movl	$-2, -28(%rbp)
	jmp	LBB72_7
LBB72_4:
	leaq	-24(%rbp), %rax
	xorl	%esi, %esi
	movl	$16, %ecx
	movl	%ecx, %edx
	movq	%rax, %rdi
	movq	%rax, -64(%rbp)         ## 8-byte Spill
	callq	_memset
	movb	$2, -23(%rbp)
	movq	-56(%rbp), %rax
	movq	24(%rax), %rax
	movq	(%rax), %rdi
	movq	-64(%rbp), %rax         ## 8-byte Reload
	addq	$4, %rax
	movq	-56(%rbp), %rdx
	movslq	20(%rdx), %rdx
	movq	%rax, %rsi
	callq	_bcopy
	movl	-44(%rbp), %ecx
	movw	%cx, %r8w
	movzwl	%r8w, %edi
	callq	__OSSwapInt16
	movl	$16, %edx
	leaq	-24(%rbp), %rsi
	movzwl	%ax, %ecx
	movw	%cx, %ax
	movw	%ax, -22(%rbp)
	movl	-48(%rbp), %edi
	callq	_connect
	cmpl	$0, %eax
	jge	LBB72_6
## BB#5:
	movl	$-1, -28(%rbp)
	jmp	LBB72_7
LBB72_6:
	movl	-48(%rbp), %eax
	movl	%eax, -28(%rbp)
LBB72_7:
	movl	-28(%rbp), %eax
	movq	___stack_chk_guard@GOTPCREL(%rip), %rcx
	movq	(%rcx), %rcx
	movq	-8(%rbp), %rdx
	cmpq	%rdx, %rcx
	movl	%eax, -68(%rbp)         ## 4-byte Spill
	jne	LBB72_9
## BB#8:
	movl	-68(%rbp), %eax         ## 4-byte Reload
	addq	$80, %rsp
	popq	%rbp
	retq
LBB72_9:
	callq	___stack_chk_fail
	.cfi_endproc

	.p2align	4, 0x90
__OSSwapInt16:                          ## @_OSSwapInt16
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp219:
	.cfi_def_cfa_offset 16
Ltmp220:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp221:
	.cfi_def_cfa_register %rbp
	movw	%di, %ax
	movw	%ax, -2(%rbp)
	movzwl	-2(%rbp), %edi
	shll	$8, %edi
	movzwl	-2(%rbp), %ecx
	sarl	$8, %ecx
	orl	%ecx, %edi
	movw	%di, %ax
	movzwl	%ax, %eax
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_open_listenfd
	.p2align	4, 0x90
_open_listenfd:                         ## @open_listenfd
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp222:
	.cfi_def_cfa_offset 16
Ltmp223:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp224:
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movl	$2, %eax
	movl	$1, %esi
	xorl	%edx, %edx
	movq	___stack_chk_guard@GOTPCREL(%rip), %rcx
	movq	(%rcx), %rcx
	movq	%rcx, -8(%rbp)
	movl	%edi, -32(%rbp)
	movl	$1, -40(%rbp)
	movl	%eax, %edi
	callq	_socket
	movl	%eax, -36(%rbp)
	cmpl	$0, %eax
	jge	LBB74_2
## BB#1:
	movl	$-1, -28(%rbp)
	jmp	LBB74_9
LBB74_2:
	movl	$65535, %esi            ## imm = 0xFFFF
	movl	$4, %eax
	leaq	-40(%rbp), %rcx
	movl	-36(%rbp), %edi
	movl	%eax, %edx
	movl	%eax, %r8d
	callq	_setsockopt
	cmpl	$0, %eax
	jge	LBB74_4
## BB#3:
	movl	$-1, -28(%rbp)
	jmp	LBB74_9
LBB74_4:
	xorl	%esi, %esi
	movl	$16, %eax
	movl	%eax, %edx
	leaq	-24(%rbp), %rcx
	movq	%rcx, %rdi
	callq	_memset
	movb	$2, -23(%rbp)
	movl	$0, -20(%rbp)
	movl	-32(%rbp), %eax
	movw	%ax, %r8w
	movzwl	%r8w, %edi
	callq	__OSSwapInt16
	movl	$16, %edx
	leaq	-24(%rbp), %rcx
	movzwl	%ax, %esi
	movw	%si, %ax
	movw	%ax, -22(%rbp)
	movl	-36(%rbp), %edi
	movq	%rcx, %rsi
	callq	_bind
	cmpl	$0, %eax
	jge	LBB74_6
## BB#5:
	movl	$-1, -28(%rbp)
	jmp	LBB74_9
LBB74_6:
	movl	$1024, %esi             ## imm = 0x400
	movl	-36(%rbp), %edi
	callq	_listen
	cmpl	$0, %eax
	jge	LBB74_8
## BB#7:
	movl	$-1, -28(%rbp)
	jmp	LBB74_9
LBB74_8:
	movl	-36(%rbp), %eax
	movl	%eax, -28(%rbp)
LBB74_9:
	movl	-28(%rbp), %eax
	movq	___stack_chk_guard@GOTPCREL(%rip), %rcx
	movq	(%rcx), %rcx
	movq	-8(%rbp), %rdx
	cmpq	%rdx, %rcx
	movl	%eax, -44(%rbp)         ## 4-byte Spill
	jne	LBB74_11
## BB#10:
	movl	-44(%rbp), %eax         ## 4-byte Reload
	addq	$48, %rsp
	popq	%rbp
	retq
LBB74_11:
	callq	___stack_chk_fail
	.cfi_endproc

	.globl	_Open_clientfd
	.p2align	4, 0x90
_Open_clientfd:                         ## @Open_clientfd
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp225:
	.cfi_def_cfa_offset 16
Ltmp226:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp227:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movq	-8(%rbp), %rdi
	movl	-12(%rbp), %esi
	callq	_open_clientfd
	movl	%eax, -16(%rbp)
	cmpl	$0, %eax
	jge	LBB75_5
## BB#1:
	cmpl	$-1, -16(%rbp)
	jne	LBB75_3
## BB#2:
	leaq	L_.str.57(%rip), %rdi
	callq	_unix_error
	jmp	LBB75_4
LBB75_3:
	leaq	L_.str.58(%rip), %rdi
	callq	_dns_error
LBB75_4:
	jmp	LBB75_5
LBB75_5:
	movl	-16(%rbp), %eax
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_Open_listenfd
	.p2align	4, 0x90
_Open_listenfd:                         ## @Open_listenfd
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp228:
	.cfi_def_cfa_offset 16
Ltmp229:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp230:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %edi
	callq	_open_listenfd
	movl	%eax, -8(%rbp)
	cmpl	$0, %eax
	jge	LBB76_2
## BB#1:
	leaq	L_.str.59(%rip), %rdi
	callq	_unix_error
LBB76_2:
	movl	-8(%rbp), %eax
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"%s: %s\n"

L_.str.1:                               ## @.str.1
	.asciz	"%s: DNS error %d\n"

L_.str.2:                               ## @.str.2
	.asciz	"%s\n"

L_.str.3:                               ## @.str.3
	.asciz	"Fork error"

L_.str.4:                               ## @.str.4
	.asciz	"Execve error"

L_.str.5:                               ## @.str.5
	.asciz	"Wait error"

L_.str.6:                               ## @.str.6
	.asciz	"Waitpid error"

L_.str.7:                               ## @.str.7
	.asciz	"Kill error"

L_.str.8:                               ## @.str.8
	.asciz	"Sleep error"

L_.str.9:                               ## @.str.9
	.asciz	"Setpgid error"

L_.str.10:                              ## @.str.10
	.asciz	"Signal error"

L_.str.11:                              ## @.str.11
	.asciz	"Sigprocmask error"

L_.str.12:                              ## @.str.12
	.asciz	"Sigemptyset error"

L_.str.13:                              ## @.str.13
	.asciz	"Sigfillset error"

L_.str.14:                              ## @.str.14
	.asciz	"Sigaddset error"

L_.str.15:                              ## @.str.15
	.asciz	"Sigdelset error"

L_.str.16:                              ## @.str.16
	.asciz	"Sigismember error"

L_.str.17:                              ## @.str.17
	.asciz	"Open error"

L_.str.18:                              ## @.str.18
	.asciz	"Read error"

L_.str.19:                              ## @.str.19
	.asciz	"Write error"

L_.str.20:                              ## @.str.20
	.asciz	"Lseek error"

L_.str.21:                              ## @.str.21
	.asciz	"Close error"

L_.str.22:                              ## @.str.22
	.asciz	"Select error"

L_.str.23:                              ## @.str.23
	.asciz	"Dup2 error"

L_.str.24:                              ## @.str.24
	.asciz	"Stat error"

L_.str.25:                              ## @.str.25
	.asciz	"Fstat error"

L_.str.26:                              ## @.str.26
	.asciz	"mmap error"

L_.str.27:                              ## @.str.27
	.asciz	"munmap error"

L_.str.28:                              ## @.str.28
	.asciz	"Malloc error"

L_.str.29:                              ## @.str.29
	.asciz	"Realloc error"

L_.str.30:                              ## @.str.30
	.asciz	"Calloc error"

L_.str.31:                              ## @.str.31
	.asciz	"Fclose error"

L_.str.32:                              ## @.str.32
	.asciz	"Fdopen error"

L_.str.33:                              ## @.str.33
	.asciz	"Fgets error"

L_.str.34:                              ## @.str.34
	.asciz	"Fopen error"

L_.str.35:                              ## @.str.35
	.asciz	"Fputs error"

L_.str.36:                              ## @.str.36
	.asciz	"Fread error"

L_.str.37:                              ## @.str.37
	.asciz	"Fwrite error"

L_.str.38:                              ## @.str.38
	.asciz	"Socket error"

L_.str.39:                              ## @.str.39
	.asciz	"Setsockopt error"

L_.str.40:                              ## @.str.40
	.asciz	"Bind error"

L_.str.41:                              ## @.str.41
	.asciz	"Listen error"

L_.str.42:                              ## @.str.42
	.asciz	"Accept error"

L_.str.43:                              ## @.str.43
	.asciz	"Connect error"

L_.str.44:                              ## @.str.44
	.asciz	"Gethostbyname error"

L_.str.45:                              ## @.str.45
	.asciz	"Gethostbyaddr error"

L_.str.46:                              ## @.str.46
	.asciz	"Pthread_create error"

L_.str.47:                              ## @.str.47
	.asciz	"Pthread_cancel error"

L_.str.48:                              ## @.str.48
	.asciz	"Pthread_join error"

L_.str.49:                              ## @.str.49
	.asciz	"Pthread_detach error"

L_.str.50:                              ## @.str.50
	.asciz	"Sem_init error"

L_.str.51:                              ## @.str.51
	.asciz	"P error"

L_.str.52:                              ## @.str.52
	.asciz	"V error"

L_.str.53:                              ## @.str.53
	.asciz	"Rio_readn error"

L_.str.54:                              ## @.str.54
	.asciz	"Rio_writen error"

L_.str.55:                              ## @.str.55
	.asciz	"Rio_readnb error"

L_.str.56:                              ## @.str.56
	.asciz	"Rio_readlineb error"

L_.str.57:                              ## @.str.57
	.asciz	"Open_clientfd Unix error"

L_.str.58:                              ## @.str.58
	.asciz	"Open_clientfd DNS error"

L_.str.59:                              ## @.str.59
	.asciz	"Open_listenfd error"


.subsections_via_symbols
