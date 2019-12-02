	.file	"test1.c"
	.globl	a
	.data
	.align 4
	.type	a, @object
	.size	a, 4
a:
	.long	50
	.globl	c
	.align 4
	.type	c, @object
	.size	c, 4
c:
	.long	10
	.globl	v
	.align 16
	.type	v, @object
	.size	v, 16
v:
	.long	10
	.long	20
	.long	30
	.long	1
	.text
	.globl	fun
	.type	fun, @function
fun:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	-4(%rbp), %eax
	subl	-8(%rbp), %eax
	movl	%eax, c(%rip)
	movl	c(%rip), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	fun, .-fun
	.section	.rodata
.LC0:
	.string	"%d"
	.text
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$8, v+8(%rip)
	jmp	.L4
.L5:
	movl	v+8(%rip), %edx
	movl	a(%rip), %eax
	movl	%edx, %esi
	movl	%eax, %edi
	call	fun
	movl	%eax, a(%rip)
	movl	a(%rip), %eax
	movl	%eax, %esi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
.L4:
	movl	a(%rip), %eax
	testl	%eax, %eax
	jg	.L5
	movl	$1, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
