	.file	"MainModule"
	.text
	.globl	str_upper                       # -- Begin function str_upper
	.p2align	4
	.type	str_upper,@function
str_upper:                              # @str_upper
	.cfi_startproc
# %bb.0:                                # %str_upper
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	subq	$40, %rsp
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	movq	%rdi, -72(%rbp)
	callq	len@PLT
	movl	%eax, -60(%rbp)
	leaq	.Lstr(%rip), %rax
	movq	%rax, -56(%rbp)
	movl	$0, -44(%rbp)
	leaq	.Lstr.1(%rip), %rbx
	leaq	.Lstr.2(%rip), %r14
	leaq	.Lstr.3(%rip), %r15
	leaq	.Lstr.4(%rip), %r12
	jmp	.LBB0_1
	.p2align	4
.LBB0_3:                                # %if_0
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	movq	%r14, %rsi
.LBB0_4:                                # %for_start_1
                                        #   in Loop: Header=BB0_1 Depth=1
	callq	strcat@PLT
	movq	%rax, -56(%rbp)
	incl	-44(%rbp)
.LBB0_1:                                # %for_start_1
                                        # =>This Inner Loop Header: Depth=1
	movl	-44(%rbp), %eax
	cmpl	-60(%rbp), %eax
	jge	.LBB0_5
# %bb.2:                                # %for_body_1
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-72(%rbp), %rdi
	movl	-44(%rbp), %esi
	leal	1(%rsi), %edx
                                        # kill: def $esi killed $esi killed $rsi
	callq	strsub@PLT
	movq	%rsp, %rcx
	leaq	-16(%rcx), %r13
	movq	%r13, %rsp
	movq	%rax, -16(%rcx)
	movq	%rax, %rdi
	movq	%rbx, %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	jne	.LBB0_3
# %bb.6:                                # %elif_0_0
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	movq	%r15, %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_7
# %bb.33:                               # %elif_body_0_0
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	movq	%r12, %rsi
	jmp	.LBB0_4
