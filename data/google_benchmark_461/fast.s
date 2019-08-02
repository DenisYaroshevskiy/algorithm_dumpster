	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 14	sdk_version 10, 14
	.globl	__ZN5bench6detail16static_generatorEv ## -- Begin function _ZN5bench6detail16static_generatorEv
	.p2align	4, 0x90
__ZN5bench6detail16static_generatorEv:  ## @_ZN5bench6detail16static_generatorEv
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movb	__ZGVZN5bench6detail16static_generatorEvE1g(%rip), %al
	testb	%al, %al
	je	LBB0_1
LBB0_5:
	leaq	__ZZN5bench6detail16static_generatorEvE1g(%rip), %rax
	popq	%rbp
	retq
LBB0_1:
	leaq	__ZGVZN5bench6detail16static_generatorEvE1g(%rip), %rdi
	callq	___cxa_guard_acquire
	testl	%eax, %eax
	je	LBB0_5
## %bb.2:
	movl	$5489, __ZZN5bench6detail16static_generatorEvE1g(%rip) ## imm = 0x1571
	movl	$5489, %esi             ## imm = 0x1571
	movl	$1, %eax
	movl	$2, %ecx
	leaq	__ZZN5bench6detail16static_generatorEvE1g(%rip), %r8
	jmp	LBB0_3
LBB0_6:                                 ##   in Loop: Header=BB0_3 Depth=1
	leaq	1(%rax), %rsi
	movl	%edx, %edi
	shrl	$30, %edi
	xorl	%edx, %edi
	imull	$1812433253, %edi, %edx ## imm = 0x6C078965
	addl	%edx, %esi
	addl	%ecx, %edx
	movl	%edx, (%r8,%rcx,4)
	addq	$2, %rax
	addq	$2, %rcx
LBB0_3:                                 ## =>This Inner Loop Header: Depth=1
	movl	%esi, %edi
	shrl	$30, %edi
	xorl	%esi, %edi
	imull	$1812433253, %edi, %edx ## imm = 0x6C078965
	addl	%eax, %edx
	movl	%edx, -4(%r8,%rcx,4)
	cmpq	$624, %rcx              ## imm = 0x270
	jne	LBB0_6
## %bb.4:
	movq	$0, __ZZN5bench6detail16static_generatorEvE1g+2496(%rip)
	leaq	__ZGVZN5bench6detail16static_generatorEvE1g(%rip), %rdi
	callq	___cxa_guard_release
	leaq	__ZZN5bench6detail16static_generatorEvE1g(%rip), %rax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	__ZN5bench6detail11uniform_srcEm ## -- Begin function _ZN5bench6detail11uniform_srcEm
	.p2align	4, 0x90
__ZN5bench6detail11uniform_srcEm:       ## @_ZN5bench6detail11uniform_srcEm
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	shlq	$34, %rdi
	leaq	(%rdi,%rdi,4), %rax
	orq	$1, %rax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	__Z10do_nothingv        ## -- Begin function _Z10do_nothingv
	.p2align	4, 0x90
__Z10do_nothingv:                       ## @_Z10do_nothingv
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__literal16,16byte_literals
	.p2align	4               ## -- Begin function _Z14test_benchmarkRN9benchmark5StateE
LCPI3_0:
	.quad	1300                    ## 0x514
	.quad	700                     ## 0x2bc
	.section	__TEXT,__text,regular,pure_instructions
	.globl	__Z14test_benchmarkRN9benchmark5StateE
	.p2align	4, 0x90
__Z14test_benchmarkRN9benchmark5StateE: ## @_Z14test_benchmarkRN9benchmark5StateE
Lfunc_begin0:
	.cfi_startproc
	.cfi_personality 155, ___gxx_personality_v0
	.cfi_lsda 16, Lexception0
## %bb.0:
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
	subq	$72, %rsp
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	movq	%rdi, %rbx
	movq	__ZGVZN5bench18two_sorted_vectorsIiEENSt3__14pairINS1_6vectorIT_NS1_9allocatorIS4_EEEES7_EEmmE3gen@GOTPCREL(%rip), %rax
	movb	(%rax), %al
	testb	%al, %al
	je	LBB3_1
LBB3_3:
	movaps	LCPI3_0(%rip), %xmm0    ## xmm0 = [1300,700]
	movaps	%xmm0, -112(%rbp)
	movq	__ZZN5bench18two_sorted_vectorsIiEENSt3__14pairINS1_6vectorIT_NS1_9allocatorIS4_EEEES7_EEmmE3gen@GOTPCREL(%rip), %rsi
	leaq	-88(%rbp), %rdi
	leaq	-112(%rbp), %rdx
	callq	__ZZN5tools17memoized_functionINSt3__14pairImmEEZN5bench18two_sorted_vectorsIiEENS2_INS1_6vectorIT_NS1_9allocatorIS7_EEEESA_EEmmEUlS3_E_EEDaT0_ENUlRKS3_E_clESG_
Ltmp0:
	movl	$8000, %edi             ## imm = 0x1F40
	callq	__Znwm
Ltmp1:
## %bb.4:
	movq	%rax, %r15
	movl	$8000, %esi             ## imm = 0x1F40
	movq	%rax, %rdi
	callq	___bzero
	movb	26(%rbx), %r12b
	movq	16(%rbx), %r14
Ltmp3:
	movq	%rbx, -96(%rbp)         ## 8-byte Spill
	movq	%rbx, %rdi
	callq	__ZN9benchmark5State16StartKeepRunningEv
Ltmp4:
## %bb.5:
	testq	%r14, %r14
	je	LBB3_13
## %bb.6:
	testb	%r12b, %r12b
	jne	LBB3_13
	.p2align	4, 0x90
LBB3_7:                                 ## =>This Loop Header: Depth=1
                                        ##     Child Loop BB3_9 Depth 2
	movq	-88(%rbp), %rax
	movq	-80(%rbp), %r12
	movq	-64(%rbp), %rsi
	movq	-56(%rbp), %r13
	movq	%r15, %rbx
	cmpq	%r12, %rax
	jne	LBB3_9
	jmp	LBB3_24
	.p2align	4, 0x90
LBB3_21:                                ##   in Loop: Header=BB3_9 Depth=2
	movl	%ecx, (%rbx)
	addq	$4, %rsi
	addq	$4, %rbx
	cmpq	%r12, %rax
	je	LBB3_24
LBB3_9:                                 ##   Parent Loop BB3_7 Depth=1
                                        ## =>  This Inner Loop Header: Depth=2
	cmpq	%r13, %rsi
	je	LBB3_10
## %bb.20:                              ##   in Loop: Header=BB3_9 Depth=2
	movl	(%rsi), %ecx
	movl	(%rax), %edx
	cmpl	%edx, %ecx
	jl	LBB3_21
## %bb.22:                              ##   in Loop: Header=BB3_9 Depth=2
	movl	%edx, (%rbx)
	addq	$4, %rax
	addq	$4, %rbx
	cmpq	%r12, %rax
	jne	LBB3_9
LBB3_24:                                ##   in Loop: Header=BB3_7 Depth=1
	subq	%rsi, %r13
	je	LBB3_25
## %bb.26:                              ##   in Loop: Header=BB3_7 Depth=1
	movq	%rbx, %rdi
	movq	%r13, %rdx
	callq	_memmove
	movq	%r13, %r12
	addq	%r12, %rbx
	## InlineAsm Start
	## InlineAsm End
	decq	%r14
	jne	LBB3_7
	jmp	LBB3_13
	.p2align	4, 0x90
LBB3_10:                                ##   in Loop: Header=BB3_7 Depth=1
	subq	%rax, %r12
	je	LBB3_19
## %bb.11:                              ##   in Loop: Header=BB3_7 Depth=1
	movq	%rbx, %rdi
	movq	%rax, %rsi
	movq	%r12, %rdx
	callq	_memmove
	addq	%r12, %rbx
	## InlineAsm Start
	## InlineAsm End
	decq	%r14
	jne	LBB3_7
	jmp	LBB3_13
	.p2align	4, 0x90
LBB3_25:                                ##   in Loop: Header=BB3_7 Depth=1
	xorl	%r12d, %r12d
	addq	%r12, %rbx
	## InlineAsm Start
	## InlineAsm End
	decq	%r14
	jne	LBB3_7
	jmp	LBB3_13
LBB3_19:                                ##   in Loop: Header=BB3_7 Depth=1
	xorl	%r12d, %r12d
	addq	%r12, %rbx
	## InlineAsm Start
	## InlineAsm End
	decq	%r14
	jne	LBB3_7
LBB3_13:
Ltmp5:
	movq	-96(%rbp), %rdi         ## 8-byte Reload
	callq	__ZN9benchmark5State17FinishKeepRunningEv
Ltmp6:
## %bb.14:
	movq	%r15, %rdi
	callq	__ZdlPv
	movq	-64(%rbp), %rdi
	testq	%rdi, %rdi
	je	LBB3_16
## %bb.15:
	movq	%rdi, -56(%rbp)
	callq	__ZdlPv
LBB3_16:
	movq	-88(%rbp), %rdi
	testq	%rdi, %rdi
	je	LBB3_18
## %bb.17:
	movq	%rdi, -80(%rbp)
	callq	__ZdlPv
LBB3_18:
	addq	$72, %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
LBB3_1:
	movq	__ZGVZN5bench18two_sorted_vectorsIiEENSt3__14pairINS1_6vectorIT_NS1_9allocatorIS4_EEEES7_EEmmE3gen@GOTPCREL(%rip), %rdi
	callq	___cxa_guard_acquire
	testl	%eax, %eax
	je	LBB3_3
