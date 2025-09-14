	.file	"MainModule"
	.text
	.globl	bytostr                         # -- Begin function bytostr
	.p2align	4
	.type	bytostr,@function
bytostr:                                # @bytostr
	.cfi_startproc
# %bb.0:                                # %bytostr
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$32, %rsp
	.cfi_def_cfa_offset 48
	.cfi_offset %rbx, -16
	movq	%rdi, 24(%rsp)
	movl	%esi, 20(%rsp)
	leaq	.L.str(%rip), %rax
	movq	%rax, 8(%rsp)
	movl	$0, 4(%rsp)
	.p2align	4
.LBB0_1:                                # %for_start_1
                                        # =>This Inner Loop Header: Depth=1
	movl	4(%rsp), %eax
	cmpl	20(%rsp), %eax
	jge	.LBB0_3
# %bb.2:                                # %for_body_1
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	8(%rsp), %rbx
	movq	24(%rsp), %rdi
	movl	4(%rsp), %esi
	leal	1(%rsi), %edx
                                        # kill: def $esi killed $esi killed $rsi
	callq	strsub@PLT
	movq	%rbx, %rdi
	movq	%rax, %rsi
	callq	strcat@PLT
	movq	%rax, 8(%rsp)
	incl	4(%rsp)
	jmp	.LBB0_1
.LBB0_3:                                # %for_end_1
	movq	8(%rsp), %rax
	addq	$32, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	bytostr, .Lfunc_end0-bytostr
	.cfi_endproc
                                        # -- End function
	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.zero	1
	.size	.L.str, 1

	.section	".note.GNU-stack","",@progbits