.LBB0_7:                                # %elif_0_1
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.5(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_8
# %bb.34:                               # %elif_body_0_1
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.6(%rip), %rsi
	jmp	.LBB0_4
.LBB0_8:                                # %elif_0_2
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.7(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_9
# %bb.35:                               # %elif_body_0_2
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.8(%rip), %rsi
	jmp	.LBB0_4
.LBB0_9:                                # %elif_0_3
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.9(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_10
# %bb.36:                               # %elif_body_0_3
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.10(%rip), %rsi
	jmp	.LBB0_4
.LBB0_10:                               # %elif_0_4
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.11(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_11
# %bb.37:                               # %elif_body_0_4
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.12(%rip), %rsi
	jmp	.LBB0_4
.LBB0_11:                               # %elif_0_5
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.13(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_12
# %bb.38:                               # %elif_body_0_5
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.14(%rip), %rsi
	jmp	.LBB0_4
.LBB0_12:                               # %elif_0_6
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.15(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_13
# %bb.39:                               # %elif_body_0_6
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.16(%rip), %rsi
	jmp	.LBB0_4
.LBB0_13:                               # %elif_0_7
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.17(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_14
# %bb.40:                               # %elif_body_0_7
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.18(%rip), %rsi
	jmp	.LBB0_4
.LBB0_14:                               # %elif_0_8
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.19(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_15
# %bb.41:                               # %elif_body_0_8
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.20(%rip), %rsi
	jmp	.LBB0_4
.LBB0_15:                               # %elif_0_9
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.21(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_16
# %bb.42:                               # %elif_body_0_9
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.22(%rip), %rsi
	jmp	.LBB0_4
.LBB0_16:                               # %elif_0_10
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.23(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_17
# %bb.43:                               # %elif_body_0_10
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.24(%rip), %rsi
	jmp	.LBB0_4
.LBB0_17:                               # %elif_0_11
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.25(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_18
# %bb.44:                               # %elif_body_0_11
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.26(%rip), %rsi
	jmp	.LBB0_4
.LBB0_18:                               # %elif_0_12
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.27(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_19
# %bb.45:                               # %elif_body_0_12
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.28(%rip), %rsi
	jmp	.LBB0_4
.LBB0_19:                               # %elif_0_13
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.29(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_20
# %bb.46:                               # %elif_body_0_13
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.30(%rip), %rsi
	jmp	.LBB0_4
.LBB0_20:                               # %elif_0_14
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.31(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_21
# %bb.47:                               # %elif_body_0_14
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.32(%rip), %rsi
	jmp	.LBB0_4
.LBB0_21:                               # %elif_0_15
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.33(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_22
# %bb.48:                               # %elif_body_0_15
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.34(%rip), %rsi
	jmp	.LBB0_4
.LBB0_22:                               # %elif_0_16
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.35(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_23
# %bb.49:                               # %elif_body_0_16
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.36(%rip), %rsi
	jmp	.LBB0_4
.LBB0_23:                               # %elif_0_17
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.37(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_24
# %bb.50:                               # %elif_body_0_17
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.38(%rip), %rsi
	jmp	.LBB0_4
.LBB0_24:                               # %elif_0_18
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.39(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_25
# %bb.51:                               # %elif_body_0_18
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.40(%rip), %rsi
	jmp	.LBB0_4
.LBB0_25:                               # %elif_0_19
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.41(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_26
# %bb.52:                               # %elif_body_0_19
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.42(%rip), %rsi
	jmp	.LBB0_4
.LBB0_26:                               # %elif_0_20
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.43(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_27
# %bb.53:                               # %elif_body_0_20
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.44(%rip), %rsi
	jmp	.LBB0_4
.LBB0_27:                               # %elif_0_21
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.45(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_28
# %bb.54:                               # %elif_body_0_21
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.46(%rip), %rsi
	jmp	.LBB0_4
.LBB0_28:                               # %elif_0_22
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.47(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_29
# %bb.55:                               # %elif_body_0_22
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.48(%rip), %rsi
	jmp	.LBB0_4
.LBB0_29:                               # %elif_0_23
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.49(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_30
# %bb.56:                               # %elif_body_0_23
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.50(%rip), %rsi
	jmp	.LBB0_4
.LBB0_30:                               # %elif_0_24
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.51(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_31
# %bb.57:                               # %elif_body_0_24
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.52(%rip), %rsi
	jmp	.LBB0_4
.LBB0_31:                               # %elif_0_25
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.53(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_32
# %bb.58:                               # %elif_body_0_25
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.54(%rip), %rsi
	jmp	.LBB0_4
.LBB0_32:                               # %else_0
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	movq	(%r13), %rsi
	jmp	.LBB0_4
.LBB0_5:                                # %for_end_1
	movq	-56(%rbp), %rax
	leaq	-40(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end0:
	.size	str_upper, .Lfunc_end0-str_upper
	.cfi_endproc
                                        # -- End function
	.globl	str_lower                       # -- Begin function str_lower
	.p2align	4
	.type	str_lower,@function
str_lower:                              # @str_lower
	.cfi_startproc
# %bb.0:                                # %str_lower
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	subq	$40, %rsp
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	movq	%rdi, -72(%rbp)
	callq	len@PLT
	movl	%eax, -60(%rbp)
	leaq	.Lstr.55(%rip), %rax
	movq	%rax, -56(%rbp)
	movl	$0, -44(%rbp)
	leaq	.Lstr.56(%rip), %rbx
	leaq	.Lstr.57(%rip), %r14
	leaq	.Lstr.58(%rip), %r15
	leaq	.Lstr.59(%rip), %r12
	jmp	.LBB1_1
	.p2align	4
.LBB1_3:                                # %if_1
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	movq	%r14, %rsi
.LBB1_4:                                # %for_start_2
                                        #   in Loop: Header=BB1_1 Depth=1
	callq	strcat@PLT
	movq	%rax, -56(%rbp)
	incl	-44(%rbp)
.LBB1_1:                                # %for_start_2
                                        # =>This Inner Loop Header: Depth=1
	movl	-44(%rbp), %eax
	cmpl	-60(%rbp), %eax
	jge	.LBB1_5
# %bb.2:                                # %for_body_2
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-72(%rbp), %rdi
	movl	-44(%rbp), %esi
	leal	1(%rsi), %edx
                                        # kill: def $esi killed $esi killed $rsi
	callq	strsub@PLT
	movq	%rsp, %rcx
	leaq	-16(%rcx), %r13
	movq	%r13, %rsp
	movq	%rax, -16(%rcx)
	movq	%rax, %rdi
	movq	%rbx, %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	jne	.LBB1_3
# %bb.6:                                # %elif_1_0
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	movq	%r15, %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_7
# %bb.33:                               # %elif_body_1_0
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	movq	%r12, %rsi
	jmp	.LBB1_4
.LBB1_7:                                # %elif_1_1
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.60(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_8
# %bb.34:                               # %elif_body_1_1
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.61(%rip), %rsi
	jmp	.LBB1_4
.LBB1_8:                                # %elif_1_2
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.62(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_9
# %bb.35:                               # %elif_body_1_2
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.63(%rip), %rsi
	jmp	.LBB1_4
.LBB1_9:                                # %elif_1_3
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.64(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_10
# %bb.36:                               # %elif_body_1_3
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.65(%rip), %rsi
	jmp	.LBB1_4
.LBB1_10:                               # %elif_1_4
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.66(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_11
# %bb.37:                               # %elif_body_1_4
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.67(%rip), %rsi
	jmp	.LBB1_4
.LBB1_11:                               # %elif_1_5
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.68(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_12
# %bb.38:                               # %elif_body_1_5
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.69(%rip), %rsi
	jmp	.LBB1_4
.LBB1_12:                               # %elif_1_6
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.70(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_13
# %bb.39:                               # %elif_body_1_6
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.71(%rip), %rsi
	jmp	.LBB1_4
.LBB1_13:                               # %elif_1_7
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.72(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_14
# %bb.40:                               # %elif_body_1_7
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.73(%rip), %rsi
	jmp	.LBB1_4
.LBB1_14:                               # %elif_1_8
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.74(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_15
# %bb.41:                               # %elif_body_1_8
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.75(%rip), %rsi
	jmp	.LBB1_4
.LBB1_15:                               # %elif_1_9
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.76(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_16
# %bb.42:                               # %elif_body_1_9
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.77(%rip), %rsi
	jmp	.LBB1_4
.LBB1_16:                               # %elif_1_10
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.78(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_17
# %bb.43:                               # %elif_body_1_10
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.79(%rip), %rsi
	jmp	.LBB1_4
.LBB1_17:                               # %elif_1_11
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.80(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_18
# %bb.44:                               # %elif_body_1_11
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.81(%rip), %rsi
	jmp	.LBB1_4
.LBB1_18:                               # %elif_1_12
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.82(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_19
# %bb.45:                               # %elif_body_1_12
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.83(%rip), %rsi
	jmp	.LBB1_4
.LBB1_19:                               # %elif_1_13
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.84(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_20
# %bb.46:                               # %elif_body_1_13
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.85(%rip), %rsi
	jmp	.LBB1_4
.LBB1_20:                               # %elif_1_14
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.86(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_21
# %bb.47:                               # %elif_body_1_14
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.87(%rip), %rsi
	jmp	.LBB1_4
.LBB1_21:                               # %elif_1_15
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.88(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_22
# %bb.48:                               # %elif_body_1_15
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.89(%rip), %rsi
	jmp	.LBB1_4
.LBB1_22:                               # %elif_1_16
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.90(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_23
# %bb.49:                               # %elif_body_1_16
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.91(%rip), %rsi
	jmp	.LBB1_4
.LBB1_23:                               # %elif_1_17
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.92(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_24
# %bb.50:                               # %elif_body_1_17
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.93(%rip), %rsi
	jmp	.LBB1_4
.LBB1_24:                               # %elif_1_18
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.94(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_25
# %bb.51:                               # %elif_body_1_18
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.95(%rip), %rsi
	jmp	.LBB1_4
.LBB1_25:                               # %elif_1_19
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.96(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_26
# %bb.52:                               # %elif_body_1_19
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.97(%rip), %rsi
	jmp	.LBB1_4
.LBB1_26:                               # %elif_1_20
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.98(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_27
# %bb.53:                               # %elif_body_1_20
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.99(%rip), %rsi
	jmp	.LBB1_4
.LBB1_27:                               # %elif_1_21
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.100(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_28
# %bb.54:                               # %elif_body_1_21
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.101(%rip), %rsi
	jmp	.LBB1_4
.LBB1_28:                               # %elif_1_22
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.102(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_29
# %bb.55:                               # %elif_body_1_22
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.103(%rip), %rsi
	jmp	.LBB1_4
.LBB1_29:                               # %elif_1_23
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.104(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_30
# %bb.56:                               # %elif_body_1_23
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.105(%rip), %rsi
	jmp	.LBB1_4
.LBB1_30:                               # %elif_1_24
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.106(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_31
# %bb.57:                               # %elif_body_1_24
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.107(%rip), %rsi
	jmp	.LBB1_4
.LBB1_31:                               # %elif_1_25
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.Lstr.108(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_32
# %bb.58:                               # %elif_body_1_25
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.Lstr.109(%rip), %rsi
	jmp	.LBB1_4
.LBB1_32:                               # %else_1
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	movq	(%r13), %rsi
	jmp	.LBB1_4
.LBB1_5:                                # %for_end_2
	movq	-56(%rbp), %rax
	leaq	-40(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end1:
	.size	str_lower, .Lfunc_end1-str_lower
	.cfi_endproc
                                        # -- End function
	.type	.Lstr,@object                   # @str
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lstr:
	.zero	1
	.size	.Lstr, 1

	.type	.Lstr.1,@object                 # @str.1
.Lstr.1:
	.asciz	"a"
	.size	.Lstr.1, 2

	.type	.Lstr.2,@object                 # @str.2
.Lstr.2:
	.asciz	"A"
	.size	.Lstr.2, 2

	.type	.Lstr.3,@object                 # @str.3
.Lstr.3:
	.asciz	"b"
	.size	.Lstr.3, 2

	.type	.Lstr.4,@object                 # @str.4
.Lstr.4:
	.asciz	"B"
	.size	.Lstr.4, 2

	.type	.Lstr.5,@object                 # @str.5
.Lstr.5:
	.asciz	"c"
	.size	.Lstr.5, 2

	.type	.Lstr.6,@object                 # @str.6
.Lstr.6:
	.asciz	"C"
	.size	.Lstr.6, 2

	.type	.Lstr.7,@object                 # @str.7
.Lstr.7:
	.asciz	"d"
	.size	.Lstr.7, 2

	.type	.Lstr.8,@object                 # @str.8
.Lstr.8:
	.asciz	"D"
	.size	.Lstr.8, 2

	.type	.Lstr.9,@object                 # @str.9
.Lstr.9:
	.asciz	"e"
	.size	.Lstr.9, 2

	.type	.Lstr.10,@object                # @str.10
.Lstr.10:
	.asciz	"E"
	.size	.Lstr.10, 2

	.type	.Lstr.11,@object                # @str.11
.Lstr.11:
	.asciz	"f"
	.size	.Lstr.11, 2

	.type	.Lstr.12,@object                # @str.12
.Lstr.12:
	.asciz	"F"
	.size	.Lstr.12, 2

	.type	.Lstr.13,@object                # @str.13
.Lstr.13:
	.asciz	"g"
	.size	.Lstr.13, 2

	.type	.Lstr.14,@object                # @str.14
.Lstr.14:
	.asciz	"G"
	.size	.Lstr.14, 2

	.type	.Lstr.15,@object                # @str.15
.Lstr.15:
	.asciz	"h"
	.size	.Lstr.15, 2

	.type	.Lstr.16,@object                # @str.16
.Lstr.16:
	.asciz	"H"
	.size	.Lstr.16, 2

	.type	.Lstr.17,@object                # @str.17
.Lstr.17:
	.asciz	"i"
	.size	.Lstr.17, 2

	.type	.Lstr.18,@object                # @str.18
.Lstr.18:
	.asciz	"I"
	.size	.Lstr.18, 2

	.type	.Lstr.19,@object                # @str.19
.Lstr.19:
	.asciz	"j"
	.size	.Lstr.19, 2

	.type	.Lstr.20,@object                # @str.20
.Lstr.20:
	.asciz	"J"
	.size	.Lstr.20, 2

	.type	.Lstr.21,@object                # @str.21
.Lstr.21:
	.asciz	"k"
	.size	.Lstr.21, 2

	.type	.Lstr.22,@object                # @str.22
.Lstr.22:
	.asciz	"K"
	.size	.Lstr.22, 2

	.type	.Lstr.23,@object                # @str.23
.Lstr.23:
	.asciz	"l"
	.size	.Lstr.23, 2

	.type	.Lstr.24,@object                # @str.24
.Lstr.24:
	.asciz	"L"
	.size	.Lstr.24, 2

	.type	.Lstr.25,@object                # @str.25
.Lstr.25:
	.asciz	"m"
	.size	.Lstr.25, 2

	.type	.Lstr.26,@object                # @str.26
.Lstr.26:
	.asciz	"M"
	.size	.Lstr.26, 2

	.type	.Lstr.27,@object                # @str.27
.Lstr.27:
	.asciz	"n"
	.size	.Lstr.27, 2

	.type	.Lstr.28,@object                # @str.28
.Lstr.28:
	.asciz	"N"
	.size	.Lstr.28, 2

	.type	.Lstr.29,@object                # @str.29
.Lstr.29:
	.asciz	"\303\261"
	.size	.Lstr.29, 3

	.type	.Lstr.30,@object                # @str.30
.Lstr.30:
	.asciz	"\303\221"
	.size	.Lstr.30, 3

	.type	.Lstr.31,@object                # @str.31
.Lstr.31:
	.asciz	"o"
	.size	.Lstr.31, 2

	.type	.Lstr.32,@object                # @str.32
.Lstr.32:
	.asciz	"O"
	.size	.Lstr.32, 2

	.type	.Lstr.33,@object                # @str.33
.Lstr.33:
	.asciz	"p"
	.size	.Lstr.33, 2

	.type	.Lstr.34,@object                # @str.34
.Lstr.34:
	.asciz	"P"
	.size	.Lstr.34, 2

	.type	.Lstr.35,@object                # @str.35
.Lstr.35:
	.asciz	"q"
	.size	.Lstr.35, 2

	.type	.Lstr.36,@object                # @str.36
.Lstr.36:
	.asciz	"Q"
	.size	.Lstr.36, 2

	.type	.Lstr.37,@object                # @str.37
.Lstr.37:
	.asciz	"r"
	.size	.Lstr.37, 2

	.type	.Lstr.38,@object                # @str.38
.Lstr.38:
	.asciz	"R"
	.size	.Lstr.38, 2

	.type	.Lstr.39,@object                # @str.39
.Lstr.39:
	.asciz	"s"
	.size	.Lstr.39, 2

	.type	.Lstr.40,@object                # @str.40
.Lstr.40:
	.asciz	"S"
	.size	.Lstr.40, 2

	.type	.Lstr.41,@object                # @str.41
.Lstr.41:
	.asciz	"t"
	.size	.Lstr.41, 2

	.type	.Lstr.42,@object                # @str.42
.Lstr.42:
	.asciz	"T"
	.size	.Lstr.42, 2

	.type	.Lstr.43,@object                # @str.43
.Lstr.43:
	.asciz	"u"
	.size	.Lstr.43, 2

	.type	.Lstr.44,@object                # @str.44
.Lstr.44:
	.asciz	"U"
	.size	.Lstr.44, 2

	.type	.Lstr.45,@object                # @str.45
.Lstr.45:
	.asciz	"v"
	.size	.Lstr.45, 2

	.type	.Lstr.46,@object                # @str.46
.Lstr.46:
	.asciz	"V"
	.size	.Lstr.46, 2

	.type	.Lstr.47,@object                # @str.47
.Lstr.47:
	.asciz	"w"
	.size	.Lstr.47, 2

	.type	.Lstr.48,@object                # @str.48
.Lstr.48:
	.asciz	"W"
	.size	.Lstr.48, 2

	.type	.Lstr.49,@object                # @str.49
.Lstr.49:
	.asciz	"x"
	.size	.Lstr.49, 2

	.type	.Lstr.50,@object                # @str.50
.Lstr.50:
	.asciz	"X"
	.size	.Lstr.50, 2

	.type	.Lstr.51,@object                # @str.51
.Lstr.51:
	.asciz	"y"
	.size	.Lstr.51, 2

	.type	.Lstr.52,@object                # @str.52
.Lstr.52:
	.asciz	"Y"
	.size	.Lstr.52, 2

	.type	.Lstr.53,@object                # @str.53
.Lstr.53:
	.asciz	"z"
	.size	.Lstr.53, 2

	.type	.Lstr.54,@object                # @str.54
.Lstr.54:
	.asciz	"Z"
	.size	.Lstr.54, 2

	.type	.Lstr.55,@object                # @str.55
.Lstr.55:
	.zero	1
	.size	.Lstr.55, 1

	.type	.Lstr.56,@object                # @str.56
.Lstr.56:
	.asciz	"A"
	.size	.Lstr.56, 2

	.type	.Lstr.57,@object                # @str.57
.Lstr.57:
	.asciz	"a"
	.size	.Lstr.57, 2

	.type	.Lstr.58,@object                # @str.58
.Lstr.58:
	.asciz	"B"
	.size	.Lstr.58, 2

	.type	.Lstr.59,@object                # @str.59
.Lstr.59:
	.asciz	"b"
	.size	.Lstr.59, 2

	.type	.Lstr.60,@object                # @str.60
.Lstr.60:
	.asciz	"C"
	.size	.Lstr.60, 2

	.type	.Lstr.61,@object                # @str.61
.Lstr.61:
	.asciz	"c"
	.size	.Lstr.61, 2

	.type	.Lstr.62,@object                # @str.62
.Lstr.62:
	.asciz	"D"
	.size	.Lstr.62, 2

	.type	.Lstr.63,@object                # @str.63
.Lstr.63:
	.asciz	"d"
	.size	.Lstr.63, 2

	.type	.Lstr.64,@object                # @str.64
.Lstr.64:
	.asciz	"E"
	.size	.Lstr.64, 2

	.type	.Lstr.65,@object                # @str.65
.Lstr.65:
	.asciz	"e"
	.size	.Lstr.65, 2

	.type	.Lstr.66,@object                # @str.66
.Lstr.66:
	.asciz	"F"
	.size	.Lstr.66, 2

	.type	.Lstr.67,@object                # @str.67
.Lstr.67:
	.asciz	"f"
	.size	.Lstr.67, 2

	.type	.Lstr.68,@object                # @str.68
.Lstr.68:
	.asciz	"G"
	.size	.Lstr.68, 2

	.type	.Lstr.69,@object                # @str.69
.Lstr.69:
	.asciz	"g"
	.size	.Lstr.69, 2

	.type	.Lstr.70,@object                # @str.70
.Lstr.70:
	.asciz	"H"
	.size	.Lstr.70, 2

	.type	.Lstr.71,@object                # @str.71
.Lstr.71:
	.asciz	"h"
	.size	.Lstr.71, 2

	.type	.Lstr.72,@object                # @str.72
.Lstr.72:
	.asciz	"I"
	.size	.Lstr.72, 2

	.type	.Lstr.73,@object                # @str.73
.Lstr.73:
	.asciz	"i"
	.size	.Lstr.73, 2

	.type	.Lstr.74,@object                # @str.74
.Lstr.74:
	.asciz	"J"
	.size	.Lstr.74, 2

	.type	.Lstr.75,@object                # @str.75
.Lstr.75:
	.asciz	"j"
	.size	.Lstr.75, 2

	.type	.Lstr.76,@object                # @str.76
.Lstr.76:
	.asciz	"K"
	.size	.Lstr.76, 2

	.type	.Lstr.77,@object                # @str.77
.Lstr.77:
	.asciz	"k"
	.size	.Lstr.77, 2

	.type	.Lstr.78,@object                # @str.78
.Lstr.78:
	.asciz	"L"
	.size	.Lstr.78, 2

	.type	.Lstr.79,@object                # @str.79
.Lstr.79:
	.asciz	"l"
	.size	.Lstr.79, 2

	.type	.Lstr.80,@object                # @str.80
.Lstr.80:
	.asciz	"M"
	.size	.Lstr.80, 2

	.type	.Lstr.81,@object                # @str.81
.Lstr.81:
	.asciz	"m"
	.size	.Lstr.81, 2

	.type	.Lstr.82,@object                # @str.82
.Lstr.82:
	.asciz	"N"
	.size	.Lstr.82, 2

	.type	.Lstr.83,@object                # @str.83
.Lstr.83:
	.asciz	"n"
	.size	.Lstr.83, 2

	.type	.Lstr.84,@object                # @str.84
.Lstr.84:
	.asciz	"\303\221"
	.size	.Lstr.84, 3

	.type	.Lstr.85,@object                # @str.85
.Lstr.85:
	.asciz	"\303\261"
	.size	.Lstr.85, 3

	.type	.Lstr.86,@object                # @str.86
.Lstr.86:
	.asciz	"O"
	.size	.Lstr.86, 2

	.type	.Lstr.87,@object                # @str.87
.Lstr.87:
	.asciz	"o"
	.size	.Lstr.87, 2

	.type	.Lstr.88,@object                # @str.88
.Lstr.88:
	.asciz	"P"
	.size	.Lstr.88, 2

	.type	.Lstr.89,@object                # @str.89
.Lstr.89:
	.asciz	"p"
	.size	.Lstr.89, 2

	.type	.Lstr.90,@object                # @str.90
.Lstr.90:
	.asciz	"Q"
	.size	.Lstr.90, 2

	.type	.Lstr.91,@object                # @str.91
.Lstr.91:
	.asciz	"q"
	.size	.Lstr.91, 2

	.type	.Lstr.92,@object                # @str.92
.Lstr.92:
	.asciz	"R"
	.size	.Lstr.92, 2

	.type	.Lstr.93,@object                # @str.93
.Lstr.93:
	.asciz	"r"
	.size	.Lstr.93, 2

	.type	.Lstr.94,@object                # @str.94
.Lstr.94:
	.asciz	"S"
	.size	.Lstr.94, 2

	.type	.Lstr.95,@object                # @str.95
.Lstr.95:
	.asciz	"s"
	.size	.Lstr.95, 2

	.type	.Lstr.96,@object                # @str.96
.Lstr.96:
	.asciz	"T"
	.size	.Lstr.96, 2

	.type	.Lstr.97,@object                # @str.97
.Lstr.97:
	.asciz	"t"
	.size	.Lstr.97, 2

	.type	.Lstr.98,@object                # @str.98
.Lstr.98:
	.asciz	"U"
	.size	.Lstr.98, 2

	.type	.Lstr.99,@object                # @str.99
.Lstr.99:
	.asciz	"u"
	.size	.Lstr.99, 2

	.type	.Lstr.100,@object               # @str.100
.Lstr.100:
	.asciz	"V"
	.size	.Lstr.100, 2

	.type	.Lstr.101,@object               # @str.101
.Lstr.101:
	.asciz	"v"
	.size	.Lstr.101, 2

	.type	.Lstr.102,@object               # @str.102
.Lstr.102:
	.asciz	"W"
	.size	.Lstr.102, 2

	.type	.Lstr.103,@object               # @str.103
.Lstr.103:
	.asciz	"w"
	.size	.Lstr.103, 2

	.type	.Lstr.104,@object               # @str.104
.Lstr.104:
	.asciz	"X"
	.size	.Lstr.104, 2

	.type	.Lstr.105,@object               # @str.105
.Lstr.105:
	.asciz	"x"
	.size	.Lstr.105, 2

	.type	.Lstr.106,@object               # @str.106
.Lstr.106:
	.asciz	"Y"
	.size	.Lstr.106, 2

	.type	.Lstr.107,@object               # @str.107
.Lstr.107:
	.asciz	"y"
	.size	.Lstr.107, 2

	.type	.Lstr.108,@object               # @str.108
.Lstr.108:
	.asciz	"Z"
	.size	.Lstr.108, 2

	.type	.Lstr.109,@object               # @str.109
.Lstr.109:
	.asciz	"z"
	.size	.Lstr.109, 2

	.section	".note.GNU-stack","",@progbits