## %bb.2:
	movq	__ZZN5bench18two_sorted_vectorsIiEENSt3__14pairINS1_6vectorIT_NS1_9allocatorIS4_EEEES7_EEmmE3gen@GOTPCREL(%rip), %rsi
	movq	$0, 16(%rsi)
	movq	$0, 8(%rsi)
	leaq	8(%rsi), %rax
	movq	%rax, (%rsi)
	movq	__ZZN5tools17memoized_functionINSt3__14pairImmEEZN5bench18two_sorted_vectorsIiEENS2_INS1_6vectorIT_NS1_9allocatorIS7_EEEESA_EEmmEUlS3_E_EEDaT0_ENUlRKS3_E_D1Ev@GOTPCREL(%rip), %rdi
	leaq	___dso_handle(%rip), %rdx
	callq	___cxa_atexit
	movq	__ZGVZN5bench18two_sorted_vectorsIiEENSt3__14pairINS1_6vectorIT_NS1_9allocatorIS4_EEEES7_EEmmE3gen@GOTPCREL(%rip), %rdi
	callq	___cxa_guard_release
	jmp	LBB3_3
LBB3_33:
Ltmp2:
	movq	%rax, %rbx
	movq	-64(%rbp), %rdi
	testq	%rdi, %rdi
	jne	LBB3_29
	jmp	LBB3_30
LBB3_27:
Ltmp7:
	movq	%rax, %rbx
	movq	%r15, %rdi
	callq	__ZdlPv
	movq	-64(%rbp), %rdi
	testq	%rdi, %rdi
	je	LBB3_30
LBB3_29:
	movq	%rdi, -56(%rbp)
	callq	__ZdlPv
LBB3_30:
	movq	-88(%rbp), %rdi
	testq	%rdi, %rdi
	jne	LBB3_31
## %bb.32:
	movq	%rbx, %rdi
	callq	__Unwind_Resume
	ud2
LBB3_31:
	movq	%rdi, -80(%rbp)
	callq	__ZdlPv
	movq	%rbx, %rdi
	callq	__Unwind_Resume
	ud2
Lfunc_end0:
	.cfi_endproc
	.section	__TEXT,__gcc_except_tab
	.p2align	2
GCC_except_table3:
Lexception0:
	.byte	255                     ## @LPStart Encoding = omit
	.byte	255                     ## @TType Encoding = omit
	.byte	1                       ## Call site Encoding = uleb128
	.uleb128 Lcst_end0-Lcst_begin0
Lcst_begin0:
	.uleb128 Lfunc_begin0-Lfunc_begin0 ## >> Call Site 1 <<
	.uleb128 Ltmp0-Lfunc_begin0     ##   Call between Lfunc_begin0 and Ltmp0
	.byte	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp0-Lfunc_begin0     ## >> Call Site 2 <<
	.uleb128 Ltmp1-Ltmp0            ##   Call between Ltmp0 and Ltmp1
	.uleb128 Ltmp2-Lfunc_begin0     ##     jumps to Ltmp2
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp1-Lfunc_begin0     ## >> Call Site 3 <<
	.uleb128 Ltmp3-Ltmp1            ##   Call between Ltmp1 and Ltmp3
	.byte	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp3-Lfunc_begin0     ## >> Call Site 4 <<
	.uleb128 Ltmp4-Ltmp3            ##   Call between Ltmp3 and Ltmp4
	.uleb128 Ltmp7-Lfunc_begin0     ##     jumps to Ltmp7
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp4-Lfunc_begin0     ## >> Call Site 5 <<
	.uleb128 Ltmp5-Ltmp4            ##   Call between Ltmp4 and Ltmp5
	.byte	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp5-Lfunc_begin0     ## >> Call Site 6 <<
	.uleb128 Ltmp6-Ltmp5            ##   Call between Ltmp5 and Ltmp6
	.uleb128 Ltmp7-Lfunc_begin0     ##     jumps to Ltmp7
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp6-Lfunc_begin0     ## >> Call Site 7 <<
	.uleb128 Lfunc_end0-Ltmp6       ##   Call between Ltmp6 and Lfunc_end0
	.byte	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
Lcst_end0:
	.p2align	2
                                        ## -- End function
	.section	__TEXT,__text,regular,pure_instructions
	.globl	__ZZN5tools17memoized_functionINSt3__14pairImmEEZN5bench18two_sorted_vectorsIiEENS2_INS1_6vectorIT_NS1_9allocatorIS7_EEEESA_EEmmEUlS3_E_EEDaT0_ENUlRKS3_E_D1Ev ## -- Begin function _ZZN5tools17memoized_functionINSt3__14pairImmEEZN5bench18two_sorted_vectorsIiEENS2_INS1_6vectorIT_NS1_9allocatorIS7_EEEESA_EEmmEUlS3_E_EEDaT0_ENUlRKS3_E_D1Ev
	.weak_def_can_be_hidden	__ZZN5tools17memoized_functionINSt3__14pairImmEEZN5bench18two_sorted_vectorsIiEENS2_INS1_6vectorIT_NS1_9allocatorIS7_EEEESA_EEmmEUlS3_E_EEDaT0_ENUlRKS3_E_D1Ev
	.p2align	4, 0x90
__ZZN5tools17memoized_functionINSt3__14pairImmEEZN5bench18two_sorted_vectorsIiEENS2_INS1_6vectorIT_NS1_9allocatorIS7_EEEESA_EEmmEUlS3_E_EEDaT0_ENUlRKS3_E_D1Ev: ## @_ZZN5tools17memoized_functionINSt3__14pairImmEEZN5bench18two_sorted_vectorsIiEENS2_INS1_6vectorIT_NS1_9allocatorIS7_EEEESA_EEmmEUlS3_E_EEDaT0_ENUlRKS3_E_D1Ev
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	8(%rdi), %rsi
	popq	%rbp
	jmp	__ZNSt3__16__treeINS_12__value_typeINS_4pairImmEENS2_INS_6vectorIiNS_9allocatorIiEEEES7_EEEENS_19__map_value_compareIS3_S9_NS_4lessIS3_EELb1EEENS5_IS9_EEE7destroyEPNS_11__tree_nodeIS9_PvEE ## TAILCALL
	.cfi_endproc
                                        ## -- End function
	.globl	__ZZN5tools17memoized_functionINSt3__14pairImmEEZN5bench18two_sorted_vectorsIiEENS2_INS1_6vectorIT_NS1_9allocatorIS7_EEEESA_EEmmEUlS3_E_EEDaT0_ENUlRKS3_E_clESG_ ## -- Begin function _ZZN5tools17memoized_functionINSt3__14pairImmEEZN5bench18two_sorted_vectorsIiEENS2_INS1_6vectorIT_NS1_9allocatorIS7_EEEESA_EEmmEUlS3_E_EEDaT0_ENUlRKS3_E_clESG_
	.weak_def_can_be_hidden	__ZZN5tools17memoized_functionINSt3__14pairImmEEZN5bench18two_sorted_vectorsIiEENS2_INS1_6vectorIT_NS1_9allocatorIS7_EEEESA_EEmmEUlS3_E_EEDaT0_ENUlRKS3_E_clESG_
	.p2align	4, 0x90
__ZZN5tools17memoized_functionINSt3__14pairImmEEZN5bench18two_sorted_vectorsIiEENS2_INS1_6vectorIT_NS1_9allocatorIS7_EEEESA_EEmmEUlS3_E_EEDaT0_ENUlRKS3_E_clESG_: ## @_ZZN5tools17memoized_functionINSt3__14pairImmEEZN5bench18two_sorted_vectorsIiEENS2_INS1_6vectorIT_NS1_9allocatorIS7_EEEESA_EEmmEUlS3_E_EEDaT0_ENUlRKS3_E_clESG_
Lfunc_begin1:
	.cfi_startproc
	.cfi_personality 155, ___gxx_personality_v0
	.cfi_lsda 16, Lexception1
## %bb.0:
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
	subq	$136, %rsp
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	movq	%rdx, %r13
	movq	%rsi, %r15
	movq	%rdi, %r14
	movq	(%rdx), %rsi
	movq	8(%rdx), %rbx
	leaq	8(%r15), %r12
	movq	8(%r15), %rcx
	testq	%rcx, %rcx
	je	LBB5_12
## %bb.1:
	movq	%r12, %rax
	.p2align	4, 0x90
LBB5_2:                                 ## =>This Inner Loop Header: Depth=1
	movq	%rax, %rdx
	cmpq	%rsi, 32(%rcx)
	jb	LBB5_6
## %bb.3:                               ##   in Loop: Header=BB5_2 Depth=1
	ja	LBB5_4
## %bb.5:                               ##   in Loop: Header=BB5_2 Depth=1
	cmpq	%rbx, 40(%rcx)
	movq	%rcx, %rax
	jae	LBB5_7
LBB5_6:                                 ##   in Loop: Header=BB5_2 Depth=1
	addq	$8, %rcx
	movq	%rdx, %rax
LBB5_7:                                 ##   in Loop: Header=BB5_2 Depth=1
	movq	(%rcx), %rcx
	testq	%rcx, %rcx
	jne	LBB5_2
	jmp	LBB5_8
LBB5_4:                                 ##   in Loop: Header=BB5_2 Depth=1
	movq	%rcx, %rax
	movq	(%rcx), %rcx
	testq	%rcx, %rcx
	jne	LBB5_2
LBB5_8:
	cmpq	%r12, %rax
	je	LBB5_12
## %bb.9:
	cmpq	%rsi, 32(%rax)
	jne	LBB5_11
