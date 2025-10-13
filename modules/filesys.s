	.file	"MainModule"
	.text
	.globl	open$MODfile                    # -- Begin function open$MODfile
	.p2align	4
	.type	open$MODfile,@function
open$MODfile:                           # @"open$MODfile"
	.cfi_startproc
# %bb.0:                                # %"open$MODfile"
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
	callq	alloc$MODmem@PLT
	movq	%rax, -32(%rbp)
	testq	%rax, %rax
	jne	.LBB0_2
# %bb.6:                                # %if_1
	leaq	.L.str.1(%rip), %rdi
.LBB0_4:                                # %if_0
	xorl	%eax, %eax
	callq	print$MODstd@PLT
	movl	-40(%rbp), %eax
	movq	-32(%rbp), %rcx
	movq	-24(%rbp), %rdx
	movq	-16(%rbp), %rsi
	movq	%rsi, 24(%rbx)
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
	movl	$1, -12(%rbp)
	movq	-16(%rbp), %rdx
	movq	-24(%rbp), %rsi
	movl	%eax, (%rbx)
	movq	%rsi, 16(%rbx)
	movq	%rdx, 24(%rbx)
	movq	%rcx, 8(%rbx)
	jmp	.LBB0_5
.Lfunc_end0:
	.size	open$MODfile, .Lfunc_end0-open$MODfile
	.cfi_endproc
                                        # -- End function
	.globl	close$MODfile                   # -- Begin function close$MODfile
	.p2align	4
	.type	close$MODfile,@function
close$MODfile:                          # @"close$MODfile"
	.cfi_startproc
# %bb.0:                                # %"close$MODfile"
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	movq	%rdi, 32(%rsp)
	movl	$0, (%rsp)
	movl	24(%rdi), %eax
	movl	16(%rdi), %ecx
	movl	(%rdi), %edx
	movl	20(%rdi), %esi
	movl	28(%rdi), %r8d
	movq	8(%rdi), %rdi
	movq	%rdi, 8(%rsp)
	movl	%r8d, 28(%rsp)
	movl	%esi, 20(%rsp)
	movl	%edx, (%rsp)
	movl	%ecx, 16(%rsp)
	movl	%eax, 24(%rsp)
	testl	%edx, %edx
	js	.LBB1_1
# %bb.4:                                # %if_2
	movl	(%rsp), %edi
	callq	sys_close@PLT
.LBB1_1:                                # %endif_2
	cmpq	$0, 8(%rsp)
	je	.LBB1_3
# %bb.2:                                # %if_3
	movq	8(%rsp), %rdi
	callq	free$MODmem@PLT
.LBB1_3:                                # %endif_3
	movl	24(%rsp), %eax
	movl	(%rsp), %ecx
	movq	16(%rsp), %rdx
	movq	$0, 8(%rsp)
	movl	$0, 28(%rsp)
	movq	32(%rsp), %rsi
	movq	%rdx, 16(%rsi)
	movl	%ecx, (%rsi)
	movl	%eax, 24(%rsi)
	movq	$0, 8(%rsi)
	movl	$0, 28(%rsi)
	addq	$40, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	close$MODfile, .Lfunc_end1-close$MODfile
	.cfi_endproc
                                        # -- End function
	.globl	isopen$MODfile                  # -- Begin function isopen$MODfile
	.p2align	4
	.type	isopen$MODfile,@function
isopen$MODfile:                         # @"isopen$MODfile"
	.cfi_startproc
# %bb.0:                                # %"isopen$MODfile"
	movl	%r9d, -4(%rsp)
	movl	%r8d, -8(%rsp)
	movl	%ecx, -12(%rsp)
	movl	%edx, -16(%rsp)
	movq	%rsi, -24(%rsp)
	movl	%edi, -32(%rsp)
	testq	%rsi, %rsi
	jne	.LBB2_1
# %bb.2:                                # %if_4
	xorl	%eax, %eax
	retq
.LBB2_1:                                # %else_4
	movl	$1, %eax
	retq
.Lfunc_end2:
	.size	isopen$MODfile, .Lfunc_end2-isopen$MODfile
	.cfi_endproc
                                        # -- End function
	.globl	write$MODfile                   # -- Begin function write$MODfile
	.p2align	4
	.type	write$MODfile,@function
write$MODfile:                          # @"write$MODfile"
	.cfi_startproc
# %bb.0:                                # %"write$MODfile"
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -48(%rbp)
	movq	%rsi, -40(%rbp)
	movl	28(%rdi), %eax
	movl	24(%rdi), %ecx
	movl	20(%rdi), %edx
	movl	16(%rdi), %esi
	movq	8(%rdi), %r8
	movl	(%rdi), %edi
	movl	%edi, -32(%rbp)
	movq	%r8, -24(%rbp)
	movl	%esi, -16(%rbp)
	movl	%edx, -12(%rbp)
	movl	%ecx, -8(%rbp)
	movl	%eax, -4(%rbp)
	testl	%edi, %edi
	jns	.LBB3_1
# %bb.2:                                # %if_5
	leaq	.L.str.3(%rip), %rdi
	xorl	%eax, %eax
	callq	print$MODstd@PLT
	movl	$-1, %eax
	jmp	.LBB3_3
