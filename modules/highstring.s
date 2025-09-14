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
	leaq	.L.str(%rip), %rax
	movq	%rax, -56(%rbp)
	movl	$0, -44(%rbp)
	leaq	.L.str.1(%rip), %rbx
	leaq	.L.str.2(%rip), %r14
	leaq	.L.str.3(%rip), %r15
	leaq	.L.str.4(%rip), %r12
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
	leaq	.L.str.5(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_8
# %bb.34:                               # %elif_body_0_1
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.6(%rip), %rsi
	jmp	.LBB0_4
.LBB0_8:                                # %elif_0_2
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.7(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_9
# %bb.35:                               # %elif_body_0_2
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.8(%rip), %rsi
	jmp	.LBB0_4
.LBB0_9:                                # %elif_0_3
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.9(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_10
# %bb.36:                               # %elif_body_0_3
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.10(%rip), %rsi
	jmp	.LBB0_4
.LBB0_10:                               # %elif_0_4
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.11(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_11
# %bb.37:                               # %elif_body_0_4
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.12(%rip), %rsi
	jmp	.LBB0_4
.LBB0_11:                               # %elif_0_5
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.13(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_12
# %bb.38:                               # %elif_body_0_5
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.14(%rip), %rsi
	jmp	.LBB0_4
.LBB0_12:                               # %elif_0_6
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.15(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_13
# %bb.39:                               # %elif_body_0_6
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.16(%rip), %rsi
	jmp	.LBB0_4
.LBB0_13:                               # %elif_0_7
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.17(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_14
# %bb.40:                               # %elif_body_0_7
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.18(%rip), %rsi
	jmp	.LBB0_4
.LBB0_14:                               # %elif_0_8
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.19(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_15
# %bb.41:                               # %elif_body_0_8
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.20(%rip), %rsi
	jmp	.LBB0_4
.LBB0_15:                               # %elif_0_9
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.21(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_16
# %bb.42:                               # %elif_body_0_9
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.22(%rip), %rsi
	jmp	.LBB0_4
.LBB0_16:                               # %elif_0_10
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.23(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_17
# %bb.43:                               # %elif_body_0_10
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.24(%rip), %rsi
	jmp	.LBB0_4
.LBB0_17:                               # %elif_0_11
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.25(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_18
# %bb.44:                               # %elif_body_0_11
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.26(%rip), %rsi
	jmp	.LBB0_4
.LBB0_18:                               # %elif_0_12
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.27(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_19
# %bb.45:                               # %elif_body_0_12
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.28(%rip), %rsi
	jmp	.LBB0_4
.LBB0_19:                               # %elif_0_13
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.29(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_20
# %bb.46:                               # %elif_body_0_13
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.30(%rip), %rsi
	jmp	.LBB0_4
.LBB0_20:                               # %elif_0_14
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.31(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_21
# %bb.47:                               # %elif_body_0_14
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.32(%rip), %rsi
	jmp	.LBB0_4
.LBB0_21:                               # %elif_0_15
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.33(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_22
# %bb.48:                               # %elif_body_0_15
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.34(%rip), %rsi
	jmp	.LBB0_4
.LBB0_22:                               # %elif_0_16
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.35(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_23
# %bb.49:                               # %elif_body_0_16
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.36(%rip), %rsi
	jmp	.LBB0_4
.LBB0_23:                               # %elif_0_17
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.37(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_24
# %bb.50:                               # %elif_body_0_17
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.38(%rip), %rsi
	jmp	.LBB0_4
.LBB0_24:                               # %elif_0_18
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.39(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_25
# %bb.51:                               # %elif_body_0_18
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.40(%rip), %rsi
	jmp	.LBB0_4
.LBB0_25:                               # %elif_0_19
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.41(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_26
# %bb.52:                               # %elif_body_0_19
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.42(%rip), %rsi
	jmp	.LBB0_4
.LBB0_26:                               # %elif_0_20
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.43(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_27
# %bb.53:                               # %elif_body_0_20
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.44(%rip), %rsi
	jmp	.LBB0_4
.LBB0_27:                               # %elif_0_21
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.45(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_28
# %bb.54:                               # %elif_body_0_21
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.46(%rip), %rsi
	jmp	.LBB0_4
.LBB0_28:                               # %elif_0_22
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.47(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_29
# %bb.55:                               # %elif_body_0_22
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.48(%rip), %rsi
	jmp	.LBB0_4
.LBB0_29:                               # %elif_0_23
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.49(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_30
# %bb.56:                               # %elif_body_0_23
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.50(%rip), %rsi
	jmp	.LBB0_4
.LBB0_30:                               # %elif_0_24
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.51(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_31
# %bb.57:                               # %elif_body_0_24
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.52(%rip), %rsi
	jmp	.LBB0_4
.LBB0_31:                               # %elif_0_25
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.53(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB0_32
# %bb.58:                               # %elif_body_0_25
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.54(%rip), %rsi
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
	leaq	.L.str.55(%rip), %rax
	movq	%rax, -56(%rbp)
	movl	$0, -44(%rbp)
	leaq	.L.str.56(%rip), %rbx
	leaq	.L.str.57(%rip), %r14
	leaq	.L.str.58(%rip), %r15
	leaq	.L.str.59(%rip), %r12
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
	leaq	.L.str.60(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_8
# %bb.34:                               # %elif_body_1_1
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.61(%rip), %rsi
	jmp	.LBB1_4
.LBB1_8:                                # %elif_1_2
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.62(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_9
# %bb.35:                               # %elif_body_1_2
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.63(%rip), %rsi
	jmp	.LBB1_4
.LBB1_9:                                # %elif_1_3
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.64(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_10
# %bb.36:                               # %elif_body_1_3
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.65(%rip), %rsi
	jmp	.LBB1_4
.LBB1_10:                               # %elif_1_4
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.66(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_11
# %bb.37:                               # %elif_body_1_4
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.67(%rip), %rsi
	jmp	.LBB1_4
.LBB1_11:                               # %elif_1_5
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.68(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_12
# %bb.38:                               # %elif_body_1_5
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.69(%rip), %rsi
	jmp	.LBB1_4
.LBB1_12:                               # %elif_1_6
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.70(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_13
# %bb.39:                               # %elif_body_1_6
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.71(%rip), %rsi
	jmp	.LBB1_4
.LBB1_13:                               # %elif_1_7
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.72(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_14
# %bb.40:                               # %elif_body_1_7
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.73(%rip), %rsi
	jmp	.LBB1_4
.LBB1_14:                               # %elif_1_8
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.74(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_15
# %bb.41:                               # %elif_body_1_8
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.75(%rip), %rsi
	jmp	.LBB1_4
.LBB1_15:                               # %elif_1_9
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.76(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_16
# %bb.42:                               # %elif_body_1_9
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.77(%rip), %rsi
	jmp	.LBB1_4
.LBB1_16:                               # %elif_1_10
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.78(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_17
# %bb.43:                               # %elif_body_1_10
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.79(%rip), %rsi
	jmp	.LBB1_4
.LBB1_17:                               # %elif_1_11
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.80(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_18
# %bb.44:                               # %elif_body_1_11
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.81(%rip), %rsi
	jmp	.LBB1_4
.LBB1_18:                               # %elif_1_12
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.82(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_19
# %bb.45:                               # %elif_body_1_12
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.83(%rip), %rsi
	jmp	.LBB1_4
.LBB1_19:                               # %elif_1_13
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.84(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_20
# %bb.46:                               # %elif_body_1_13
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.85(%rip), %rsi
	jmp	.LBB1_4
.LBB1_20:                               # %elif_1_14
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.86(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_21
# %bb.47:                               # %elif_body_1_14
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.87(%rip), %rsi
	jmp	.LBB1_4
.LBB1_21:                               # %elif_1_15
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.88(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_22
# %bb.48:                               # %elif_body_1_15
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.89(%rip), %rsi
	jmp	.LBB1_4
.LBB1_22:                               # %elif_1_16
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.90(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_23
# %bb.49:                               # %elif_body_1_16
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.91(%rip), %rsi
	jmp	.LBB1_4
.LBB1_23:                               # %elif_1_17
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.92(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_24
# %bb.50:                               # %elif_body_1_17
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.93(%rip), %rsi
	jmp	.LBB1_4
.LBB1_24:                               # %elif_1_18
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.94(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_25
# %bb.51:                               # %elif_body_1_18
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.95(%rip), %rsi
	jmp	.LBB1_4
.LBB1_25:                               # %elif_1_19
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.96(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_26
# %bb.52:                               # %elif_body_1_19
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.97(%rip), %rsi
	jmp	.LBB1_4
.LBB1_26:                               # %elif_1_20
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.98(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_27
# %bb.53:                               # %elif_body_1_20
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.99(%rip), %rsi
	jmp	.LBB1_4
.LBB1_27:                               # %elif_1_21
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.100(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_28
# %bb.54:                               # %elif_body_1_21
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.101(%rip), %rsi
	jmp	.LBB1_4
.LBB1_28:                               # %elif_1_22
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.102(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_29
# %bb.55:                               # %elif_body_1_22
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.103(%rip), %rsi
	jmp	.LBB1_4
.LBB1_29:                               # %elif_1_23
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.104(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_30
# %bb.56:                               # %elif_body_1_23
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.105(%rip), %rsi
	jmp	.LBB1_4
.LBB1_30:                               # %elif_1_24
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.106(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_31
# %bb.57:                               # %elif_body_1_24
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.107(%rip), %rsi
	jmp	.LBB1_4
.LBB1_31:                               # %elif_1_25
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	(%r13), %rdi
	leaq	.L.str.108(%rip), %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	je	.LBB1_32
# %bb.58:                               # %elif_body_1_25
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	-56(%rbp), %rdi
	leaq	.L.str.109(%rip), %rsi
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
	.globl	is_whitespace                   # -- Begin function is_whitespace
	.p2align	4
	.type	is_whitespace,@function
is_whitespace:                          # @is_whitespace
	.cfi_startproc
# %bb.0:                                # %is_whitespace
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%rbx
	subq	$24, %rsp
	.cfi_offset %rbx, -24
	movq	%rdi, -32(%rbp)
	callq	len@PLT
	movl	%eax, -20(%rbp)
	movl	$1, -16(%rbp)
	movl	$0, -12(%rbp)
	leaq	.L.str.110(%rip), %rbx
	jmp	.LBB2_1
	.p2align	4
.LBB2_3:                                # %endif_2
                                        #   in Loop: Header=BB2_1 Depth=1
	incl	-12(%rbp)
.LBB2_1:                                # %for_start_3
                                        # =>This Inner Loop Header: Depth=1
	movl	-12(%rbp), %eax
	cmpl	-20(%rbp), %eax
	jge	.LBB2_5
# %bb.2:                                # %for_body_3
                                        #   in Loop: Header=BB2_1 Depth=1
	movq	-32(%rbp), %rdi
	movl	-12(%rbp), %esi
	leal	1(%rsi), %edx
                                        # kill: def $esi killed $esi killed $rsi
	callq	strsub@PLT
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rsp
	movq	%rax, -16(%rcx)
	movq	%rax, %rdi
	movq	%rbx, %rsi
	callq	strcmp@PLT
	testl	%eax, %eax
	jne	.LBB2_3
# %bb.4:                                # %if_2
                                        #   in Loop: Header=BB2_1 Depth=1
	movl	$0, -16(%rbp)
	incl	-12(%rbp)
	jmp	.LBB2_1
.LBB2_5:                                # %for_end_3
	movl	-16(%rbp), %eax
	leaq	-8(%rbp), %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end2:
	.size	is_whitespace, .Lfunc_end2-is_whitespace
	.cfi_endproc
                                        # -- End function
	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.zero	1
	.size	.L.str, 1

	.type	.L.str.1,@object                # @.str.1
.L.str.1:
	.asciz	"a"
	.size	.L.str.1, 2

	.type	.L.str.2,@object                # @.str.2
.L.str.2:
	.asciz	"A"
	.size	.L.str.2, 2

	.type	.L.str.3,@object                # @.str.3
.L.str.3:
	.asciz	"b"
	.size	.L.str.3, 2

	.type	.L.str.4,@object                # @.str.4
.L.str.4:
	.asciz	"B"
	.size	.L.str.4, 2

	.type	.L.str.5,@object                # @.str.5
.L.str.5:
	.asciz	"c"
	.size	.L.str.5, 2

	.type	.L.str.6,@object                # @.str.6
.L.str.6:
	.asciz	"C"
	.size	.L.str.6, 2

	.type	.L.str.7,@object                # @.str.7
.L.str.7:
	.asciz	"d"
	.size	.L.str.7, 2

	.type	.L.str.8,@object                # @.str.8
.L.str.8:
	.asciz	"D"
	.size	.L.str.8, 2

	.type	.L.str.9,@object                # @.str.9
.L.str.9:
	.asciz	"e"
	.size	.L.str.9, 2

	.type	.L.str.10,@object               # @.str.10
.L.str.10:
	.asciz	"E"
	.size	.L.str.10, 2

	.type	.L.str.11,@object               # @.str.11
.L.str.11:
	.asciz	"f"
	.size	.L.str.11, 2

	.type	.L.str.12,@object               # @.str.12
.L.str.12:
	.asciz	"F"
	.size	.L.str.12, 2

	.type	.L.str.13,@object               # @.str.13
.L.str.13:
	.asciz	"g"
	.size	.L.str.13, 2

	.type	.L.str.14,@object               # @.str.14
.L.str.14:
	.asciz	"G"
	.size	.L.str.14, 2

	.type	.L.str.15,@object               # @.str.15
.L.str.15:
	.asciz	"h"
	.size	.L.str.15, 2

	.type	.L.str.16,@object               # @.str.16
.L.str.16:
	.asciz	"H"
	.size	.L.str.16, 2

	.type	.L.str.17,@object               # @.str.17
.L.str.17:
	.asciz	"i"
	.size	.L.str.17, 2

	.type	.L.str.18,@object               # @.str.18
.L.str.18:
	.asciz	"I"
	.size	.L.str.18, 2

	.type	.L.str.19,@object               # @.str.19
.L.str.19:
	.asciz	"j"
	.size	.L.str.19, 2

	.type	.L.str.20,@object               # @.str.20
.L.str.20:
	.asciz	"J"
	.size	.L.str.20, 2

	.type	.L.str.21,@object               # @.str.21
.L.str.21:
	.asciz	"k"
	.size	.L.str.21, 2

	.type	.L.str.22,@object               # @.str.22
.L.str.22:
	.asciz	"K"
	.size	.L.str.22, 2

	.type	.L.str.23,@object               # @.str.23
.L.str.23:
	.asciz	"l"
	.size	.L.str.23, 2

	.type	.L.str.24,@object               # @.str.24
.L.str.24:
	.asciz	"L"
	.size	.L.str.24, 2

	.type	.L.str.25,@object               # @.str.25
.L.str.25:
	.asciz	"m"
	.size	.L.str.25, 2

	.type	.L.str.26,@object               # @.str.26
.L.str.26:
	.asciz	"M"
	.size	.L.str.26, 2

	.type	.L.str.27,@object               # @.str.27
.L.str.27:
	.asciz	"n"
	.size	.L.str.27, 2

	.type	.L.str.28,@object               # @.str.28
.L.str.28:
	.asciz	"N"
	.size	.L.str.28, 2

	.type	.L.str.29,@object               # @.str.29
.L.str.29:
	.asciz	"\303\261"
	.size	.L.str.29, 3

	.type	.L.str.30,@object               # @.str.30
.L.str.30:
	.asciz	"\303\221"
	.size	.L.str.30, 3

	.type	.L.str.31,@object               # @.str.31
.L.str.31:
	.asciz	"o"
	.size	.L.str.31, 2

	.type	.L.str.32,@object               # @.str.32
.L.str.32:
	.asciz	"O"
	.size	.L.str.32, 2

	.type	.L.str.33,@object               # @.str.33
.L.str.33:
	.asciz	"p"
	.size	.L.str.33, 2

	.type	.L.str.34,@object               # @.str.34
.L.str.34:
	.asciz	"P"
	.size	.L.str.34, 2

	.type	.L.str.35,@object               # @.str.35
.L.str.35:
	.asciz	"q"
	.size	.L.str.35, 2

	.type	.L.str.36,@object               # @.str.36
.L.str.36:
	.asciz	"Q"
	.size	.L.str.36, 2

	.type	.L.str.37,@object               # @.str.37
.L.str.37:
	.asciz	"r"
	.size	.L.str.37, 2

	.type	.L.str.38,@object               # @.str.38
.L.str.38:
	.asciz	"R"
	.size	.L.str.38, 2

	.type	.L.str.39,@object               # @.str.39
.L.str.39:
	.asciz	"s"
	.size	.L.str.39, 2

	.type	.L.str.40,@object               # @.str.40
.L.str.40:
	.asciz	"S"
	.size	.L.str.40, 2

	.type	.L.str.41,@object               # @.str.41
.L.str.41:
	.asciz	"t"
	.size	.L.str.41, 2

	.type	.L.str.42,@object               # @.str.42
.L.str.42:
	.asciz	"T"
	.size	.L.str.42, 2

	.type	.L.str.43,@object               # @.str.43
.L.str.43:
	.asciz	"u"
	.size	.L.str.43, 2

	.type	.L.str.44,@object               # @.str.44
.L.str.44:
	.asciz	"U"
	.size	.L.str.44, 2

	.type	.L.str.45,@object               # @.str.45
.L.str.45:
	.asciz	"v"
	.size	.L.str.45, 2

	.type	.L.str.46,@object               # @.str.46
.L.str.46:
	.asciz	"V"
	.size	.L.str.46, 2

	.type	.L.str.47,@object               # @.str.47
.L.str.47:
	.asciz	"w"
	.size	.L.str.47, 2

	.type	.L.str.48,@object               # @.str.48
.L.str.48:
	.asciz	"W"
	.size	.L.str.48, 2

	.type	.L.str.49,@object               # @.str.49
.L.str.49:
	.asciz	"x"
	.size	.L.str.49, 2

	.type	.L.str.50,@object               # @.str.50
.L.str.50:
	.asciz	"X"
	.size	.L.str.50, 2

	.type	.L.str.51,@object               # @.str.51
.L.str.51:
	.asciz	"y"
	.size	.L.str.51, 2

	.type	.L.str.52,@object               # @.str.52
.L.str.52:
	.asciz	"Y"
	.size	.L.str.52, 2

	.type	.L.str.53,@object               # @.str.53
.L.str.53:
	.asciz	"z"
	.size	.L.str.53, 2

	.type	.L.str.54,@object               # @.str.54
.L.str.54:
	.asciz	"Z"
	.size	.L.str.54, 2

	.type	.L.str.55,@object               # @.str.55
.L.str.55:
	.zero	1
	.size	.L.str.55, 1

	.type	.L.str.56,@object               # @.str.56
.L.str.56:
	.asciz	"A"
	.size	.L.str.56, 2

	.type	.L.str.57,@object               # @.str.57
.L.str.57:
	.asciz	"a"
	.size	.L.str.57, 2

	.type	.L.str.58,@object               # @.str.58
.L.str.58:
	.asciz	"B"
	.size	.L.str.58, 2

	.type	.L.str.59,@object               # @.str.59
.L.str.59:
	.asciz	"b"
	.size	.L.str.59, 2

	.type	.L.str.60,@object               # @.str.60
.L.str.60:
	.asciz	"C"
	.size	.L.str.60, 2

	.type	.L.str.61,@object               # @.str.61
.L.str.61:
	.asciz	"c"
	.size	.L.str.61, 2

	.type	.L.str.62,@object               # @.str.62
.L.str.62:
	.asciz	"D"
	.size	.L.str.62, 2

	.type	.L.str.63,@object               # @.str.63
.L.str.63:
	.asciz	"d"
	.size	.L.str.63, 2

	.type	.L.str.64,@object               # @.str.64
.L.str.64:
	.asciz	"E"
	.size	.L.str.64, 2

	.type	.L.str.65,@object               # @.str.65
.L.str.65:
	.asciz	"e"
	.size	.L.str.65, 2

	.type	.L.str.66,@object               # @.str.66
.L.str.66:
	.asciz	"F"
	.size	.L.str.66, 2

	.type	.L.str.67,@object               # @.str.67
.L.str.67:
	.asciz	"f"
	.size	.L.str.67, 2

	.type	.L.str.68,@object               # @.str.68
.L.str.68:
	.asciz	"G"
	.size	.L.str.68, 2

	.type	.L.str.69,@object               # @.str.69
.L.str.69:
	.asciz	"g"
	.size	.L.str.69, 2

	.type	.L.str.70,@object               # @.str.70
.L.str.70:
	.asciz	"H"
	.size	.L.str.70, 2

	.type	.L.str.71,@object               # @.str.71
.L.str.71:
	.asciz	"h"
	.size	.L.str.71, 2

	.type	.L.str.72,@object               # @.str.72
.L.str.72:
	.asciz	"I"
	.size	.L.str.72, 2

	.type	.L.str.73,@object               # @.str.73
.L.str.73:
	.asciz	"i"
	.size	.L.str.73, 2

	.type	.L.str.74,@object               # @.str.74
.L.str.74:
	.asciz	"J"
	.size	.L.str.74, 2

	.type	.L.str.75,@object               # @.str.75
.L.str.75:
	.asciz	"j"
	.size	.L.str.75, 2

	.type	.L.str.76,@object               # @.str.76
.L.str.76:
	.asciz	"K"
	.size	.L.str.76, 2

	.type	.L.str.77,@object               # @.str.77
.L.str.77:
	.asciz	"k"
	.size	.L.str.77, 2

	.type	.L.str.78,@object               # @.str.78
.L.str.78:
	.asciz	"L"
	.size	.L.str.78, 2

	.type	.L.str.79,@object               # @.str.79
.L.str.79:
	.asciz	"l"
	.size	.L.str.79, 2

	.type	.L.str.80,@object               # @.str.80
.L.str.80:
	.asciz	"M"
	.size	.L.str.80, 2

	.type	.L.str.81,@object               # @.str.81
.L.str.81:
	.asciz	"m"
	.size	.L.str.81, 2

	.type	.L.str.82,@object               # @.str.82
.L.str.82:
	.asciz	"N"
	.size	.L.str.82, 2

	.type	.L.str.83,@object               # @.str.83
.L.str.83:
	.asciz	"n"
	.size	.L.str.83, 2

	.type	.L.str.84,@object               # @.str.84
.L.str.84:
	.asciz	"\303\221"
	.size	.L.str.84, 3

	.type	.L.str.85,@object               # @.str.85
.L.str.85:
	.asciz	"\303\261"
	.size	.L.str.85, 3

	.type	.L.str.86,@object               # @.str.86
.L.str.86:
	.asciz	"O"
	.size	.L.str.86, 2

	.type	.L.str.87,@object               # @.str.87
.L.str.87:
	.asciz	"o"
	.size	.L.str.87, 2

	.type	.L.str.88,@object               # @.str.88
.L.str.88:
	.asciz	"P"
	.size	.L.str.88, 2

	.type	.L.str.89,@object               # @.str.89
.L.str.89:
	.asciz	"p"
	.size	.L.str.89, 2

	.type	.L.str.90,@object               # @.str.90
.L.str.90:
	.asciz	"Q"
	.size	.L.str.90, 2

	.type	.L.str.91,@object               # @.str.91
.L.str.91:
	.asciz	"q"
	.size	.L.str.91, 2

	.type	.L.str.92,@object               # @.str.92
.L.str.92:
	.asciz	"R"
	.size	.L.str.92, 2

	.type	.L.str.93,@object               # @.str.93
.L.str.93:
	.asciz	"r"
	.size	.L.str.93, 2

	.type	.L.str.94,@object               # @.str.94
.L.str.94:
	.asciz	"S"
	.size	.L.str.94, 2

	.type	.L.str.95,@object               # @.str.95
.L.str.95:
	.asciz	"s"
	.size	.L.str.95, 2

	.type	.L.str.96,@object               # @.str.96
.L.str.96:
	.asciz	"T"
	.size	.L.str.96, 2

	.type	.L.str.97,@object               # @.str.97
.L.str.97:
	.asciz	"t"
	.size	.L.str.97, 2

	.type	.L.str.98,@object               # @.str.98
.L.str.98:
	.asciz	"U"
	.size	.L.str.98, 2

	.type	.L.str.99,@object               # @.str.99
.L.str.99:
	.asciz	"u"
	.size	.L.str.99, 2

	.type	.L.str.100,@object              # @.str.100
.L.str.100:
	.asciz	"V"
	.size	.L.str.100, 2

	.type	.L.str.101,@object              # @.str.101
.L.str.101:
	.asciz	"v"
	.size	.L.str.101, 2

	.type	.L.str.102,@object              # @.str.102
.L.str.102:
	.asciz	"W"
	.size	.L.str.102, 2

	.type	.L.str.103,@object              # @.str.103
.L.str.103:
	.asciz	"w"
	.size	.L.str.103, 2

	.type	.L.str.104,@object              # @.str.104
.L.str.104:
	.asciz	"X"
	.size	.L.str.104, 2

	.type	.L.str.105,@object              # @.str.105
.L.str.105:
	.asciz	"x"
	.size	.L.str.105, 2

	.type	.L.str.106,@object              # @.str.106
.L.str.106:
	.asciz	"Y"
	.size	.L.str.106, 2

	.type	.L.str.107,@object              # @.str.107
.L.str.107:
	.asciz	"y"
	.size	.L.str.107, 2

	.type	.L.str.108,@object              # @.str.108
.L.str.108:
	.asciz	"Z"
	.size	.L.str.108, 2

	.type	.L.str.109,@object              # @.str.109
.L.str.109:
	.asciz	"z"
	.size	.L.str.109, 2

	.type	.L.str.110,@object              # @.str.110
.L.str.110:
	.asciz	" "
	.size	.L.str.110, 2

	.section	".note.GNU-stack","",@progbits