## %bb.10:
	cmpq	%rbx, 40(%rax)
	jne	LBB5_11
## %bb.29:
	addq	$48, %rax
	movq	%r14, %rdi
	movq	%rax, %rsi
	callq	__ZNSt3__14pairINS_6vectorIiNS_9allocatorIiEEEES4_EC1ERKS5_
	movq	%r14, %rax
	jmp	LBB5_20
LBB5_11:
	movq	%rax, %r12
LBB5_12:
	movq	%r14, -48(%rbp)         ## 8-byte Spill
	leal	(%rbx,%rsi), %eax
	shlq	$34, %rax
	leaq	(%rax,%rax,4), %r14
	orq	$1, %r14
	leaq	-144(%rbp), %rdi
	movq	%r14, %rdx
	callq	__ZN5bench6detail22generate_sorted_vectorIiZNS0_11uniform_srcEmE3$_0EEDamT0_
Ltmp8:
	leaq	-176(%rbp), %rdi
	movq	%rbx, %rsi
	movq	%r14, %rdx
	callq	__ZN5bench6detail22generate_sorted_vectorIiZNS0_11uniform_srcEmE3$_0EEDamT0_
Ltmp9:
## %bb.13:
	movaps	-144(%rbp), %xmm0
	movq	-128(%rbp), %rax
	movaps	-176(%rbp), %xmm1
	movq	-160(%rbp), %rcx
	movq	(%r13), %rdx
	movq	8(%r13), %rsi
	movq	%rsi, -104(%rbp)
	movq	%rdx, -112(%rbp)
	movups	%xmm0, -96(%rbp)
	movq	%rax, -80(%rbp)
	movups	%xmm1, -72(%rbp)
	movq	%rcx, -56(%rbp)
Ltmp11:
	leaq	-112(%rbp), %rdx
	movq	%r15, %rdi
	movq	%r12, %rsi
	movq	%rdx, %rcx
	callq	__ZNSt3__16__treeINS_12__value_typeINS_4pairImmEENS2_INS_6vectorIiNS_9allocatorIiEEEES7_EEEENS_19__map_value_compareIS3_S9_NS_4lessIS3_EELb1EEENS5_IS9_EEE30__emplace_hint_unique_key_argsIS3_JNS2_IKS3_S8_EEEEENS_15__tree_iteratorIS9_PNS_11__tree_nodeIS9_PvEElEENS_21__tree_const_iteratorIS9_SN_lEERKT_DpOT0_
Ltmp12:
	movq	-48(%rbp), %rbx         ## 8-byte Reload
## %bb.14:
	addq	$48, %rax
Ltmp13:
	movq	%rbx, %rdi
	movq	%rax, %rsi
	callq	__ZNSt3__14pairINS_6vectorIiNS_9allocatorIiEEEES4_EC1ERKS5_
Ltmp14:
## %bb.15:
	movq	-72(%rbp), %rdi
	testq	%rdi, %rdi
	je	LBB5_17
## %bb.16:
	movq	%rdi, -64(%rbp)
	callq	__ZdlPv
LBB5_17:
	movq	-96(%rbp), %rdi
	testq	%rdi, %rdi
	je	LBB5_19
## %bb.18:
	movq	%rdi, -88(%rbp)
	callq	__ZdlPv
LBB5_19:
	movq	%rbx, %rax
LBB5_20:
	addq	$136, %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
LBB5_21:
Ltmp10:
	movq	%rax, %rbx
	movq	-144(%rbp), %rdi
	testq	%rdi, %rdi
	je	LBB5_28
## %bb.22:
	movq	%rdi, -136(%rbp)
	jmp	LBB5_27
LBB5_23:
Ltmp15:
	movq	%rax, %rbx
	movq	-72(%rbp), %rdi
	testq	%rdi, %rdi
	je	LBB5_25
## %bb.24:
	movq	%rdi, -64(%rbp)
	callq	__ZdlPv
LBB5_25:
	movq	-96(%rbp), %rdi
	testq	%rdi, %rdi
	je	LBB5_28
## %bb.26:
	movq	%rdi, -88(%rbp)
LBB5_27:
	callq	__ZdlPv
LBB5_28:
	movq	%rbx, %rdi
	callq	__Unwind_Resume
	ud2
Lfunc_end1:
	.cfi_endproc
	.section	__TEXT,__gcc_except_tab
	.p2align	2
GCC_except_table5:
Lexception1:
	.byte	255                     ## @LPStart Encoding = omit
	.byte	255                     ## @TType Encoding = omit
	.byte	1                       ## Call site Encoding = uleb128
	.uleb128 Lcst_end1-Lcst_begin1
Lcst_begin1:
	.uleb128 Lfunc_begin1-Lfunc_begin1 ## >> Call Site 1 <<
	.uleb128 Ltmp8-Lfunc_begin1     ##   Call between Lfunc_begin1 and Ltmp8
	.byte	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp8-Lfunc_begin1     ## >> Call Site 2 <<
	.uleb128 Ltmp9-Ltmp8            ##   Call between Ltmp8 and Ltmp9
	.uleb128 Ltmp10-Lfunc_begin1    ##     jumps to Ltmp10
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp11-Lfunc_begin1    ## >> Call Site 3 <<
	.uleb128 Ltmp14-Ltmp11          ##   Call between Ltmp11 and Ltmp14
	.uleb128 Ltmp15-Lfunc_begin1    ##     jumps to Ltmp15
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp14-Lfunc_begin1    ## >> Call Site 4 <<
	.uleb128 Lfunc_end1-Ltmp14      ##   Call between Ltmp14 and Lfunc_end1
	.byte	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
Lcst_end1:
	.p2align	2
                                        ## -- End function
	.section	__TEXT,__text,regular,pure_instructions
	.globl	__ZNSt3__16__treeINS_12__value_typeINS_4pairImmEENS2_INS_6vectorIiNS_9allocatorIiEEEES7_EEEENS_19__map_value_compareIS3_S9_NS_4lessIS3_EELb1EEENS5_IS9_EEE7destroyEPNS_11__tree_nodeIS9_PvEE ## -- Begin function _ZNSt3__16__treeINS_12__value_typeINS_4pairImmEENS2_INS_6vectorIiNS_9allocatorIiEEEES7_EEEENS_19__map_value_compareIS3_S9_NS_4lessIS3_EELb1EEENS5_IS9_EEE7destroyEPNS_11__tree_nodeIS9_PvEE
	.weak_def_can_be_hidden	__ZNSt3__16__treeINS_12__value_typeINS_4pairImmEENS2_INS_6vectorIiNS_9allocatorIiEEEES7_EEEENS_19__map_value_compareIS3_S9_NS_4lessIS3_EELb1EEENS5_IS9_EEE7destroyEPNS_11__tree_nodeIS9_PvEE
	.p2align	4, 0x90
__ZNSt3__16__treeINS_12__value_typeINS_4pairImmEENS2_INS_6vectorIiNS_9allocatorIiEEEES7_EEEENS_19__map_value_compareIS3_S9_NS_4lessIS3_EELb1EEENS5_IS9_EEE7destroyEPNS_11__tree_nodeIS9_PvEE: ## @_ZNSt3__16__treeINS_12__value_typeINS_4pairImmEENS2_INS_6vectorIiNS_9allocatorIiEEEES7_EEEENS_19__map_value_compareIS3_S9_NS_4lessIS3_EELb1EEENS5_IS9_EEE7destroyEPNS_11__tree_nodeIS9_PvEE
	.cfi_startproc
## %bb.0:
	testq	%rsi, %rsi
	je	LBB6_6
## %bb.1:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r14
	pushq	%rbx
	.cfi_offset %rbx, -32
	.cfi_offset %r14, -24
	movq	%rsi, %rbx
	movq	%rdi, %r14
	movq	(%rsi), %rsi
	callq	__ZNSt3__16__treeINS_12__value_typeINS_4pairImmEENS2_INS_6vectorIiNS_9allocatorIiEEEES7_EEEENS_19__map_value_compareIS3_S9_NS_4lessIS3_EELb1EEENS5_IS9_EEE7destroyEPNS_11__tree_nodeIS9_PvEE
	movq	8(%rbx), %rsi
	movq	%r14, %rdi
	callq	__ZNSt3__16__treeINS_12__value_typeINS_4pairImmEENS2_INS_6vectorIiNS_9allocatorIiEEEES7_EEEENS_19__map_value_compareIS3_S9_NS_4lessIS3_EELb1EEENS5_IS9_EEE7destroyEPNS_11__tree_nodeIS9_PvEE
	movq	72(%rbx), %rdi
	testq	%rdi, %rdi
	je	LBB6_3
## %bb.2:
	movq	%rdi, 80(%rbx)
	callq	__ZdlPv
LBB6_3:
	movq	48(%rbx), %rdi
	testq	%rdi, %rdi
	je	LBB6_5
## %bb.4:
	movq	%rdi, 56(%rbx)
	callq	__ZdlPv
LBB6_5:
	movq	%rbx, %rdi
	popq	%rbx
	popq	%r14
	popq	%rbp
	jmp	__ZdlPv                 ## TAILCALL
LBB6_6:
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	__ZNSt3__14pairINS_6vectorIiNS_9allocatorIiEEEES4_EC1ERKS5_ ## -- Begin function _ZNSt3__14pairINS_6vectorIiNS_9allocatorIiEEEES4_EC1ERKS5_
	.weak_def_can_be_hidden	__ZNSt3__14pairINS_6vectorIiNS_9allocatorIiEEEES4_EC1ERKS5_
	.p2align	4, 0x90
__ZNSt3__14pairINS_6vectorIiNS_9allocatorIiEEEES4_EC1ERKS5_: ## @_ZNSt3__14pairINS_6vectorIiNS_9allocatorIiEEEES4_EC1ERKS5_
Lfunc_begin2:
	.cfi_startproc
	.cfi_personality 155, ___gxx_personality_v0
	.cfi_lsda 16, Lexception2
## %bb.0:
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
	pushq	%rax
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	movq	%rsi, %r14
	movq	%rdi, %r12
	movabsq	$4611686018427387903, %r13 ## imm = 0x3FFFFFFFFFFFFFFF
	movq	$0, 16(%rdi)
	movq	$0, 8(%rdi)
	movq	$0, (%rdi)
	movq	8(%rsi), %rdi
	subq	(%rsi), %rdi
	je	LBB7_7
## %bb.1:
	movq	%rdi, %rbx
	sarq	$2, %rbx
	cmpq	%r13, %rbx
	ja	LBB7_2
## %bb.4:
Ltmp16:
	callq	__Znwm
Ltmp17:
## %bb.5:
	movq	%rax, %r15
	movq	%rax, 8(%r12)
	movq	%rax, (%r12)
	leaq	(%rax,%rbx,4), %rax
	movq	%rax, 16(%r12)
	movq	(%r14), %rsi
	movq	8(%r14), %rbx
	subq	%rsi, %rbx
	testq	%rbx, %rbx
	jle	LBB7_7
## %bb.6:
	movq	%r15, %rdi
	movq	%rbx, %rdx
	callq	_memcpy
	addq	%rbx, %r15
	movq	%r15, 8(%r12)
LBB7_7:
	movq	$0, 40(%r12)
	movq	$0, 32(%r12)
	movq	$0, 24(%r12)
	movq	32(%r14), %rdi
	subq	24(%r14), %rdi
	je	LBB7_14
## %bb.8:
	leaq	24(%r12), %r15
	movq	%rdi, %rbx
	sarq	$2, %rbx
	cmpq	%r13, %rbx
	ja	LBB7_9
## %bb.11:
Ltmp21:
	callq	__Znwm
Ltmp22:
## %bb.12:
	movq	%rax, %r13
	movq	%rax, 32(%r12)
	movq	%rax, 24(%r12)
	leaq	(%rax,%rbx,4), %rax
	movq	%rax, 40(%r12)
	movq	24(%r14), %rsi
	movq	32(%r14), %rbx
	subq	%rsi, %rbx
	testq	%rbx, %rbx
	jle	LBB7_14
## %bb.13:
	movq	%r13, %rdi
	movq	%rbx, %rdx
	callq	_memcpy
	addq	%rbx, %r13
	movq	%r13, 32(%r12)
LBB7_14:
	addq	$8, %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
LBB7_2:
Ltmp18:
	movq	%r12, %rdi
	callq	__ZNKSt3__120__vector_base_commonILb1EE20__throw_length_errorEv
Ltmp19:
	jmp	LBB7_3
LBB7_9:
Ltmp23:
	movq	%r15, %rdi
	callq	__ZNKSt3__120__vector_base_commonILb1EE20__throw_length_errorEv
Ltmp24:
LBB7_3:
	ud2
LBB7_15:
Ltmp25:
	movq	%rax, %r14
	movq	(%r15), %rdi
	testq	%rdi, %rdi
	jne	LBB7_16
## %bb.17:
	movq	(%r12), %rdi
	testq	%rdi, %rdi
	jne	LBB7_18
LBB7_19:
	movq	%r14, %rdi
	callq	__Unwind_Resume
	ud2
LBB7_16:
	movq	%rdi, 32(%r12)
	callq	__ZdlPv
	movq	(%r12), %rdi
	testq	%rdi, %rdi
	je	LBB7_19
	jmp	LBB7_18
LBB7_10:
Ltmp20:
	movq	%rax, %r14
	movq	(%r12), %rdi
	testq	%rdi, %rdi
	je	LBB7_19
LBB7_18:
	movq	%rdi, 8(%r12)
	callq	__ZdlPv
	movq	%r14, %rdi
	callq	__Unwind_Resume
	ud2
Lfunc_end2:
	.cfi_endproc
	.section	__TEXT,__gcc_except_tab
	.p2align	2
GCC_except_table7:
Lexception2:
	.byte	255                     ## @LPStart Encoding = omit
	.byte	255                     ## @TType Encoding = omit
	.byte	1                       ## Call site Encoding = uleb128
	.uleb128 Lcst_end2-Lcst_begin2
Lcst_begin2:
	.uleb128 Ltmp16-Lfunc_begin2    ## >> Call Site 1 <<
	.uleb128 Ltmp17-Ltmp16          ##   Call between Ltmp16 and Ltmp17
	.uleb128 Ltmp20-Lfunc_begin2    ##     jumps to Ltmp20
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp17-Lfunc_begin2    ## >> Call Site 2 <<
	.uleb128 Ltmp21-Ltmp17          ##   Call between Ltmp17 and Ltmp21
	.byte	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp21-Lfunc_begin2    ## >> Call Site 3 <<
	.uleb128 Ltmp22-Ltmp21          ##   Call between Ltmp21 and Ltmp22
	.uleb128 Ltmp25-Lfunc_begin2    ##     jumps to Ltmp25
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp22-Lfunc_begin2    ## >> Call Site 4 <<
	.uleb128 Ltmp18-Ltmp22          ##   Call between Ltmp22 and Ltmp18
	.byte	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp18-Lfunc_begin2    ## >> Call Site 5 <<
	.uleb128 Ltmp19-Ltmp18          ##   Call between Ltmp18 and Ltmp19
	.uleb128 Ltmp20-Lfunc_begin2    ##     jumps to Ltmp20
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp23-Lfunc_begin2    ## >> Call Site 6 <<
	.uleb128 Ltmp24-Ltmp23          ##   Call between Ltmp23 and Ltmp24
	.uleb128 Ltmp25-Lfunc_begin2    ##     jumps to Ltmp25
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp24-Lfunc_begin2    ## >> Call Site 7 <<
	.uleb128 Lfunc_end2-Ltmp24      ##   Call between Ltmp24 and Lfunc_end2
	.byte	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
Lcst_end2:
	.p2align	2
                                        ## -- End function
	.section	__TEXT,__text,regular,pure_instructions
	.globl	__ZNSt3__16__treeINS_12__value_typeINS_4pairImmEENS2_INS_6vectorIiNS_9allocatorIiEEEES7_EEEENS_19__map_value_compareIS3_S9_NS_4lessIS3_EELb1EEENS5_IS9_EEE30__emplace_hint_unique_key_argsIS3_JNS2_IKS3_S8_EEEEENS_15__tree_iteratorIS9_PNS_11__tree_nodeIS9_PvEElEENS_21__tree_const_iteratorIS9_SN_lEERKT_DpOT0_ ## -- Begin function _ZNSt3__16__treeINS_12__value_typeINS_4pairImmEENS2_INS_6vectorIiNS_9allocatorIiEEEES7_EEEENS_19__map_value_compareIS3_S9_NS_4lessIS3_EELb1EEENS5_IS9_EEE30__emplace_hint_unique_key_argsIS3_JNS2_IKS3_S8_EEEEENS_15__tree_iteratorIS9_PNS_11__tree_nodeIS9_PvEElEENS_21__tree_const_iteratorIS9_SN_lEERKT_DpOT0_
	.weak_def_can_be_hidden	__ZNSt3__16__treeINS_12__value_typeINS_4pairImmEENS2_INS_6vectorIiNS_9allocatorIiEEEES7_EEEENS_19__map_value_compareIS3_S9_NS_4lessIS3_EELb1EEENS5_IS9_EEE30__emplace_hint_unique_key_argsIS3_JNS2_IKS3_S8_EEEEENS_15__tree_iteratorIS9_PNS_11__tree_nodeIS9_PvEElEENS_21__tree_const_iteratorIS9_SN_lEERKT_DpOT0_
	.p2align	4, 0x90
__ZNSt3__16__treeINS_12__value_typeINS_4pairImmEENS2_INS_6vectorIiNS_9allocatorIiEEEES7_EEEENS_19__map_value_compareIS3_S9_NS_4lessIS3_EELb1EEENS5_IS9_EEE30__emplace_hint_unique_key_argsIS3_JNS2_IKS3_S8_EEEEENS_15__tree_iteratorIS9_PNS_11__tree_nodeIS9_PvEElEENS_21__tree_const_iteratorIS9_SN_lEERKT_DpOT0_: ## @_ZNSt3__16__treeINS_12__value_typeINS_4pairImmEENS2_INS_6vectorIiNS_9allocatorIiEEEES7_EEEENS_19__map_value_compareIS3_S9_NS_4lessIS3_EELb1EEENS5_IS9_EEE30__emplace_hint_unique_key_argsIS3_JNS2_IKS3_S8_EEEEENS_15__tree_iteratorIS9_PNS_11__tree_nodeIS9_PvEElEENS_21__tree_const_iteratorIS9_SN_lEERKT_DpOT0_
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r15
	pushq	%r14
	pushq	%r12
	pushq	%rbx
	subq	$16, %rsp
	.cfi_offset %rbx, -48
	.cfi_offset %r12, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	movq	%rcx, %r15
	movq	%rdx, %rax
	movq	%rdi, %r14
	leaq	-40(%rbp), %rdx
	leaq	-48(%rbp), %rcx
	movq	%rax, %r8
	callq	__ZNSt3__16__treeINS_12__value_typeINS_4pairImmEENS2_INS_6vectorIiNS_9allocatorIiEEEES7_EEEENS_19__map_value_compareIS3_S9_NS_4lessIS3_EELb1EEENS5_IS9_EEE12__find_equalIS3_EERPNS_16__tree_node_baseIPvEENS_21__tree_const_iteratorIS9_PNS_11__tree_nodeIS9_SI_EElEERPNS_15__tree_end_nodeISK_EESL_RKT_
	movq	(%rax), %rbx
	testq	%rbx, %rbx
	jne	LBB8_5
## %bb.1:
	movq	%rax, %r12
	movl	$96, %edi
	callq	__Znwm
	movq	%rax, %rbx
	movq	(%r15), %rax
	movq	8(%r15), %rcx
	movq	%rcx, 40(%rbx)
	movq	%rax, 32(%rbx)
	movups	16(%r15), %xmm0
	movups	%xmm0, 48(%rbx)
	movq	32(%r15), %rax
	movq	%rax, 64(%rbx)
	movq	$0, 32(%r15)
	movq	$0, 24(%r15)
	movq	$0, 16(%r15)
	movups	40(%r15), %xmm0
	movups	%xmm0, 72(%rbx)
	movq	56(%r15), %rax
	movq	%rax, 88(%rbx)
	movq	$0, 56(%r15)
	movq	$0, 48(%r15)
	movq	$0, 40(%r15)
	movq	-40(%rbp), %rax
	movq	$0, 8(%rbx)
	movq	$0, (%rbx)
	movq	%rax, 16(%rbx)
	movq	%rbx, (%r12)
	movq	(%r14), %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	je	LBB8_2
## %bb.3:
	movq	%rax, (%r14)
	movq	(%r12), %rsi
	jmp	LBB8_4
LBB8_2:
	movq	%rbx, %rsi
LBB8_4:
	movq	8(%r14), %rdi
	callq	__ZNSt3__127__tree_balance_after_insertIPNS_16__tree_node_baseIPvEEEEvT_S5_
	incq	16(%r14)
LBB8_5:
	movq	%rbx, %rax
	addq	$16, %rsp
	popq	%rbx
	popq	%r12
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	__ZNSt3__16__treeINS_12__value_typeINS_4pairImmEENS2_INS_6vectorIiNS_9allocatorIiEEEES7_EEEENS_19__map_value_compareIS3_S9_NS_4lessIS3_EELb1EEENS5_IS9_EEE12__find_equalIS3_EERPNS_16__tree_node_baseIPvEENS_21__tree_const_iteratorIS9_PNS_11__tree_nodeIS9_SI_EElEERPNS_15__tree_end_nodeISK_EESL_RKT_ ## -- Begin function _ZNSt3__16__treeINS_12__value_typeINS_4pairImmEENS2_INS_6vectorIiNS_9allocatorIiEEEES7_EEEENS_19__map_value_compareIS3_S9_NS_4lessIS3_EELb1EEENS5_IS9_EEE12__find_equalIS3_EERPNS_16__tree_node_baseIPvEENS_21__tree_const_iteratorIS9_PNS_11__tree_nodeIS9_SI_EElEERPNS_15__tree_end_nodeISK_EESL_RKT_
	.weak_def_can_be_hidden	__ZNSt3__16__treeINS_12__value_typeINS_4pairImmEENS2_INS_6vectorIiNS_9allocatorIiEEEES7_EEEENS_19__map_value_compareIS3_S9_NS_4lessIS3_EELb1EEENS5_IS9_EEE12__find_equalIS3_EERPNS_16__tree_node_baseIPvEENS_21__tree_const_iteratorIS9_PNS_11__tree_nodeIS9_SI_EElEERPNS_15__tree_end_nodeISK_EESL_RKT_
	.p2align	4, 0x90
__ZNSt3__16__treeINS_12__value_typeINS_4pairImmEENS2_INS_6vectorIiNS_9allocatorIiEEEES7_EEEENS_19__map_value_compareIS3_S9_NS_4lessIS3_EELb1EEENS5_IS9_EEE12__find_equalIS3_EERPNS_16__tree_node_baseIPvEENS_21__tree_const_iteratorIS9_PNS_11__tree_nodeIS9_SI_EElEERPNS_15__tree_end_nodeISK_EESL_RKT_: ## @_ZNSt3__16__treeINS_12__value_typeINS_4pairImmEENS2_INS_6vectorIiNS_9allocatorIiEEEES7_EEEENS_19__map_value_compareIS3_S9_NS_4lessIS3_EELb1EEENS5_IS9_EEE12__find_equalIS3_EERPNS_16__tree_node_baseIPvEENS_21__tree_const_iteratorIS9_PNS_11__tree_nodeIS9_SI_EElEERPNS_15__tree_end_nodeISK_EESL_RKT_
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%rbx
	.cfi_offset %rbx, -24
	leaq	8(%rdi), %rax
	cmpq	%rsi, %rax
	je	LBB9_4
## %bb.1:
	movq	(%r8), %r9
	movq	32(%rsi), %r11
	cmpq	%r9, %r11
	ja	LBB9_4
## %bb.2:
	movq	8(%r8), %r10
	movq	40(%rsi), %rbx
	jb	LBB9_29
## %bb.3:
	cmpq	%r10, %rbx
	jbe	LBB9_29
LBB9_4:
	cmpq	%rsi, (%rdi)
	movq	(%rsi), %r9
	je	LBB9_5
## %bb.6:
	testq	%r9, %r9
	je	LBB9_7
## %bb.9:
	movq	%r9, %rcx
	.p2align	4, 0x90
LBB9_10:                                ## =>This Inner Loop Header: Depth=1
	movq	%rcx, %rbx
	movq	8(%rcx), %rcx
	testq	%rcx, %rcx
	jne	LBB9_10
	jmp	LBB9_11
LBB9_5:
	movq	%rsi, %rbx
	testq	%r9, %r9
	jne	LBB9_16
	jmp	LBB9_15
LBB9_7:
	movq	%rsi, %rbx
	.p2align	4, 0x90
LBB9_8:                                 ## =>This Inner Loop Header: Depth=1
	movq	%rbx, %rcx
	movq	16(%rbx), %rbx
	cmpq	%rcx, (%rbx)
	je	LBB9_8
LBB9_11:
	movq	(%r8), %r10
	cmpq	%r10, 32(%rbx)
	jb	LBB9_14
## %bb.12:
	movq	8(%r8), %r8
	ja	LBB9_17
## %bb.13:
	cmpq	%r8, 40(%rbx)
	jae	LBB9_17
LBB9_14:
	testq	%r9, %r9
	je	LBB9_15
LBB9_16:
	movq	%rbx, (%rdx)
	addq	$8, %rbx
	movq	%rbx, %rax
	popq	%rbx
	popq	%rbp
	retq
LBB9_29:
	cmpq	%r9, %r11
	jb	LBB9_31
## %bb.30:
	cmpq	%r10, %rbx
	jb	LBB9_31
## %bb.55:
	movq	%rsi, (%rdx)
	movq	%rsi, (%rcx)
	movq	%rcx, %rax
	popq	%rbx
	popq	%rbp
	retq
LBB9_17:
	movq	(%rax), %rsi
	testq	%rsi, %rsi
	je	LBB9_28
## %bb.18:
	addq	$8, %rdi
	movq	%rdi, %rax
	movq	32(%rsi), %rcx
	cmpq	%r10, %rcx
	ja	LBB9_22
	jmp	LBB9_20
	.p2align	4, 0x90
LBB9_23:
	movq	%rcx, %rsi
	movq	32(%rsi), %rcx
	cmpq	%r10, %rcx
	ja	LBB9_22
LBB9_20:
	movq	40(%rsi), %rdi
	jb	LBB9_24
## %bb.21:
	cmpq	%r8, %rdi
	jbe	LBB9_24
LBB9_22:
	movq	(%rsi), %rcx
	movq	%rsi, %rax
	testq	%rcx, %rcx
	jne	LBB9_23
	jmp	LBB9_15
	.p2align	4, 0x90
LBB9_24:
	cmpq	%r10, %rcx
	jb	LBB9_26
## %bb.25:
	cmpq	%r8, %rdi
	jae	LBB9_27
LBB9_26:
	leaq	8(%rsi), %rax
	movq	8(%rsi), %rcx
	testq	%rcx, %rcx
	jne	LBB9_23
LBB9_27:
	movq	%rsi, (%rdx)
	popq	%rbx
	popq	%rbp
	retq
LBB9_15:
	movq	%rsi, (%rdx)
	movq	%rsi, %rax
	popq	%rbx
	popq	%rbp
	retq
LBB9_31:
	movq	8(%rsi), %r8
	testq	%r8, %r8
	je	LBB9_34
## %bb.32:
	movq	%r8, %rdi
	.p2align	4, 0x90
LBB9_33:                                ## =>This Inner Loop Header: Depth=1
	movq	%rdi, %rcx
	movq	(%rdi), %rdi
	testq	%rdi, %rdi
	jne	LBB9_33
	jmp	LBB9_37
LBB9_28:
	movq	%rax, (%rdx)
	popq	%rbx
	popq	%rbp
	retq
LBB9_34:
	movq	16(%rsi), %rcx
	cmpq	%rsi, (%rcx)
	je	LBB9_37
## %bb.35:
	leaq	16(%rsi), %rbx
	.p2align	4, 0x90
LBB9_36:                                ## =>This Inner Loop Header: Depth=1
	movq	(%rbx), %rdi
	movq	16(%rdi), %rcx
	cmpq	%rdi, (%rcx)
	leaq	16(%rdi), %rbx
	jne	LBB9_36
LBB9_37:
	cmpq	%rax, %rcx
	je	LBB9_41
