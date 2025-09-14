	.file	"MainModule"
	.text
	.globl	file_open                       # -- Begin function file_open
	.p2align	4
	.type	file_open,@function
file_open:                              # @file_open
	.cfi_startproc
# %bb.0:                                # %file_open
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%rbx
	subq	$56, %rsp
	.cfi_offset %rbx, -24
	movq	%rdi, %rbx
	movq	%rsi, -56(%rbp)
	movl	%edx, -44(%rbp)
	movl	$0, -40(%rbp)
	movq	%rsi, %rdi
	movl	%edx, %esi
	movl	$420, %edx                      # imm = 0x1A4
	callq	sys_open@PLT
	movl	%eax, -40(%rbp)
	testl	%eax, %eax
	jns	.LBB0_1
# %bb.3:                                # %if_0
	leaq	.L.str(%rip), %rdi
	jmp	.LBB0_4
.LBB0_1:                                # %else_0
	movl	$4096, -24(%rbp)                # imm = 0x1000
	movl	$4096, %edi                     # imm = 0x1000
	callq	malloc@PLT
	movq	%rax, -32(%rbp)
	testq	%rax, %rax
	jne	.LBB0_2
# %bb.6:                                # %if_1
	leaq	.L.str.1(%rip), %rdi
.LBB0_4:                                # %if_0
	xorl	%eax, %eax
	callq	print@PLT
	movl	-40(%rbp), %eax
	movq	-32(%rbp), %rcx
	movq	-24(%rbp), %rdx
	movl	-16(%rbp), %esi
	movl	%esi, 24(%rbx)
	movq	%rdx, 16(%rbx)
	movq	%rcx, 8(%rbx)
	movl	%eax, (%rbx)
.LBB0_5:                                # %if_0
	movq	%rbx, %rax
	leaq	-8(%rbp), %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.LBB0_2:                                # %else_1
	.cfi_def_cfa %rbp, 16
	movq	-32(%rbp), %rax
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rsp
	movq	%rax, -16(%rcx)
	leaq	.L.str.2(%rip), %rdx
	movq	%rdx, (%rax)
	movl	-40(%rbp), %eax
	movq	-16(%rcx), %rcx
	movq	%rcx, -32(%rbp)
	movl	$0, -20(%rbp)
	movl	-44(%rbp), %edx
	movl	%edx, -16(%rbp)
	movq	-24(%rbp), %rsi
	movl	%eax, (%rbx)
	movq	%rsi, 16(%rbx)
	movl	%edx, 24(%rbx)
	movq	%rcx, 8(%rbx)
	jmp	.LBB0_5
.Lfunc_end0:
	.size	file_open, .Lfunc_end0-file_open
	.cfi_endproc
                                        # -- End function
	.globl	file_close                      # -- Begin function file_close
	.p2align	4
	.type	file_close,@function
file_close:                             # @file_close
	.cfi_startproc
# %bb.0:                                # %file_close
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	movq	%rdi, 32(%rsp)
	movl	$0, (%rsp)
	movl	24(%rdi), %eax
	movl	16(%rdi), %ecx
	movq	8(%rdi), %rdx
	movl	(%rdi), %esi
	movl	20(%rdi), %edi
	movl	%edi, 20(%rsp)
	movl	%esi, (%rsp)
	movq	%rdx, 8(%rsp)
	movl	%ecx, 16(%rsp)
	movl	%eax, 24(%rsp)
	testl	%esi, %esi
	js	.LBB1_1
# %bb.4:                                # %if_2
	movl	(%rsp), %edi
	callq	sys_close@PLT
.LBB1_1:                                # %endif_2
	cmpq	$0, 8(%rsp)
	je	.LBB1_3
# %bb.2:                                # %if_3
	movq	8(%rsp), %rdi
	callq	free@PLT
.LBB1_3:                                # %endif_3
	movq	32(%rsp), %rax
	movq	$0, (%rax)
	addq	$40, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	file_close, .Lfunc_end1-file_close
	.cfi_endproc
                                        # -- End function
	.globl	file_write                      # -- Begin function file_write
	.p2align	4
	.type	file_write,@function
file_write:                             # @file_write
	.cfi_startproc
# %bb.0:                                # %file_write
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -48(%rbp)
	movq	%rsi, -8(%rbp)
	movl	24(%rdi), %eax
	movl	20(%rdi), %ecx
	movl	16(%rdi), %edx
	movq	8(%rdi), %rsi
	movl	(%rdi), %edi
	movl	%edi, -40(%rbp)
	movq	%rsi, -32(%rbp)
	movl	%edx, -24(%rbp)
	movl	%ecx, -20(%rbp)
	movl	%eax, -16(%rbp)
	testl	%edi, %edi
	jns	.LBB2_1
# %bb.2:                                # %if_4
	leaq	.L.str.3(%rip), %rdi
	xorl	%eax, %eax
	callq	print@PLT
	movl	$-1, %eax
	jmp	.LBB2_3
.LBB2_1:                                # %else_4
	movq	-8(%rbp), %rdi
	callq	len@PLT
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rsp
	movl	%eax, -16(%rcx)
	movl	-40(%rbp), %edi
	movq	-8(%rbp), %rsi
	movl	%eax, %edx
	callq	sys_write@PLT
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rsp
	movl	%eax, -16(%rcx)
	movl	-40(%rbp), %edx
	movl	-16(%rbp), %esi
	movl	-24(%rbp), %edi
	movq	-32(%rbp), %r8
	movl	%eax, -20(%rbp)
	movq	-48(%rbp), %r9
	movq	%r8, 8(%r9)
	movl	%edi, 16(%r9)
	movl	%esi, 24(%r9)
	movl	%edx, (%r9)
	movl	%eax, 20(%r9)
	movl	-16(%rcx), %eax
.LBB2_3:                                # %if_4
	movq	%rbp, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end2:
	.size	file_write, .Lfunc_end2-file_write
	.cfi_endproc
                                        # -- End function
	.globl	file_read                       # -- Begin function file_read
	.p2align	4
	.type	file_read,@function
file_read:                              # @file_read
	.cfi_startproc
# %bb.0:                                # %file_read
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%rbx
	subq	$40, %rsp
	.cfi_offset %rbx, -24
	movq	%rdi, -48(%rbp)
	movl	24(%rdi), %eax
	movl	20(%rdi), %ecx
	movl	16(%rdi), %edx
	movq	8(%rdi), %rsi
	movl	(%rdi), %edi
	movl	%edi, -40(%rbp)
	movq	%rsi, -32(%rbp)
	movl	%edx, -24(%rbp)
	movl	%ecx, -20(%rbp)
	movl	%eax, -16(%rbp)
	testl	%edi, %edi
	jns	.LBB3_1
# %bb.6:                                # %if_5
	leaq	.L.str.4(%rip), %rdi
	xorl	%eax, %eax
	callq	print@PLT
	leaq	.L.str.5(%rip), %rax
	jmp	.LBB3_5
.LBB3_1:                                # %else_5
	cmpq	$0, -32(%rbp)
	jne	.LBB3_2
# %bb.7:                                # %if_6
	leaq	.L.str.6(%rip), %rdi
	xorl	%eax, %eax
	callq	print@PLT
	leaq	.L.str.7(%rip), %rax
	jmp	.LBB3_5
.LBB3_2:                                # %else_6
	movq	-32(%rbp), %rsi
	movq	%rsp, %rax
	leaq	-16(%rax), %rbx
	movq	%rbx, %rsp
	movq	%rsi, -16(%rax)
	movl	-40(%rbp), %edi
	movl	-24(%rbp), %edx
	callq	sys_read@PLT
	movq	%rsp, %rdx
	leaq	-16(%rdx), %rcx
	movq	%rcx, %rsp
	movl	%eax, -16(%rdx)
	testl	%eax, %eax
	jg	.LBB3_3
# %bb.4:                                # %if_7
	leaq	.L.str.8(%rip), %rax
	jmp	.LBB3_5
.LBB3_3:                                # %else_7
	movq	(%rbx), %rdi
	movl	(%rcx), %esi
	decl	%esi
	callq	bytostr@PLT
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rsp
	movq	%rax, -16(%rcx)
	movq	-48(%rbp), %rax
	movl	-16(%rbp), %edx
	movl	-20(%rbp), %esi
	movl	-24(%rbp), %edi
	movq	-32(%rbp), %r8
	movl	-40(%rbp), %r9d
	movl	%r9d, (%rax)
	movq	%r8, 8(%rax)
	movl	%edi, 16(%rax)
	movl	%esi, 20(%rax)
	movl	%edx, 24(%rax)
	movq	-16(%rcx), %rax
.LBB3_5:                                # %if_7
	leaq	-8(%rbp), %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end3:
	.size	file_read, .Lfunc_end3-file_read
	.cfi_endproc
                                        # -- End function
	.globl	file_seek                       # -- Begin function file_seek
	.p2align	4
	.type	file_seek,@function
file_seek:                              # @file_seek
	.cfi_startproc
# %bb.0:                                # %file_seek
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -48(%rbp)
	movl	%esi, -8(%rbp)
	movl	%edx, -4(%rbp)
	movl	24(%rdi), %eax
	movl	20(%rdi), %ecx
	movl	16(%rdi), %edx
	movq	8(%rdi), %rsi
	movl	(%rdi), %edi
	movl	%edi, -40(%rbp)
	movq	%rsi, -32(%rbp)
	movl	%edx, -24(%rbp)
	movl	%ecx, -20(%rbp)
	movl	%eax, -16(%rbp)
	testl	%edi, %edi
	jns	.LBB4_1
# %bb.3:                                # %if_8
	leaq	.L.str.9(%rip), %rdi
	jmp	.LBB4_4
.LBB4_1:                                # %else_8
	movl	-40(%rbp), %edi
	movl	-8(%rbp), %esi
	movl	-4(%rbp), %edx
	callq	sys_seek@PLT
	movq	%rsp, %rdx
	leaq	-16(%rdx), %rcx
	movq	%rcx, %rsp
	movl	%eax, -16(%rdx)
	testl	%eax, %eax
	jns	.LBB4_2
# %bb.6:                                # %if_9
	leaq	.L.str.10(%rip), %rdi
.LBB4_4:                                # %if_8
	xorl	%eax, %eax
	callq	print@PLT
.LBB4_5:                                # %if_8
	movq	%rbp, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.LBB4_2:                                # %else_9
	.cfi_def_cfa %rbp, 16
	movl	-16(%rbp), %eax
	movl	-24(%rbp), %edx
	movq	-32(%rbp), %rsi
	movl	-40(%rbp), %edi
	movl	(%rcx), %ecx
	movl	%ecx, -20(%rbp)
	movq	-48(%rbp), %r8
	movl	%ecx, 20(%r8)
	movl	%edi, (%r8)
	movq	%rsi, 8(%r8)
	movl	%edx, 16(%r8)
	movl	%eax, 24(%r8)
	jmp	.LBB4_5
.Lfunc_end4:
	.size	file_seek, .Lfunc_end4-file_seek
	.cfi_endproc
                                        # -- End function
	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"ERROR: could not open file\n"
	.size	.L.str, 28

	.type	.L.str.1,@object                # @.str.1
.L.str.1:
	.asciz	"ERROR: could not allocate file buffer\n"
	.size	.L.str.1, 39

	.type	.L.str.2,@object                # @.str.2
.L.str.2:
	.zero	1
	.size	.L.str.2, 1

	.type	.L.str.3,@object                # @.str.3
.L.str.3:
	.asciz	"ERROR: file not open\n"
	.size	.L.str.3, 22

	.type	.L.str.4,@object                # @.str.4
.L.str.4:
	.asciz	"ERROR: file not open\n"
	.size	.L.str.4, 22

	.type	.L.str.5,@object                # @.str.5
.L.str.5:
	.asciz	"(null)"
	.size	.L.str.5, 7

	.type	.L.str.6,@object                # @.str.6
.L.str.6:
	.asciz	"ERROR: file with a NULL buffer\n"
	.size	.L.str.6, 32

	.type	.L.str.7,@object                # @.str.7
.L.str.7:
	.asciz	"(null)"
	.size	.L.str.7, 7

	.type	.L.str.8,@object                # @.str.8
.L.str.8:
	.zero	1
	.size	.L.str.8, 1

	.type	.L.str.9,@object                # @.str.9
.L.str.9:
	.asciz	"ERROR: file not open\n"
	.size	.L.str.9, 22

	.type	.L.str.10,@object               # @.str.10
.L.str.10:
	.asciz	"ERROR: seek failed"
	.size	.L.str.10, 19

	.section	".note.GNU-stack","",@progbits
