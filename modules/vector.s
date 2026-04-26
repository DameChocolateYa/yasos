	.file	"MainModule"
	.text
	.globl	new$MODVec                      # -- Begin function new$MODVec
	.p2align	4
	.type	new$MODVec,@function
new$MODVec:                             # @"new$MODVec"
	.cfi_startproc
# %bb.0:                                # %"new$MODVec"
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$48, %rsp
	.cfi_def_cfa_offset 64
	.cfi_offset %rbx, -16
                                        # kill: def $esi killed $esi def $rsi
	movq	%rdi, %rbx
	movl	%esi, 12(%rsp)
	movl	$0, 16(%rsp)
	leal	(%rsi,%rsi), %edi
	callq	alloc$MODmem@PLT
	movq	%rax, 16(%rsp)
	movabsq	$8589934592, %rcx               # imm = 0x200000000
	movq	%rcx, 24(%rsp)
	movl	12(%rsp), %edx
	movl	%edx, 32(%rsp)
	movq	%rax, (%rbx)
	movl	%edx, 16(%rbx)
	movq	%rcx, 8(%rbx)
	movq	%rbx, %rax
	addq	$48, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	new$MODVec, .Lfunc_end0-new$MODVec
	.cfi_endproc
                                        # -- End function
	.globl	push$MODVec                     # -- Begin function push$MODVec
	.p2align	4
	.type	push$MODVec,@function
push$MODVec:                            # @"push$MODVec"
	.cfi_startproc
# %bb.0:                                # %"push$MODVec"
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%rbx
	subq	$56, %rsp
	.cfi_offset %rbx, -24
	movq	%rdi, -48(%rbp)
	movq	%rsi, -40(%rbp)
	movl	16(%rdi), %eax
	movl	12(%rdi), %ecx
	movl	8(%rdi), %edx
	movq	(%rdi), %rsi
	movq	%rsi, -32(%rbp)
	movl	%edx, -24(%rbp)
	movl	%ecx, -20(%rbp)
	movl	%eax, -16(%rbp)
	incl	%edx
	cmpl	%ecx, %edx
	jl	.LBB1_2
# %bb.1:                                # %if_0
	movq	-32(%rbp), %rdi
	movl	-20(%rbp), %esi
	addl	%esi, %esi
	movl	%esi, -20(%rbp)
	imull	-16(%rbp), %esi
	callq	realloc$MODmem@PLT
	movq	%rax, -32(%rbp)
.LBB1_2:                                # %endif_0
	movq	-32(%rbp), %rax
	movq	%rsp, %rbx
	leaq	-16(%rbx), %rsp
	movq	%rax, -16(%rbx)
	movl	-16(%rbp), %edx
	movl	-24(%rbp), %ecx
	imull	%edx, %ecx
	movslq	%ecx, %rdi
	addq	%rax, %rdi
	movq	-40(%rbp), %rsi
	callq	memcpy@PLT
	movl	-16(%rbp), %eax
	movl	-24(%rbp), %ecx
	movl	-20(%rbp), %edx
	movq	-16(%rbx), %rsi
	movq	%rsi, -32(%rbp)
	incl	%ecx
	movl	%ecx, -24(%rbp)
	movq	-48(%rbp), %rdi
	movl	%edx, 12(%rdi)
	movl	%ecx, 8(%rdi)
	movl	%eax, 16(%rdi)
	movq	%rsi, (%rdi)
	leaq	-8(%rbp), %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end1:
	.size	push$MODVec, .Lfunc_end1-push$MODVec
	.cfi_endproc
                                        # -- End function
	.globl	push_int$MODVec                 # -- Begin function push_int$MODVec
	.p2align	4
	.type	push_int$MODVec,@function
push_int$MODVec:                        # @"push_int$MODVec"
	.cfi_startproc
# %bb.0:                                # %"push_int$MODVec"
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movq	%rdi, 8(%rsp)
	movl	%esi, 4(%rsp)
	leaq	4(%rsp), %rsi
	callq	push$MODVec@PLT
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end2:
	.size	push_int$MODVec, .Lfunc_end2-push_int$MODVec
	.cfi_endproc
                                        # -- End function
	.globl	push_string$MODVec              # -- Begin function push_string$MODVec
	.p2align	4
	.type	push_string$MODVec,@function
push_string$MODVec:                     # @"push_string$MODVec"
	.cfi_startproc
# %bb.0:                                # %"push_string$MODVec"
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	movq	%rdi, 24(%rsp)
	movq	%rsi, 8(%rsp)
	movl	%edx, 16(%rsp)
	leaq	8(%rsp), %rsi
	callq	push$MODVec@PLT
	addq	$40, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end3:
	.size	push_string$MODVec, .Lfunc_end3-push_string$MODVec
	.cfi_endproc
                                        # -- End function
	.globl	delete$MODVec                   # -- Begin function delete$MODVec
	.p2align	4
	.type	delete$MODVec,@function
delete$MODVec:                          # @"delete$MODVec"
	.cfi_startproc
# %bb.0:                                # %"delete$MODVec"
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	movq	%rdi, 24(%rsp)
	movl	%esi, 32(%rsp)
	movl	16(%rdi), %eax
	movl	12(%rdi), %ecx
	movl	8(%rdi), %edx
	movq	(%rdi), %rdi
	movq	%rdi, (%rsp)
	movl	%edx, 8(%rsp)
	movl	%ecx, 12(%rsp)
	movl	%eax, 16(%rsp)
	callq	free$MODmem@PLT
	movq	(%rsp), %rax
	movl	$0, 8(%rsp)
	movq	$0, 12(%rsp)
	movq	24(%rsp), %rcx
	movq	%rax, (%rcx)
	movl	$0, 12(%rcx)
	movl	$0, 8(%rcx)
	movl	$0, 16(%rcx)
	addq	$40, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end4:
	.size	delete$MODVec, .Lfunc_end4-delete$MODVec
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