## %bb.38:
	cmpq	%r9, 32(%rcx)
	ja	LBB9_41
## %bb.39:
	jb	LBB9_43
## %bb.40:
	cmpq	%r10, 40(%rcx)
	jbe	LBB9_43
LBB9_41:
	testq	%r8, %r8
	je	LBB9_42
LBB9_48:
	movq	%rcx, (%rdx)
	movq	%rcx, %rax
	popq	%rbx
	popq	%rbp
	retq
LBB9_42:
	movq	%rsi, (%rdx)
	addq	$8, %rsi
	movq	%rsi, %rax
	popq	%rbx
	popq	%rbp
	retq
LBB9_43:
	movq	(%rax), %rcx
	testq	%rcx, %rcx
	jne	LBB9_44
## %bb.54:
	movq	%rax, (%rdx)
	popq	%rbx
	popq	%rbp
	retq
LBB9_53:                                ##   in Loop: Header=BB9_44 Depth=1
	movq	%rsi, %rcx
LBB9_44:                                ## =>This Inner Loop Header: Depth=1
	movq	32(%rcx), %rsi
	cmpq	%r9, %rsi
	ja	LBB9_47
## %bb.45:                              ##   in Loop: Header=BB9_44 Depth=1
	movq	40(%rcx), %rdi
	jb	LBB9_49
## %bb.46:                              ##   in Loop: Header=BB9_44 Depth=1
	cmpq	%r10, %rdi
	jbe	LBB9_49
LBB9_47:                                ##   in Loop: Header=BB9_44 Depth=1
	movq	(%rcx), %rsi
	movq	%rcx, %rax
	testq	%rsi, %rsi
	jne	LBB9_53
	jmp	LBB9_48
LBB9_49:                                ##   in Loop: Header=BB9_44 Depth=1
	cmpq	%r9, %rsi
	jb	LBB9_51
## %bb.50:                              ##   in Loop: Header=BB9_44 Depth=1
	cmpq	%r10, %rdi
	jae	LBB9_52
LBB9_51:                                ##   in Loop: Header=BB9_44 Depth=1
	leaq	8(%rcx), %rax
	movq	8(%rcx), %rsi
	testq	%rsi, %rsi
	jne	LBB9_53
LBB9_52:
	movq	%rcx, (%rdx)
	popq	%rbx
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	__ZNSt3__127__tree_balance_after_insertIPNS_16__tree_node_baseIPvEEEEvT_S5_ ## -- Begin function _ZNSt3__127__tree_balance_after_insertIPNS_16__tree_node_baseIPvEEEEvT_S5_
	.weak_def_can_be_hidden	__ZNSt3__127__tree_balance_after_insertIPNS_16__tree_node_baseIPvEEEEvT_S5_
	.p2align	4, 0x90
__ZNSt3__127__tree_balance_after_insertIPNS_16__tree_node_baseIPvEEEEvT_S5_: ## @_ZNSt3__127__tree_balance_after_insertIPNS_16__tree_node_baseIPvEEEEvT_S5_
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	cmpq	%rdi, %rsi
	sete	24(%rsi)
	je	LBB10_23
	.p2align	4, 0x90
LBB10_1:                                ## =>This Inner Loop Header: Depth=1
	movq	16(%rsi), %rdx
	cmpb	$0, 24(%rdx)
	jne	LBB10_23
## %bb.2:                               ##   in Loop: Header=BB10_1 Depth=1
	leaq	24(%rdx), %r8
	movq	16(%rdx), %rax
	movq	(%rax), %rcx
	cmpq	%rdx, %rcx
	je	LBB10_3
## %bb.13:                              ##   in Loop: Header=BB10_1 Depth=1
	testq	%rcx, %rcx
	je	LBB10_15
## %bb.14:                              ##   in Loop: Header=BB10_1 Depth=1
	cmpb	$0, 24(%rcx)
	je	LBB10_5
	jmp	LBB10_15
	.p2align	4, 0x90
LBB10_3:                                ##   in Loop: Header=BB10_1 Depth=1
	movq	8(%rax), %rcx
	testq	%rcx, %rcx
	je	LBB10_6
## %bb.4:                               ##   in Loop: Header=BB10_1 Depth=1
	cmpb	$0, 24(%rcx)
	jne	LBB10_6
LBB10_5:                                ##   in Loop: Header=BB10_1 Depth=1
	addq	$24, %rcx
	movb	$1, (%r8)
	movq	%rax, %rsi
	cmpq	%rdi, %rax
	sete	24(%rax)
	movb	$1, (%rcx)
	jne	LBB10_1
LBB10_23:
	popq	%rbp
	retq
LBB10_15:
	cmpq	%rsi, (%rdx)
	jne	LBB10_19
## %bb.16:
	movq	8(%rsi), %rcx
	movq	%rcx, (%rdx)
	testq	%rcx, %rcx
	je	LBB10_18
## %bb.17:
	movq	%rdx, 16(%rcx)
	movq	16(%rdx), %rax
LBB10_18:
	movq	%rax, 16(%rsi)
	movq	16(%rdx), %rax
	xorl	%ecx, %ecx
	cmpq	%rdx, (%rax)
	setne	%cl
	movq	%rsi, (%rax,%rcx,8)
	movq	%rdx, 8(%rsi)
	movq	%rsi, 16(%rdx)
	movq	16(%rsi), %rax
	addq	$24, %rsi
	movq	%rsi, %r8
LBB10_19:
	movb	$1, (%r8)
	movb	$0, 24(%rax)
	movq	8(%rax), %rcx
	movq	(%rcx), %rdx
	movq	%rdx, 8(%rax)
	testq	%rdx, %rdx
	je	LBB10_21
## %bb.20:
	movq	%rax, 16(%rdx)
LBB10_21:
	movq	16(%rax), %rdx
	movq	%rdx, 16(%rcx)
	movq	16(%rax), %rdx
	xorl	%esi, %esi
	cmpq	%rax, (%rdx)
	setne	%sil
	movq	%rcx, (%rdx,%rsi,8)
	movq	%rax, (%rcx)
	movq	%rcx, 16(%rax)
	popq	%rbp
	retq
LBB10_6:
	cmpq	%rsi, (%rdx)
	je	LBB10_10
## %bb.7:
	movq	8(%rdx), %r8
	movq	(%r8), %rcx
	movq	%rcx, 8(%rdx)
	testq	%rcx, %rcx
	je	LBB10_9
## %bb.8:
	movq	%rdx, 16(%rcx)
	movq	16(%rdx), %rax
LBB10_9:
	movq	%rax, 16(%r8)
	movq	16(%rdx), %rax
	xorl	%ecx, %ecx
	cmpq	%rdx, (%rax)
	setne	%cl
	movq	%r8, (%rax,%rcx,8)
	movq	%rdx, (%r8)
	movq	%r8, 16(%rdx)
	movq	16(%r8), %rax
	addq	$24, %r8
LBB10_10:
	movb	$1, (%r8)
	movb	$0, 24(%rax)
	movq	(%rax), %rcx
	movq	8(%rcx), %rdx
	movq	%rdx, (%rax)
	testq	%rdx, %rdx
	je	LBB10_12
## %bb.11:
	movq	%rax, 16(%rdx)
LBB10_12:
	movq	16(%rax), %rdx
	movq	%rdx, 16(%rcx)
	movq	16(%rax), %rdx
	xorl	%esi, %esi
	cmpq	%rax, (%rdx)
	setne	%sil
	movq	%rcx, (%rdx,%rsi,8)
	movq	%rax, 8(%rcx)
	movq	%rcx, 16(%rax)
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90         ## -- Begin function _ZN5bench6detail22generate_sorted_vectorIiZNS0_11uniform_srcEmE3$_0EEDamT0_
__ZN5bench6detail22generate_sorted_vectorIiZNS0_11uniform_srcEmE3$_0EEDamT0_: ## @"_ZN5bench6detail22generate_sorted_vectorIiZNS0_11uniform_srcEmE3$_0EEDamT0_"
Lfunc_begin3:
	.cfi_startproc
	.cfi_personality 155, ___gxx_personality_v0
	.cfi_lsda 16, Lexception3
## %bb.0:
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
	subq	$56, %rsp
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	movq	%rdi, %r14
	movq	%rdx, -96(%rbp)
	xorps	%xmm0, %xmm0
	movaps	%xmm0, -80(%rbp)
	movq	$0, -64(%rbp)
	testq	%rsi, %rsi
	je	LBB11_1
## %bb.2:
	movq	%rsi, %r15
	movq	%rsi, %rax
	shrq	$62, %rax
	jne	LBB11_3
## %bb.7:
	leaq	(,%r15,4), %r12
Ltmp26:
	movq	%r12, %rdi
	callq	__Znwm
Ltmp27:
## %bb.8:
	movq	%rax, %rbx
	leaq	-72(%rbp), %r13
	movq	%rax, -80(%rbp)
	leaq	(%rax,%r15,4), %r15
	movq	%r15, -64(%rbp)
	movq	%rax, %rdi
	movq	%r12, %rsi
	callq	___bzero
	movq	%r15, -72(%rbp)
	testq	%r12, %r12
	je	LBB11_18
## %bb.9:
	movq	%r13, -88(%rbp)         ## 8-byte Spill
	leaq	__ZZN5bench6detail16static_generatorEvE1g(%rip), %r13
	leaq	-96(%rbp), %r12
	jmp	LBB11_10
LBB11_1:
	leaq	-72(%rbp), %r13
	xorl	%r15d, %r15d
	xorl	%ebx, %ebx
	jmp	LBB11_18
