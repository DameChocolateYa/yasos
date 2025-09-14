	.file	"MainModule"
	.text
	.globl	vec_new                         # -- Begin function vec_new
	.p2align	4
	.type	vec_new,@function
vec_new:                                # @vec_new
	.cfi_startproc
# %bb.0:                                # %vec_new
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movl	$0, 8(%rsp)
	movl	$16, %edi
	callq	malloc@PLT
	movq	%rax, 8(%rsp)
	movq	$2, 16(%rsp)
	movl	$2, %edx
	xorl	%ecx, %ecx
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	vec_new, .Lfunc_end0-vec_new
	.cfi_endproc
                                        # -- End function
	.globl	vec_push                        # -- Begin function vec_push
	.p2align	4
	.type	vec_push,@function
vec_push:                               # @vec_push
	.cfi_startproc
# %bb.0:                                # %vec_push
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -32(%rbp)
	movq	%rsi, -24(%rbp)
	movl	$0, -16(%rbp)
	movl	8(%rdi), %eax
	movq	(%rdi), %rcx
	movl	12(%rdi), %edx
	movl	%edx, -4(%rbp)
	movq	%rcx, -16(%rbp)
	movl	%eax, -8(%rbp)
	incl	%edx
	cmpl	%eax, %edx
	jne	.LBB1_2
# %bb.1:                                # %if_0
	movq	-16(%rbp), %rdi
	movl	-8(%rbp), %esi
	leal	(%rsi,%rsi), %eax
	movl	%eax, -8(%rbp)
	shll	$4, %esi
                                        # kill: def $esi killed $esi killed $rsi
	callq	realloc@PLT
	movq	%rax, -16(%rbp)
.LBB1_2:                                # %endif_0
	movq	-16(%rbp), %rax
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rsp
	movq	%rax, -16(%rcx)
	movslq	-4(%rbp), %rdx
	movq	-24(%rbp), %rsi
	movq	%rsi, (%rax,%rdx,4)
	movl	-8(%rbp), %eax
	movl	-4(%rbp), %edx
	movq	-16(%rcx), %rcx
	movq	%rcx, -16(%rbp)
	incl	%edx
	movl	%edx, -4(%rbp)
	movq	-32(%rbp), %rsi
	movl	%edx, 12(%rsi)
	movl	%eax, 8(%rsi)
	movq	%rcx, (%rsi)
	movq	%rbp, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end1:
	.size	vec_push, .Lfunc_end1-vec_push
	.cfi_endproc
                                        # -- End function
	.globl	vec_pop                         # -- Begin function vec_pop
	.p2align	4
	.type	vec_pop,@function
vec_pop:                                # @vec_pop
	.cfi_startproc
# %bb.0:                                # %vec_pop
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	$0, -16(%rbp)
	movl	8(%rdi), %eax
	movq	(%rdi), %rcx
	movl	12(%rdi), %edx
	movl	%edx, -4(%rbp)
	movq	%rcx, -16(%rbp)
	movl	%eax, -8(%rbp)
	testl	%edx, %edx
	jne	.LBB2_1
# %bb.2:                                # %if_1
	leaq	.L.str(%rip), %rdi
	xorl	%eax, %eax
	callq	print@PLT
	xorl	%eax, %eax
	jmp	.LBB2_3
.LBB2_1:                                # %else_1
	movq	-16(%rbp), %rax
	decl	-4(%rbp)
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rsp
	movq	%rax, -16(%rcx)
	movslq	-4(%rbp), %rdx
	movl	(%rax,%rdx,4), %eax
	movq	%rsp, %rdx
	leaq	-16(%rdx), %rsp
	movl	%eax, -16(%rdx)
	movslq	-4(%rbp), %rax
	movq	-16(%rcx), %rsi
	movl	$0, (%rsi,%rax,4)
	movq	-8(%rbp), %rax
	movq	-16(%rcx), %rcx
	movq	%rcx, -16(%rbp)
	movq	-24(%rbp), %rsi
	movq	%rax, 8(%rsi)
	movq	%rcx, (%rsi)
	movl	-16(%rdx), %eax
.LBB2_3:                                # %if_1
	movq	%rbp, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end2:
	.size	vec_pop, .Lfunc_end2-vec_pop
	.cfi_endproc
                                        # -- End function
	.globl	vec_get                         # -- Begin function vec_get
	.p2align	4
	.type	vec_get,@function
vec_get:                                # @vec_get
	.cfi_startproc
# %bb.0:                                # %vec_get
	movl	%edx, -12(%rsp)
	movl	%esi, -16(%rsp)
	movq	%rdi, -24(%rsp)
	movl	%ecx, -28(%rsp)
	movq	%rdi, -8(%rsp)
	movslq	%ecx, %rax
	movl	(%rdi,%rax,4), %eax
	retq
.Lfunc_end3:
	.size	vec_get, .Lfunc_end3-vec_get
	.cfi_endproc
                                        # -- End function
	.globl	vec_front                       # -- Begin function vec_front
	.p2align	4
	.type	vec_front,@function
vec_front:                              # @vec_front
	.cfi_startproc
# %bb.0:                                # %vec_front
	movl	%edx, -12(%rsp)
	movl	%esi, -16(%rsp)
	movq	%rdi, -24(%rsp)
	movq	%rdi, -8(%rsp)
	movl	(%rdi), %eax
	retq
.Lfunc_end4:
	.size	vec_front, .Lfunc_end4-vec_front
	.cfi_endproc
                                        # -- End function
	.globl	vec_back                        # -- Begin function vec_back
	.p2align	4
	.type	vec_back,@function
vec_back:                               # @vec_back
	.cfi_startproc
# %bb.0:                                # %vec_back
	movl	%edx, -12(%rsp)
	movl	%esi, -16(%rsp)
	movq	%rdi, -24(%rsp)
	movq	%rdi, -8(%rsp)
	decl	%edx
	movslq	%edx, %rax
	movl	(%rdi,%rax,4), %eax
	retq
.Lfunc_end5:
	.size	vec_back, .Lfunc_end5-vec_back
	.cfi_endproc
                                        # -- End function
	.globl	vec_del                         # -- Begin function vec_del
	.p2align	4
	.type	vec_del,@function
vec_del:                                # @vec_del
	.cfi_startproc
# %bb.0:                                # %vec_del
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movq	%rdi, 16(%rsp)
	movl	$0, (%rsp)
	movl	8(%rdi), %ecx
	movq	(%rdi), %rax
	movl	12(%rdi), %edx
	movl	%edx, 12(%rsp)
	movq	%rax, (%rsp)
	movl	%ecx, 8(%rsp)
	movq	%rax, %rdi
	callq	free@PLT
	movq	16(%rsp), %rax
	movq	$0, (%rax)
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end6:
	.size	vec_del, .Lfunc_end6-vec_del
	.cfi_endproc
                                        # -- End function
	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"ERROR: POP BACK IN EMPTY VECTOR\n"
	.size	.L.str, 33

	.section	".note.GNU-stack","",@progbits
