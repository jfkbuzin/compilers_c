	.file	"test6.c"
	.globl	a
	.data
	.align 4
	.type	a, @object
	.size	a, 4
a:
	.long	7
	.globl	five
	.align 4
	.type	five, @object
	.size	five, 4
five:
	.long	5
	.section	.rodata
.LC0:
	.string	"oi %d"
.LC1:
	.string	"whatevs"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	a(%rip), %edx
	movl	five(%rip), %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, a(%rip)
	movl	a(%rip), %edx
	movl	five(%rip), %eax
	imull	%edx, %eax
	movl	%eax, a(%rip)
	movl	a(%rip), %eax
	movl	five(%rip), %ecx
	cltd
	idivl	%ecx
	movl	%eax, a(%rip)
	movl	a(%rip), %eax
	movl	%eax, %esi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits