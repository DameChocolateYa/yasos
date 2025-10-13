	.file	"MainModule"
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0                          # -- Begin function pow$MODmath
.LCPI0_0:
	.quad	0x3ff0000000000000              # double 1
	.text
	.globl	pow$MODmath
	.p2align	4
	.type	pow$MODmath,@function
pow$MODmath:                            # @"pow$MODmath"
	.cfi_startproc
# %bb.0:                                # %"pow$MODmath"
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movsd	%xmm0, -24(%rbp)
	movl	%edi, -12(%rbp)
	movabsq	$4607182418800017408, %rax      # imm = 0x3FF0000000000000
	movq	%rax, -8(%rbp)
	testl	%edi, %edi
	jne	.LBB0_1
# %bb.5:                                # %if_0
	xorpd	%xmm0, %xmm0
	jmp	.LBB0_6
.LBB0_1:                                # %else_0
	cmpl	$0, -12(%rbp)
	jle	.LBB0_2
# %bb.7:                                # %if_1
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rax
	movq	%rax, %rsp
	movl	$0, -16(%rcx)
	.p2align	4
.LBB0_8:                                # %for_start_1
                                        # =>This Inner Loop Header: Depth=1
	movl	(%rax), %ecx
	cmpl	-12(%rbp), %ecx
	jge	.LBB0_11
# %bb.9:                                # %for_body_1
                                        #   in Loop: Header=BB0_8 Depth=1
	movsd	-8(%rbp), %xmm0                 # xmm0 = mem[0],zero
	mulsd	-24(%rbp), %xmm0
	movsd	%xmm0, -8(%rbp)
	incl	(%rax)
	jmp	.LBB0_8
.LBB0_2:                                # %else_1
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rax
	movq	%rax, %rsp
	movl	$0, -16(%rcx)
	.p2align	4
.LBB0_3:                                # %for_start_2
                                        # =>This Inner Loop Header: Depth=1
	xorl	%ecx, %ecx
	subl	-12(%rbp), %ecx
	cmpl	%ecx, (%rax)
	jge	.LBB0_10
# %bb.4:                                # %for_body_2
                                        #   in Loop: Header=BB0_3 Depth=1
	movsd	-8(%rbp), %xmm0                 # xmm0 = mem[0],zero
	mulsd	-24(%rbp), %xmm0
	movsd	%xmm0, -8(%rbp)
	incl	(%rax)
	jmp	.LBB0_3
.LBB0_10:                               # %for_end_2
	movsd	.LCPI0_0(%rip), %xmm0           # xmm0 = [1.0E+0,0.0E+0]
	divsd	-8(%rbp), %xmm0
	movsd	%xmm0, -8(%rbp)
.LBB0_11:                               # %endif_1
	movsd	-8(%rbp), %xmm0                 # xmm0 = mem[0],zero
.LBB0_6:                                # %if_0
	movq	%rbp, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end0:
	.size	pow$MODmath, .Lfunc_end0-pow$MODmath
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0                          # -- Begin function ln$MODmath
.LCPI1_0:
	.quad	0x3ff8000000000000              # double 1.5
.LCPI1_1:
	.quad	0x3fe0000000000000              # double 0.5
.LCPI1_2:
	.quad	0x3ff0000000000000              # double 1
.LCPI1_3:
	.quad	0xbff0000000000000              # double -1
	.text
	.globl	ln$MODmath
	.p2align	4
	.type	ln$MODmath,@function
ln$MODmath:                             # @"ln$MODmath"
	.cfi_startproc
# %bb.0:                                # %"ln$MODmath"
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movsd	%xmm0, -8(%rbp)
	xorpd	%xmm1, %xmm1
	ucomisd	%xmm1, %xmm0
	ja	.LBB1_1
# %bb.4:                                # %if_2
	xorpd	%xmm0, %xmm0
	jmp	.LBB1_5
