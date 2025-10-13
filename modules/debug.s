	.file	"MainModule"
	.text
	.globl	helloworld$MODdebug             # -- Begin function helloworld$MODdebug
	.p2align	4
	.type	helloworld$MODdebug,@function
helloworld$MODdebug:                    # @"helloworld$MODdebug"
	.cfi_startproc
# %bb.0:                                # %"helloworld$MODdebug"
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -16
	leaq	.L.str(%rip), %rbx
	movq	%rbx, 8(%rsp)
	movq	%rbx, %rdi
	callq	len$MODstring@PLT
	movl	$2, %edi
	movq	%rbx, %rsi
	movl	%eax, %edx
	callq	write$MODstd@PLT
	addq	$16, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	helloworld$MODdebug, .Lfunc_end0-helloworld$MODdebug
	.cfi_endproc
                                        # -- End function
	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"Hello world!\n"
	.size	.L.str, 14

	.section	".note.GNU-stack","",@progbits