LBB11_11:                               ##   in Loop: Header=BB11_10 Depth=1
	leaq	__ZGVZN5bench6detail16static_generatorEvE1g(%rip), %rdi
	callq	___cxa_guard_acquire
	testl	%eax, %eax
	je	LBB11_15
## %bb.12:                              ##   in Loop: Header=BB11_10 Depth=1
	movl	$5489, __ZZN5bench6detail16static_generatorEvE1g(%rip) ## imm = 0x1571
	movl	$5489, %edx             ## imm = 0x1571
	movl	$2, %eax
	movl	$1, %ecx
	jmp	LBB11_13
LBB11_26:                               ##   in Loop: Header=BB11_13 Depth=2
	leaq	1(%rcx), %rdx
	movl	%esi, %edi
	shrl	$30, %edi
	xorl	%esi, %edi
	imull	$1812433253, %edi, %esi ## imm = 0x6C078965
	addl	%esi, %edx
	addl	%eax, %esi
	movl	%esi, (%r13,%rax,4)
	addq	$2, %rcx
	addq	$2, %rax
LBB11_13:                               ##   Parent Loop BB11_10 Depth=1
                                        ## =>  This Inner Loop Header: Depth=2
	movl	%edx, %esi
	shrl	$30, %esi
	xorl	%edx, %esi
	imull	$1812433253, %esi, %esi ## imm = 0x6C078965
	addl	%ecx, %esi
	movl	%esi, -4(%r13,%rax,4)
	cmpq	$624, %rax              ## imm = 0x270
	jne	LBB11_26
## %bb.14:                              ##   in Loop: Header=BB11_10 Depth=1
	movq	$0, __ZZN5bench6detail16static_generatorEvE1g+2496(%rip)
	leaq	__ZGVZN5bench6detail16static_generatorEvE1g(%rip), %rdi
	callq	___cxa_guard_release
	jmp	LBB11_15
	.p2align	4, 0x90
LBB11_10:                               ## =>This Loop Header: Depth=1
                                        ##     Child Loop BB11_13 Depth 2
	movb	__ZGVZN5bench6detail16static_generatorEvE1g(%rip), %al
	testb	%al, %al
	je	LBB11_11
LBB11_15:                               ##   in Loop: Header=BB11_10 Depth=1
Ltmp28:
	movq	%r12, %rdi
	movq	%r13, %rsi
	movq	%r12, %rdx
	callq	__ZNSt3__124uniform_int_distributionIiEclINS_23mersenne_twister_engineIjLm32ELm624ELm397ELm31ELj2567483615ELm11ELj4294967295ELm7ELj2636928640ELm15ELj4022730752ELm18ELj1812433253EEEEEiRT_RKNS1_10param_typeE
Ltmp29:
## %bb.16:                              ##   in Loop: Header=BB11_10 Depth=1
	movl	%eax, (%rbx)
	addq	$4, %rbx
	cmpq	%r15, %rbx
	jne	LBB11_10
## %bb.17:
	movq	-80(%rbp), %rbx
	movq	-72(%rbp), %r15
	leaq	-72(%rbp), %r13
LBB11_18:
Ltmp34:
	leaq	-48(%rbp), %rdx
	movq	%rbx, %rdi
	movq	%r15, %rsi
	callq	__ZNSt3__16__sortIRNS_6__lessIiiEEPiEEvT0_S5_T_
Ltmp35:
## %bb.19:
	movq	-80(%rbp), %rax
	movq	%rax, (%r14)
	movq	(%r13), %rax
	movq	%rax, 8(%r14)
	movq	-64(%rbp), %rax
	movq	%rax, 16(%r14)
	addq	$56, %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
LBB11_3:
Ltmp31:
	leaq	-80(%rbp), %rdi
	callq	__ZNKSt3__120__vector_base_commonILb1EE20__throw_length_errorEv
Ltmp32:
## %bb.4:
	ud2
LBB11_5:
Ltmp33:
	movq	%rax, %rbx
	movq	-80(%rbp), %rdi
	testq	%rdi, %rdi
	je	LBB11_25
## %bb.6:
	movq	%rdi, -72(%rbp)
	jmp	LBB11_24
LBB11_21:
Ltmp36:
	movq	%r13, -88(%rbp)         ## 8-byte Spill
	movq	%rax, %rbx
	movq	-80(%rbp), %rdi
	testq	%rdi, %rdi
	jne	LBB11_23
	jmp	LBB11_25
LBB11_20:
Ltmp30:
	movq	%rax, %rbx
	movq	-80(%rbp), %rdi
	testq	%rdi, %rdi
	je	LBB11_25
LBB11_23:
	movq	-88(%rbp), %rax         ## 8-byte Reload
	movq	%rdi, (%rax)
LBB11_24:
	callq	__ZdlPv
LBB11_25:
	movq	%rbx, %rdi
	callq	__Unwind_Resume
	ud2
Lfunc_end3:
	.cfi_endproc
	.section	__TEXT,__gcc_except_tab
	.p2align	2
GCC_except_table11:
Lexception3:
	.byte	255                     ## @LPStart Encoding = omit
	.byte	255                     ## @TType Encoding = omit
	.byte	1                       ## Call site Encoding = uleb128
	.uleb128 Lcst_end3-Lcst_begin3
Lcst_begin3:
	.uleb128 Ltmp26-Lfunc_begin3    ## >> Call Site 1 <<
	.uleb128 Ltmp27-Ltmp26          ##   Call between Ltmp26 and Ltmp27
	.uleb128 Ltmp33-Lfunc_begin3    ##     jumps to Ltmp33
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp27-Lfunc_begin3    ## >> Call Site 2 <<
	.uleb128 Ltmp28-Ltmp27          ##   Call between Ltmp27 and Ltmp28
	.byte	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp28-Lfunc_begin3    ## >> Call Site 3 <<
	.uleb128 Ltmp29-Ltmp28          ##   Call between Ltmp28 and Ltmp29
	.uleb128 Ltmp30-Lfunc_begin3    ##     jumps to Ltmp30
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp34-Lfunc_begin3    ## >> Call Site 4 <<
	.uleb128 Ltmp35-Ltmp34          ##   Call between Ltmp34 and Ltmp35
	.uleb128 Ltmp36-Lfunc_begin3    ##     jumps to Ltmp36
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp31-Lfunc_begin3    ## >> Call Site 5 <<
	.uleb128 Ltmp32-Ltmp31          ##   Call between Ltmp31 and Ltmp32
	.uleb128 Ltmp33-Lfunc_begin3    ##     jumps to Ltmp33
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp32-Lfunc_begin3    ## >> Call Site 6 <<
	.uleb128 Lfunc_end3-Ltmp32      ##   Call between Ltmp32 and Lfunc_end3
	.byte	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
Lcst_end3:
	.p2align	2
                                        ## -- End function
	.section	__TEXT,__text,regular,pure_instructions
	.globl	__ZNSt3__124uniform_int_distributionIiEclINS_23mersenne_twister_engineIjLm32ELm624ELm397ELm31ELj2567483615ELm11ELj4294967295ELm7ELj2636928640ELm15ELj4022730752ELm18ELj1812433253EEEEEiRT_RKNS1_10param_typeE ## -- Begin function _ZNSt3__124uniform_int_distributionIiEclINS_23mersenne_twister_engineIjLm32ELm624ELm397ELm31ELj2567483615ELm11ELj4294967295ELm7ELj2636928640ELm15ELj4022730752ELm18ELj1812433253EEEEEiRT_RKNS1_10param_typeE
	.weak_def_can_be_hidden	__ZNSt3__124uniform_int_distributionIiEclINS_23mersenne_twister_engineIjLm32ELm624ELm397ELm31ELj2567483615ELm11ELj4294967295ELm7ELj2636928640ELm15ELj4022730752ELm18ELj1812433253EEEEEiRT_RKNS1_10param_typeE
	.p2align	4, 0x90
__ZNSt3__124uniform_int_distributionIiEclINS_23mersenne_twister_engineIjLm32ELm624ELm397ELm31ELj2567483615ELm11ELj4294967295ELm7ELj2636928640ELm15ELj4022730752ELm18ELj1812433253EEEEEiRT_RKNS1_10param_typeE: ## @_ZNSt3__124uniform_int_distributionIiEclINS_23mersenne_twister_engineIjLm32ELm624ELm397ELm31ELj2567483615ELm11ELj4294967295ELm7ELj2636928640ELm15ELj4022730752ELm18ELj1812433253EEEEEiRT_RKNS1_10param_typeE
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r15
	pushq	%r14
	pushq	%r12
	pushq	%rbx
	.cfi_offset %rbx, -48
	.cfi_offset %r12, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	movl	(%rdx), %eax
	movl	4(%rdx), %r10d
	subl	%eax, %r10d
	je	LBB12_6
## %bb.1:
	incl	%r10d
	je	LBB12_2
## %bb.3:
	movq	%rdx, %r8
	bsrl	%r10d, %eax
	xorl	$31, %eax
	movl	$32, %r9d
	subq	%rax, %r9
	movl	$33, %ecx
	subl	%r9d, %ecx
	movl	%r10d, %eax
	shll	%cl, %eax
                                        ## kill: def $cl killed $cl killed $ecx
	shrl	%cl, %eax
	cmpl	$1, %eax
	sbbq	$0, %r9
	movq	%r9, %rbx
	shrq	$5, %rbx
	movl	%r9d, %eax
	andl	$31, %eax
	cmpq	$1, %rax
	sbbq	$-1, %rbx
	xorl	%r11d, %r11d
	xorl	%edx, %edx
	movq	%r9, %rax
	divq	%rbx
	movl	$32, %ecx
	subl	%eax, %ecx
	movl	$-1, %r14d
                                        ## kill: def $cl killed $cl killed $ecx
	shrl	%cl, %r14d
	cmpq	%r9, %rbx
	cmoval	%r11d, %r14d
	movq	2496(%rsi), %r15
	movabsq	$945986875574848801, %r11 ## imm = 0xD20D20D20D20D21
	movl	$-2147483648, %r9d      ## imm = 0x80000000
	.p2align	4, 0x90
