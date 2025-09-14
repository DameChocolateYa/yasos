	.file	"MainModule"
	.text
	.globl	panic                           # -- Begin function panic
	.p2align	4
	.type	panic,@function
panic:                                  # @panic
	.cfi_startproc
# %bb.0:                                # %panic
	pushq	%rax
	.cfi_def_cfa_offset 16
	movq	%rdi, %rsi
	movq	%rdi, (%rsp)
	leaq	.L.str(%rip), %rdi
	xorl	%eax, %eax
	callq	println@PLT
	movl	$1, %edi
	callq	exit@PLT
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	panic, .Lfunc_end0-panic
	.cfi_endproc
                                        # -- End function
	.globl	cuspanic                        # -- Begin function cuspanic
	.p2align	4
	.type	cuspanic,@function
cuspanic:                               # @cuspanic
	.cfi_startproc
# %bb.0:                                # %cuspanic
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movq	%rdi, %rax
	movq	%rdi, 16(%rsp)
	movl	%esi, 12(%rsp)
	leaq	.L.str.1(%rip), %rdi
	movq	%rax, %rsi
	xorl	%eax, %eax
	callq	println@PLT
	movl	12(%rsp), %edi
	callq	exit@PLT
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	cuspanic, .Lfunc_end1-cuspanic
	.cfi_endproc
                                        # -- End function
	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"%s"
	.size	.L.str, 3

	.type	.L.str.1,@object                # @.str.1
.L.str.1:
	.asciz	"%s"
	.size	.L.str.1, 3

	.section	".note.GNU-stack","",@progbits