.LBB1_1:                                # %else_2
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rax
	movq	%rax, %rsp
	movabsq	$4604418534330597376, %rdx      # imm = 0x3FE62E4300000000
	movq	%rdx, -16(%rcx)
	movq	%rsp, %rdx
	leaq	-16(%rdx), %rcx
	movq	%rcx, %rsp
	movl	$0, -16(%rdx)
	movsd	-8(%rbp), %xmm0                 # xmm0 = mem[0],zero
	movq	%rsp, %rsi
	leaq	-16(%rsi), %rdx
	movq	%rdx, %rsp
	movsd	%xmm0, -16(%rsi)
	movsd	.LCPI1_0(%rip), %xmm1           # xmm1 = [1.5E+0,0.0E+0]
	movsd	.LCPI1_1(%rip), %xmm0           # xmm0 = [5.0E-1,0.0E+0]
	ucomisd	(%rdx), %xmm1
	jae	.LBB1_6
	.p2align	4
.LBB1_3:                                # %while_1_body
                                        # =>This Inner Loop Header: Depth=1
	movsd	(%rdx), %xmm2                   # xmm2 = mem[0],zero
	mulsd	%xmm0, %xmm2
	movsd	%xmm2, (%rdx)
	incl	(%rcx)
	ucomisd	(%rdx), %xmm1
	jb	.LBB1_3
	jmp	.LBB1_6
	.p2align	4
.LBB1_7:                                # %while_2_body
                                        #   in Loop: Header=BB1_6 Depth=1
	movsd	(%rdx), %xmm1                   # xmm1 = mem[0],zero
	addsd	%xmm1, %xmm1
	movsd	%xmm1, (%rdx)
	decl	(%rcx)
.LBB1_6:                                # %while_2_start
                                        # =>This Inner Loop Header: Depth=1
	movsd	(%rdx), %xmm1                   # xmm1 = mem[0],zero
	ucomisd	%xmm0, %xmm1
	jb	.LBB1_7
# %bb.8:                                # %while_2_end
	movsd	(%rdx), %xmm0                   # xmm0 = mem[0],zero
	movsd	.LCPI1_2(%rip), %xmm1           # xmm1 = [1.0E+0,0.0E+0]
	movapd	%xmm0, %xmm2
	subsd	%xmm1, %xmm2
	addsd	%xmm1, %xmm0
	divsd	%xmm0, %xmm2
	movq	%rsp, %rdi
	leaq	-16(%rdi), %rsp
	movsd	%xmm2, -16(%rdi)
	mulsd	%xmm2, %xmm2
	movq	%rsp, %rsi
	leaq	-16(%rsi), %rdx
	movq	%rdx, %rsp
	movsd	%xmm2, -16(%rsi)
	movq	%rsp, %r8
	leaq	-16(%r8), %rsi
	movq	%rsi, %rsp
	movq	$0, -16(%r8)
	movsd	-16(%rdi), %xmm0                # xmm0 = mem[0],zero
	movq	%rsp, %r8
	leaq	-16(%r8), %rdi
	movq	%rdi, %rsp
	movsd	%xmm0, -16(%r8)
	movq	%rsp, %r9
	leaq	-16(%r9), %r8
	movq	%r8, %rsp
	movl	$1, -16(%r9)
	movsd	.LCPI1_3(%rip), %xmm0           # xmm0 = [-1.0E+0,0.0E+0]
	cmpl	$50, (%r8)
	jg	.LBB1_11
	.p2align	4
.LBB1_10:                               # %for_body_3
                                        # =>This Inner Loop Header: Depth=1
	movsd	(%rdi), %xmm1                   # xmm1 = mem[0],zero
	xorps	%xmm2, %xmm2
	cvtsi2sdl	(%r8), %xmm2
	addsd	%xmm2, %xmm2
	addsd	%xmm0, %xmm2
	divsd	%xmm2, %xmm1
	addsd	(%rsi), %xmm1
	movsd	%xmm1, (%rsi)
	movsd	(%rdi), %xmm1                   # xmm1 = mem[0],zero
	mulsd	(%rdx), %xmm1
	movsd	%xmm1, (%rdi)
	incl	(%r8)
	cmpl	$50, (%r8)
	jle	.LBB1_10
.LBB1_11:                               # %for_end_3
	movsd	(%rsi), %xmm1                   # xmm1 = mem[0],zero
	addsd	%xmm1, %xmm1
	xorps	%xmm0, %xmm0
	cvtsi2sdl	(%rcx), %xmm0
	mulsd	(%rax), %xmm0
	addsd	%xmm1, %xmm0
.LBB1_5:                                # %if_2
	movq	%rbp, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end1:
	.size	ln$MODmath, .Lfunc_end1-ln$MODmath
	.cfi_endproc
                                        # -- End function
	.globl	log$MODmath                     # -- Begin function log$MODmath
	.p2align	4
	.type	log$MODmath,@function
log$MODmath:                            # @"log$MODmath"
	.cfi_startproc
# %bb.0:                                # %"log$MODmath"
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movsd	%xmm0, 16(%rsp)
	movsd	%xmm1, 8(%rsp)
	callq	ln$MODmath@PLT
	movsd	%xmm0, (%rsp)                   # 8-byte Spill
	movsd	8(%rsp), %xmm0                  # xmm0 = mem[0],zero
	callq	ln$MODmath@PLT
	movsd	(%rsp), %xmm1                   # 8-byte Reload
                                        # xmm1 = mem[0],zero
	divsd	%xmm0, %xmm1
	movapd	%xmm1, %xmm0
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end2:
	.size	log$MODmath, .Lfunc_end2-log$MODmath
	.cfi_endproc
                                        # -- End function
	.globl	log10$MODmath                   # -- Begin function log10$MODmath
	.p2align	4
	.type	log10$MODmath,@function
log10$MODmath:                          # @"log10$MODmath"
	.cfi_startproc
# %bb.0:                                # %"log10$MODmath"
	pushq	%rax
	.cfi_def_cfa_offset 16
	movsd	%xmm0, (%rsp)
	movl	$10, %edi
	callq	log$MODmath@PLT
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end3:
	.size	log10$MODmath, .Lfunc_end3-log10$MODmath
	.cfi_endproc
                                        # -- End function
	.globl	fact$MODmath                    # -- Begin function fact$MODmath
	.p2align	4
	.type	fact$MODmath,@function
fact$MODmath:                           # @"fact$MODmath"
	.cfi_startproc
# %bb.0:                                # %"fact$MODmath"
	movl	%edi, -4(%rsp)
	movl	$1, -8(%rsp)
	movl	%edi, -12(%rsp)
	cmpl	$0, -12(%rsp)
	jle	.LBB4_3
	.p2align	4
.LBB4_2:                                # %for_body_4
                                        # =>This Inner Loop Header: Depth=1
	movl	-8(%rsp), %eax
	imull	-12(%rsp), %eax
	movl	%eax, -8(%rsp)
	decl	-12(%rsp)
	cmpl	$0, -12(%rsp)
	jg	.LBB4_2
.LBB4_3:                                # %for_end_4
	movl	-8(%rsp), %eax
	retq
.Lfunc_end4:
	.size	fact$MODmath, .Lfunc_end4-fact$MODmath
	.cfi_endproc
                                        # -- End function
	.globl	factd$MODmath                   # -- Begin function factd$MODmath
	.p2align	4
	.type	factd$MODmath,@function
factd$MODmath:                          # @"factd$MODmath"
	.cfi_startproc
# %bb.0:                                # %"factd$MODmath"
	movl	%edi, -4(%rsp)
	movabsq	$4607182418800017408, %rax      # imm = 0x3FF0000000000000
	movq	%rax, -16(%rsp)
	movl	%edi, -20(%rsp)
	cmpl	$0, -20(%rsp)
	jle	.LBB5_3
	.p2align	4
.LBB5_2:                                # %for_body_5
                                        # =>This Inner Loop Header: Depth=1
	xorps	%xmm0, %xmm0
	cvtsi2sdl	-20(%rsp), %xmm0
	mulsd	-16(%rsp), %xmm0
	movsd	%xmm0, -16(%rsp)
	decl	-20(%rsp)
	cmpl	$0, -20(%rsp)
	jg	.LBB5_2
.LBB5_3:                                # %for_end_5
	movsd	-16(%rsp), %xmm0                # xmm0 = mem[0],zero
	retq
.Lfunc_end5:
	.size	factd$MODmath, .Lfunc_end5-factd$MODmath
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0                          # -- Begin function sin$MODmath
.LCPI6_0:
	.quad	0x402e000000000000              # double 15
.LCPI6_1:
	.quad	0x4000000000000000              # double 2
.LCPI6_2:
	.quad	0x4008000000000000              # double 3
.LCPI6_4:
	.quad	0x3ff0000000000000              # double 1
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4, 0x0
.LCPI6_3:
	.quad	0x8000000000000000              # double -0
	.quad	0x8000000000000000              # double -0
	.text
	.globl	sin$MODmath
	.p2align	4
	.type	sin$MODmath,@function
sin$MODmath:                            # @"sin$MODmath"
	.cfi_startproc
# %bb.0:                                # %"sin$MODmath"
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movsd	%xmm0, -32(%rbp)
	movq	$0, -24(%rbp)
	movsd	%xmm0, -16(%rbp)
	movq	$0, -8(%rbp)
	movsd	.LCPI6_0(%rip), %xmm0           # xmm0 = [1.5E+1,0.0E+0]
	movsd	.LCPI6_1(%rip), %xmm1           # xmm1 = [2.0E+0,0.0E+0]
	movsd	.LCPI6_2(%rip), %xmm2           # xmm2 = [3.0E+0,0.0E+0]
	movapd	.LCPI6_3(%rip), %xmm3           # xmm3 = [-0.0E+0,-0.0E+0]
	movsd	.LCPI6_4(%rip), %xmm4           # xmm4 = [1.0E+0,0.0E+0]
	.p2align	4
.LBB6_1:                                # %while_3_start
                                        # =>This Inner Loop Header: Depth=1
	movsd	-8(%rbp), %xmm5                 # xmm5 = mem[0],zero
	ucomisd	%xmm0, %xmm5
	jae	.LBB6_3
# %bb.2:                                # %while_3_body
                                        #   in Loop: Header=BB6_1 Depth=1
	movsd	-24(%rbp), %xmm5                # xmm5 = mem[0],zero
	addsd	-16(%rbp), %xmm5
	movsd	%xmm5, -24(%rbp)
	movsd	-8(%rbp), %xmm5                 # xmm5 = mem[0],zero
	addsd	%xmm5, %xmm5
	movapd	%xmm5, %xmm6
	addsd	%xmm1, %xmm6
	addsd	%xmm2, %xmm5
	mulsd	%xmm6, %xmm5
	movq	%rsp, %rax
	leaq	-16(%rax), %rsp
	movsd	%xmm5, -16(%rax)
	movsd	-32(%rbp), %xmm6                # xmm6 = mem[0],zero
	movapd	%xmm6, %xmm7
	xorpd	%xmm3, %xmm7
	mulsd	%xmm6, %xmm7
	divsd	%xmm5, %xmm7
	mulsd	-16(%rbp), %xmm7
	movsd	%xmm7, -16(%rbp)
	movsd	-8(%rbp), %xmm5                 # xmm5 = mem[0],zero
	addsd	%xmm4, %xmm5
	movsd	%xmm5, -8(%rbp)
	jmp	.LBB6_1
.LBB6_3:                                # %while_3_end
	movsd	-24(%rbp), %xmm0                # xmm0 = mem[0],zero
	movq	%rbp, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end6:
	.size	sin$MODmath, .Lfunc_end6-sin$MODmath
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0                          # -- Begin function cos$MODmath
.LCPI7_0:
	.quad	0x402e000000000000              # double 15
.LCPI7_1:
	.quad	0x3ff0000000000000              # double 1
.LCPI7_2:
	.quad	0x4000000000000000              # double 2
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4, 0x0
.LCPI7_3:
	.quad	0x8000000000000000              # double -0
	.quad	0x8000000000000000              # double -0
	.text
	.globl	cos$MODmath
	.p2align	4
	.type	cos$MODmath,@function
cos$MODmath:                            # @"cos$MODmath"
	.cfi_startproc
# %bb.0:                                # %"cos$MODmath"
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movsd	%xmm0, -32(%rbp)
	movq	$0, -24(%rbp)
	movabsq	$4607182418800017408, %rax      # imm = 0x3FF0000000000000
	movq	%rax, -16(%rbp)
	movq	$0, -8(%rbp)
	movsd	.LCPI7_0(%rip), %xmm0           # xmm0 = [1.5E+1,0.0E+0]
	movsd	.LCPI7_1(%rip), %xmm1           # xmm1 = [1.0E+0,0.0E+0]
	movsd	.LCPI7_2(%rip), %xmm2           # xmm2 = [2.0E+0,0.0E+0]
	movapd	.LCPI7_3(%rip), %xmm3           # xmm3 = [-0.0E+0,-0.0E+0]
	.p2align	4
.LBB7_1:                                # %while_4_start
                                        # =>This Inner Loop Header: Depth=1
	movsd	-8(%rbp), %xmm4                 # xmm4 = mem[0],zero
	ucomisd	%xmm0, %xmm4
	jae	.LBB7_3
# %bb.2:                                # %while_4_body
                                        #   in Loop: Header=BB7_1 Depth=1
	movsd	-24(%rbp), %xmm4                # xmm4 = mem[0],zero
	addsd	-16(%rbp), %xmm4
	movsd	%xmm4, -24(%rbp)
	movsd	-8(%rbp), %xmm4                 # xmm4 = mem[0],zero
	addsd	%xmm4, %xmm4
	movapd	%xmm4, %xmm5
	addsd	%xmm1, %xmm5
	addsd	%xmm2, %xmm4
	mulsd	%xmm5, %xmm4
	movq	%rsp, %rax
	leaq	-16(%rax), %rsp
	movsd	%xmm4, -16(%rax)
	movsd	-32(%rbp), %xmm5                # xmm5 = mem[0],zero
	movapd	%xmm5, %xmm6
	xorpd	%xmm3, %xmm6
	mulsd	%xmm5, %xmm6
	divsd	%xmm4, %xmm6
	movq	%rsp, %rax
	leaq	-16(%rax), %rsp
	movsd	%xmm6, -16(%rax)
	mulsd	-16(%rbp), %xmm6
	movsd	%xmm6, -16(%rbp)
	movsd	-8(%rbp), %xmm4                 # xmm4 = mem[0],zero
	addsd	%xmm1, %xmm4
	movsd	%xmm4, -8(%rbp)
	jmp	.LBB7_1
.LBB7_3:                                # %while_4_end
	movsd	-24(%rbp), %xmm0                # xmm0 = mem[0],zero
	movq	%rbp, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end7:
	.size	cos$MODmath, .Lfunc_end7-cos$MODmath
	.cfi_endproc
                                        # -- End function
	.globl	tan$MODmath                     # -- Begin function tan$MODmath
	.p2align	4
	.type	tan$MODmath,@function
tan$MODmath:                            # @"tan$MODmath"
	.cfi_startproc
# %bb.0:                                # %"tan$MODmath"
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movsd	%xmm0, 16(%rsp)
	callq	sin$MODmath@PLT
	movsd	%xmm0, 8(%rsp)
	movsd	16(%rsp), %xmm0                 # xmm0 = mem[0],zero
	callq	cos$MODmath@PLT
	movsd	%xmm0, (%rsp)
	xorpd	%xmm1, %xmm1
	ucomisd	%xmm1, %xmm0
	jne	.LBB8_1
# %bb.2:                                # %if_3
	xorpd	%xmm0, %xmm0
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.LBB8_1:                                # %else_3
	.cfi_def_cfa_offset 32
	movsd	8(%rsp), %xmm0                  # xmm0 = mem[0],zero
	divsd	(%rsp), %xmm0
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end8:
	.size	tan$MODmath, .Lfunc_end8-tan$MODmath
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0                          # -- Begin function cosec$MODmath
.LCPI9_0:
	.quad	0x3ff0000000000000              # double 1
	.text
	.globl	cosec$MODmath
	.p2align	4
	.type	cosec$MODmath,@function
cosec$MODmath:                          # @"cosec$MODmath"
	.cfi_startproc
# %bb.0:                                # %"cosec$MODmath"
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movsd	%xmm0, 16(%rsp)
	callq	sin$MODmath@PLT
	movsd	%xmm0, 8(%rsp)
	xorpd	%xmm1, %xmm1
	ucomisd	%xmm1, %xmm0
	jne	.LBB9_1
# %bb.2:                                # %if_4
	xorpd	%xmm0, %xmm0
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.LBB9_1:                                # %else_4
	.cfi_def_cfa_offset 32
	movsd	.LCPI9_0(%rip), %xmm0           # xmm0 = [1.0E+0,0.0E+0]
	divsd	8(%rsp), %xmm0
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end9:
	.size	cosec$MODmath, .Lfunc_end9-cosec$MODmath
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0                          # -- Begin function sec$MODmath
.LCPI10_0:
	.quad	0x3ff0000000000000              # double 1
	.text
	.globl	sec$MODmath
	.p2align	4
	.type	sec$MODmath,@function
sec$MODmath:                            # @"sec$MODmath"
	.cfi_startproc