.LBB3_1:                                # %else_5
	movq	-40(%rbp), %rdi
	callq	len$MODstring@PLT
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rsp
	movl	%eax, -16(%rcx)
	movl	-32(%rbp), %edi
	movq	-40(%rbp), %rsi
	movl	%eax, %edx
	callq	sys_write@PLT
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rsp
	movl	%eax, -16(%rcx)
	movq	-8(%rbp), %rdx
	movl	-32(%rbp), %esi
	movl	-16(%rbp), %edi
	movq	-24(%rbp), %r8
	movl	%eax, -12(%rbp)
	movq	-48(%rbp), %r9
	movq	%r8, 8(%r9)
	movl	%edi, 16(%r9)
	movl	%esi, (%r9)
	movl	%eax, 20(%r9)
	movq	%rdx, 24(%r9)
	movl	-16(%rcx), %eax
.LBB3_3:                                # %if_5
	movq	%rbp, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end3:
	.size	write$MODfile, .Lfunc_end3-write$MODfile
	.cfi_endproc
                                        # -- End function
	.globl	read$MODfile                    # -- Begin function read$MODfile
	.p2align	4
	.type	read$MODfile,@function
read$MODfile:                           # @"read$MODfile"
	.cfi_startproc
# %bb.0:                                # %"read$MODfile"
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%rbx
	subq	$40, %rsp
	.cfi_offset %rbx, -24
	movq	%rdi, -48(%rbp)
	movl	28(%rdi), %eax
	movl	24(%rdi), %ecx
	movl	20(%rdi), %edx
	movl	16(%rdi), %esi
	movq	8(%rdi), %r8
	movl	(%rdi), %edi
	movl	%edi, -40(%rbp)
	movq	%r8, -32(%rbp)
	movl	%esi, -24(%rbp)
	movl	%edx, -20(%rbp)
	movl	%ecx, -16(%rbp)
	movl	%eax, -12(%rbp)
	testl	%edi, %edi
	jns	.LBB4_1
# %bb.6:                                # %if_6
	leaq	.L.str.4(%rip), %rdi
	xorl	%eax, %eax
	callq	print$MODstd@PLT
	leaq	.L.str.5(%rip), %rax
	jmp	.LBB4_5
.LBB4_1:                                # %else_6
	cmpq	$0, -32(%rbp)
	jne	.LBB4_2
# %bb.7:                                # %if_7
	leaq	.L.str.6(%rip), %rdi
	xorl	%eax, %eax
	callq	print$MODstd@PLT
	leaq	.L.str.7(%rip), %rax
	jmp	.LBB4_5
.LBB4_2:                                # %else_7
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
	jg	.LBB4_3
# %bb.4:                                # %if_8
	leaq	.L.str.8(%rip), %rax
	jmp	.LBB4_5
.LBB4_3:                                # %else_8
	movq	(%rbx), %rdi
	movl	(%rcx), %esi
	callq	bytostr@PLT
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rsp
	movq	%rax, -16(%rcx)
	movq	-48(%rbp), %rax
	movl	-12(%rbp), %edx
	movl	-16(%rbp), %esi
	movl	-20(%rbp), %edi
	movl	-24(%rbp), %r8d
	movq	-32(%rbp), %r9
	movl	-40(%rbp), %r10d
	movl	%r10d, (%rax)
	movq	%r9, 8(%rax)
	movl	%r8d, 16(%rax)
	movl	%edi, 20(%rax)
	movl	%esi, 24(%rax)
	movl	%edx, 28(%rax)
	movq	-16(%rcx), %rax
.LBB4_5:                                # %if_8
	leaq	-8(%rbp), %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end4:
	.size	read$MODfile, .Lfunc_end4-read$MODfile
	.cfi_endproc
                                        # -- End function
	.globl	seek$MODfile                    # -- Begin function seek$MODfile
	.p2align	4
	.type	seek$MODfile,@function
seek$MODfile:                           # @"seek$MODfile"
	.cfi_startproc
# %bb.0:                                # %"seek$MODfile"
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -48(%rbp)
	movl	%esi, -8(%rbp)
	movl	%edx, -4(%rbp)
	movl	28(%rdi), %eax
	movl	24(%rdi), %ecx
	movl	20(%rdi), %edx
	movl	16(%rdi), %esi
	movq	8(%rdi), %r8
	movl	(%rdi), %edi
	movl	%edi, -40(%rbp)
	movq	%r8, -32(%rbp)
	movl	%esi, -24(%rbp)
	movl	%edx, -20(%rbp)
	movl	%ecx, -16(%rbp)
	movl	%eax, -12(%rbp)
	testl	%edi, %edi
	jns	.LBB5_1
# %bb.3:                                # %if_9
	leaq	.L.str.9(%rip), %rdi
	jmp	.LBB5_4
.LBB5_1:                                # %else_9
	movl	-40(%rbp), %edi
	movl	-8(%rbp), %esi
	movl	-4(%rbp), %edx
	callq	sys_seek@PLT
	movq	%rsp, %rdx
	leaq	-16(%rdx), %rcx
	movq	%rcx, %rsp
	movl	%eax, -16(%rdx)
	testl	%eax, %eax
	jns	.LBB5_2
# %bb.6:                                # %if_10
	leaq	.L.str.10(%rip), %rdi
.LBB5_4:                                # %if_9
	xorl	%eax, %eax
	callq	print$MODstd@PLT
.LBB5_5:                                # %if_9
	movq	%rbp, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.LBB5_2:                                # %else_10
	.cfi_def_cfa %rbp, 16
	movq	-16(%rbp), %rax
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
	movq	%rax, 24(%r8)
	jmp	.LBB5_5
.Lfunc_end5:
	.size	seek$MODfile, .Lfunc_end5-seek$MODfile
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