LBB12_4:                                ## =>This Inner Loop Header: Depth=1
	movq	%r15, %r12
	leaq	1(%r15), %rax
	shrq	$4, %rax
	mulq	%r11
	shrq	%rdx
	imulq	$624, %rdx, %rax        ## imm = 0x270
	negq	%rax
	leaq	1(%r15,%rax), %r15
	movl	(%rsi,%r12,4), %edi
	andl	%r9d, %edi
	movl	(%rsi,%r15,4), %ebx
	movl	%ebx, %ecx
	andl	$2147483646, %ecx       ## imm = 0x7FFFFFFE
	leaq	397(%r12), %rax
	shrq	$4, %rax
	mulq	%r11
	orl	%edi, %ecx
	shrq	%rdx
	imulq	$624, %rdx, %rax        ## imm = 0x270
	negq	%rax
	leaq	397(%r12,%rax), %rax
	shrl	%ecx
	andl	$1, %ebx
	imull	$-1727483681, %ebx, %edx ## imm = 0x9908B0DF
	xorl	(%rsi,%rax,4), %edx
	xorl	%ecx, %edx
	movl	%edx, (%rsi,%r12,4)
	movl	%edx, %eax
	shrl	$11, %eax
	xorl	%edx, %eax
	movl	%eax, %ecx
	shll	$7, %ecx
	andl	$-1658038656, %ecx      ## imm = 0x9D2C5680
	xorl	%eax, %ecx
	movl	%ecx, %edx
	shll	$15, %edx
	andl	$-272236544, %edx       ## imm = 0xEFC60000
	xorl	%ecx, %edx
	movl	%edx, %eax
	shrl	$18, %eax
	xorl	%edx, %eax
	andl	%r14d, %eax
	cmpl	%r10d, %eax
	jae	LBB12_4
## %bb.5:
	movq	%r15, 2496(%rsi)
	addl	(%r8), %eax
	jmp	LBB12_6
LBB12_2:
	movq	2496(%rsi), %rcx
	leaq	1(%rcx), %rax
	shrq	$4, %rax
	movabsq	$945986875574848801, %r8 ## imm = 0xD20D20D20D20D21
	mulq	%r8
	shrq	%rdx
	imulq	$624, %rdx, %rax        ## imm = 0x270
	negq	%rax
	leaq	1(%rcx,%rax), %r9
	movl	$-2147483648, %eax      ## imm = 0x80000000
	andl	(%rsi,%rcx,4), %eax
	movl	(%rsi,%r9,4), %edi
	movl	%edi, %ebx
	andl	$2147483646, %ebx       ## imm = 0x7FFFFFFE
	orl	%eax, %ebx
	leaq	397(%rcx), %rax
	shrq	$4, %rax
	mulq	%r8
	shrq	%rdx
	imulq	$624, %rdx, %rax        ## imm = 0x270
	negq	%rax
	leaq	397(%rcx,%rax), %rax
	shrl	%ebx
	andl	$1, %edi
	imull	$-1727483681, %edi, %edx ## imm = 0x9908B0DF
	xorl	(%rsi,%rax,4), %edx
	xorl	%ebx, %edx
	movl	%edx, (%rsi,%rcx,4)
	movl	%edx, %eax
	shrl	$11, %eax
	xorl	%edx, %eax
	movq	%r9, 2496(%rsi)
	movl	%eax, %ecx
	shll	$7, %ecx
	andl	$-1658038656, %ecx      ## imm = 0x9D2C5680
	xorl	%eax, %ecx
	movl	%ecx, %edx
	shll	$15, %edx
	andl	$-272236544, %edx       ## imm = 0xEFC60000
	xorl	%ecx, %edx
	movl	%edx, %eax
	shrl	$18, %eax
	xorl	%edx, %eax
LBB12_6:
	popq	%rbx
	popq	%r12
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__StaticInit,regular,pure_instructions
	.p2align	4, 0x90         ## -- Begin function _GLOBAL__sub_I_code_alignment.cc
__GLOBAL__sub_I_code_alignment.cc:      ## @_GLOBAL__sub_I_code_alignment.cc
Lfunc_begin4:
	.cfi_startproc
	.cfi_personality 155, ___gxx_personality_v0
	.cfi_lsda 16, Lexception4
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r14
	pushq	%rbx
	.cfi_offset %rbx, -32
	.cfi_offset %r14, -24
	callq	__ZN9benchmark8internal17InitializeStreamsEv
	movl	$192, %edi
	callq	__Znwm
	movq	%rax, %rbx
Ltmp37:
	leaq	L_.str(%rip), %rsi
	movq	%rax, %rdi
	callq	__ZN9benchmark8internal9BenchmarkC2EPKc
Ltmp38:
## %bb.1:
	movq	__ZTVN9benchmark8internal17FunctionBenchmarkE@GOTPCREL(%rip), %rax
	addq	$16, %rax
	movq	%rax, (%rbx)
	leaq	__Z14test_benchmarkRN9benchmark5StateE(%rip), %rax
	movq	%rax, 184(%rbx)
	movq	%rbx, %rdi
	callq	__ZN9benchmark8internal25RegisterBenchmarkInternalEPNS0_9BenchmarkE
	xorl	%esi, %esi
	movq	%rax, %rdi
	callq	__ZN9benchmark8internal9Benchmark3ArgEx
	movq	%rax, __ZL26_benchmark_2test_benchmark(%rip)
	popq	%rbx
	popq	%r14
	popq	%rbp
	retq
LBB13_2:
Ltmp39:
	movq	%rax, %r14
	movq	%rbx, %rdi
	callq	__ZdlPv
	movq	%r14, %rdi
	callq	__Unwind_Resume
	ud2
Lfunc_end4:
	.cfi_endproc
	.section	__TEXT,__gcc_except_tab
	.p2align	2
GCC_except_table13:
Lexception4:
	.byte	255                     ## @LPStart Encoding = omit
	.byte	255                     ## @TType Encoding = omit
	.byte	1                       ## Call site Encoding = uleb128
	.uleb128 Lcst_end4-Lcst_begin4
Lcst_begin4:
	.uleb128 Lfunc_begin4-Lfunc_begin4 ## >> Call Site 1 <<
	.uleb128 Ltmp37-Lfunc_begin4    ##   Call between Lfunc_begin4 and Ltmp37
	.byte	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp37-Lfunc_begin4    ## >> Call Site 2 <<
	.uleb128 Ltmp38-Ltmp37          ##   Call between Ltmp37 and Ltmp38
	.uleb128 Ltmp39-Lfunc_begin4    ##     jumps to Ltmp39
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp38-Lfunc_begin4    ## >> Call Site 3 <<
	.uleb128 Lfunc_end4-Ltmp38      ##   Call between Ltmp38 and Lfunc_end4
	.byte	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
Lcst_end4:
	.p2align	2
                                        ## -- End function
.zerofill __DATA,__bss,__ZZN5bench6detail16static_generatorEvE1g,2504,3 ## @_ZZN5bench6detail16static_generatorEvE1g
.zerofill __DATA,__bss,__ZGVZN5bench6detail16static_generatorEvE1g,8,3 ## @_ZGVZN5bench6detail16static_generatorEvE1g
.zerofill __DATA,__bss,__ZL26_benchmark_2test_benchmark,8,3 ## @_ZL26_benchmark_2test_benchmark
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"test_benchmark"

	.section	__DATA,__data
	.globl	__ZZN5bench18two_sorted_vectorsIiEENSt3__14pairINS1_6vectorIT_NS1_9allocatorIS4_EEEES7_EEmmE3gen ## @_ZZN5bench18two_sorted_vectorsIiEENSt3__14pairINS1_6vectorIT_NS1_9allocatorIS4_EEEES7_EEmmE3gen
	.weak_definition	__ZZN5bench18two_sorted_vectorsIiEENSt3__14pairINS1_6vectorIT_NS1_9allocatorIS4_EEEES7_EEmmE3gen
	.p2align	3
__ZZN5bench18two_sorted_vectorsIiEENSt3__14pairINS1_6vectorIT_NS1_9allocatorIS4_EEEES7_EEmmE3gen:
	.space	32

	.globl	__ZGVZN5bench18two_sorted_vectorsIiEENSt3__14pairINS1_6vectorIT_NS1_9allocatorIS4_EEEES7_EEmmE3gen ## @_ZGVZN5bench18two_sorted_vectorsIiEENSt3__14pairINS1_6vectorIT_NS1_9allocatorIS4_EEEES7_EEmmE3gen
	.weak_definition	__ZGVZN5bench18two_sorted_vectorsIiEENSt3__14pairINS1_6vectorIT_NS1_9allocatorIS4_EEEES7_EEmmE3gen
	.p2align	3
__ZGVZN5bench18two_sorted_vectorsIiEENSt3__14pairINS1_6vectorIT_NS1_9allocatorIS4_EEEES7_EEmmE3gen:
	.quad	0                       ## 0x0

	.section	__DATA,__mod_init_func,mod_init_funcs
	.p2align	3
	.quad	__GLOBAL__sub_I_code_alignment.cc

.subsections_via_symbols