# %bb.0:                                # %"sec$MODmath"
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movsd	%xmm0, 16(%rsp)
	callq	cos$MODmath@PLT
	movsd	%xmm0, 8(%rsp)
	xorpd	%xmm1, %xmm1
	ucomisd	%xmm1, %xmm0
	jne	.LBB10_1
# %bb.2:                                # %if_5
	xorpd	%xmm0, %xmm0
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.LBB10_1:                               # %else_5
	.cfi_def_cfa_offset 32
	movsd	.LCPI10_0(%rip), %xmm0          # xmm0 = [1.0E+0,0.0E+0]
	divsd	8(%rsp), %xmm0
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end10:
	.size	sec$MODmath, .Lfunc_end10-sec$MODmath
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0                          # -- Begin function cotan$MODmath
.LCPI11_0:
	.quad	0x3ff0000000000000              # double 1
	.text
	.globl	cotan$MODmath
	.p2align	4
	.type	cotan$MODmath,@function
cotan$MODmath:                          # @"cotan$MODmath"
	.cfi_startproc
# %bb.0:                                # %"cotan$MODmath"
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movsd	%xmm0, 16(%rsp)
	callq	tan$MODmath@PLT
	movsd	%xmm0, 8(%rsp)
	xorpd	%xmm1, %xmm1
	ucomisd	%xmm1, %xmm0
	jne	.LBB11_1
# %bb.2:                                # %if_6
	xorpd	%xmm0, %xmm0
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.LBB11_1:                               # %else_6
	.cfi_def_cfa_offset 32
	movsd	.LCPI11_0(%rip), %xmm0          # xmm0 = [1.0E+0,0.0E+0]
	divsd	8(%rsp), %xmm0
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end11:
	.size	cotan$MODmath, .Lfunc_end11-cotan$MODmath
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0                          # -- Begin function deg2rad$MODmath
.LCPI12_0:
	.quad	0x4066800000000000              # double 180
	.text
	.globl	deg2rad$MODmath
	.p2align	4
	.type	deg2rad$MODmath,@function
deg2rad$MODmath:                        # @"deg2rad$MODmath"
	.cfi_startproc
# %bb.0:                                # %"deg2rad$MODmath"
	movsd	%xmm0, -8(%rsp)
	movq	PI@GOTPCREL(%rip), %rax
	mulsd	(%rax), %xmm0
	divsd	.LCPI12_0(%rip), %xmm0
	retq
.Lfunc_end12:
	.size	deg2rad$MODmath, .Lfunc_end12-deg2rad$MODmath
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0                          # -- Begin function rad2deg$MODmath
.LCPI13_0:
	.quad	0x4066800000000000              # double 180
	.text
	.globl	rad2deg$MODmath
	.p2align	4
	.type	rad2deg$MODmath,@function
rad2deg$MODmath:                        # @"rad2deg$MODmath"
	.cfi_startproc
# %bb.0:                                # %"rad2deg$MODmath"
	movsd	%xmm0, -8(%rsp)
	mulsd	.LCPI13_0(%rip), %xmm0
	movq	PI@GOTPCREL(%rip), %rax
	divsd	(%rax), %xmm0
	retq
.Lfunc_end13:
	.size	rad2deg$MODmath, .Lfunc_end13-rad2deg$MODmath
	.cfi_endproc
                                        # -- End function
	.type	PI,@object                      # @PI
	.section	.rodata,"a",@progbits
	.globl	PI
	.p2align	3, 0x0
PI:
	.quad	0x400921fb00000000              # double 3.1415920257568359
	.size	PI, 8

	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"ERROR: ln(x) undefined for x <= 0\n"
	.size	.L.str, 35

	.type	.L.str.1,@object                # @.str.1
.L.str.1:
	.asciz	"ERROR: undefined tan\n"
	.size	.L.str.1, 22

	.type	.L.str.2,@object                # @.str.2
.L.str.2:
	.asciz	"ERROR: undefined sin\n"
	.size	.L.str.2, 22

	.type	.L.str.3,@object                # @.str.3
.L.str.3:
	.asciz	"ERROR: undefined cos\n"
	.size	.L.str.3, 22

	.type	.L.str.4,@object                # @.str.4
.L.str.4:
	.asciz	"ERROR: undefined tan\n"
	.size	.L.str.4, 22

	.section	".note.GNU-stack","",@progbits
