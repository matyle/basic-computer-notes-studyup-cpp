int accum=0;
int sum(int x,int y){
    
    int t = x+y;
    accum+=t;
    return t;
}
/*
  - ```c
  	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	-4(%rbp), %eax
	addl	-8(%rbp), %eax
	movl	%eax, -12(%rbp)
	movl	-12(%rbp), %eax
	addl	_accum(%rip), %eax
	movl	%eax, _accum(%rip)// 不确定放哪里
	movl	-12(%rbp), %eax
	popq	%rbp
  ```
0000000000000000 _sum:
       0: 55                            pushq   %rbp
       1: 48 89 e5                      movq    %rsp, %rbp
       4: 89 7d fc                      movl    %edi, -4(%rbp)
       7: 89 75 f8                      movl    %esi, -8(%rbp)
       a: 8b 45 fc                      movl    -4(%rbp), %eax
       d: 03 45 f8                      addl    -8(%rbp), %eax
      10: 89 45 f4                      movl    %eax, -12(%rbp)
      13: 8b 45 f4                      movl    -12(%rbp), %eax
      16: 03 05 00 00 00 00             addl    (%rip), %eax
      1c: 89 05 00 00 00 00             movl    %eax, (%rip)
      22: 8b 45 f4                      movl    -12(%rbp), %eax
      25: 5d                            popq    %rbp
      26: c3                            retq



  0000000100003f70 _sum:
100003f70: 55                           pushq   %rbp
100003f71: 48 89 e5                     movq    %rsp, %rbp
100003f74: 89 7d fc                     movl    %edi, -4(%rbp)
100003f77: 89 75 f8                     movl    %esi, -8(%rbp)
100003f7a: 8b 45 fc                     movl    -4(%rbp), %eax
100003f7d: 03 45 f8                     addl    -8(%rbp), %eax
100003f80: 89 45 f4                     movl    %eax, -12(%rbp)
100003f83: 8b 45 f4                     movl    -12(%rbp), %eax
100003f86: 03 05 74 00 00 00            addl    116(%rip), %eax
100003f8c: 89 05 6e 00 00 00            movl    %eax, 110(%rip)  //6e 00 00 00  
100003f92: 8b 45 f4                     movl    -12(%rbp), %eax
100003f95: 5d                           popq    %rbp
100003f96: c3                           retq
100003f97: 90                           nop

0000000100003fa0 _main:
100003fa0: 55                           pushq   %rbp
100003fa1: 48 89 e5                     movq    %rsp, %rbp
100003fa4: bf 01 00 00 00               movl    $1, %edi
100003fa9: be 03 00 00 00               movl    $3, %esi
100003fae: 5d                           popq    %rbp
100003faf: e9 bc ff ff ff               jmp     -68 <_sum>
*/