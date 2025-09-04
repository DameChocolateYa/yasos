.extern malloc
.globl str_upper
.globl str_lower
str_upper:
  push %rbp
  mov %rsp, %rbp
  sub $64, %rsp
  movq %rdi, %rax
  movq %rax, 8(%rsp)
  movq 8(%rsp), %rdi
  mov $0, %al
  call strlen@PLT
  movl %eax, 16(%rsp)
  lea str_0(%rip), %rax
  movq %rax, 24(%rsp)
  movl $0, %eax
  movl %eax, 32(%rsp)
.for_1_start:
  movl 16(%rsp), %ebx
  movl %ebx, 40(%rsp)
  movl 32(%rsp), %eax
  movl 40(%rsp), %ebx
  cmp %rbx, %rax
  setl %al
  movzx %al, %rax
  cmp $0, %rax
  je .for_1_end
  movq 8(%rsp), %rdi
  movl 32(%rsp), %esi
  movl $1, 40(%rsp)
  movl 32(%rsp), %eax
  movl 40(%rsp), %ebx
  addl %ebx, %eax
  mov %rax, %rdx
  mov $0, %al
  call strsub@PLT
  movq %rax, 40(%rsp)
  movq 40(%rsp), %rdi
  lea str_1(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_0_0
  movq 24(%rsp), %rdi
  lea str_2(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
elif_0_0:
  movq 40(%rsp), %rdi
  lea str_3(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_0_1
  movq 24(%rsp), %rdi
  lea str_4(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
elif_0_1:
  movq 40(%rsp), %rdi
  lea str_5(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_0_2
  movq 24(%rsp), %rdi
  lea str_6(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
elif_0_2:
  movq 40(%rsp), %rdi
  lea str_7(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_0_3
  movq 24(%rsp), %rdi
  lea str_8(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
elif_0_3:
  movq 40(%rsp), %rdi
  lea str_9(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_0_4
  movq 24(%rsp), %rdi
  lea str_10(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
elif_0_4:
  movq 40(%rsp), %rdi
  lea str_11(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_0_5
  movq 24(%rsp), %rdi
  lea str_12(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
elif_0_5:
  movq 40(%rsp), %rdi
  lea str_13(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_0_6
  movq 24(%rsp), %rdi
  lea str_14(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
elif_0_6:
  movq 40(%rsp), %rdi
  lea str_15(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_0_7
  movq 24(%rsp), %rdi
  lea str_16(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
elif_0_7:
  movq 40(%rsp), %rdi
  lea str_17(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_0_8
  movq 24(%rsp), %rdi
  lea str_18(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
elif_0_8:
  movq 40(%rsp), %rdi
  lea str_19(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_0_9
  movq 24(%rsp), %rdi
  lea str_20(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
elif_0_9:
  movq 40(%rsp), %rdi
  lea str_21(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_0_10
  movq 24(%rsp), %rdi
  lea str_22(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
elif_0_10:
  movq 40(%rsp), %rdi
  lea str_23(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_0_11
  movq 24(%rsp), %rdi
  lea str_24(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
elif_0_11:
  movq 40(%rsp), %rdi
  lea str_25(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_0_12
  movq 24(%rsp), %rdi
  lea str_26(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
elif_0_12:
  movq 40(%rsp), %rdi
  lea str_27(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_0_13
  movq 24(%rsp), %rdi
  lea str_28(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
elif_0_13:
  movq 40(%rsp), %rdi
  lea str_29(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_0_14
  movq 24(%rsp), %rdi
  lea str_30(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
elif_0_14:
  movq 40(%rsp), %rdi
  lea str_31(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_0_15
  movq 24(%rsp), %rdi
  lea str_32(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
elif_0_15:
  movq 40(%rsp), %rdi
  lea str_33(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_0_16
  movq 24(%rsp), %rdi
  lea str_34(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
elif_0_16:
  movq 40(%rsp), %rdi
  lea str_35(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_0_17
  movq 24(%rsp), %rdi
  lea str_36(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
elif_0_17:
  movq 40(%rsp), %rdi
  lea str_37(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_0_18
  movq 24(%rsp), %rdi
  lea str_38(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
elif_0_18:
  movq 40(%rsp), %rdi
  lea str_39(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_0_19
  movq 24(%rsp), %rdi
  lea str_40(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
elif_0_19:
  movq 40(%rsp), %rdi
  lea str_41(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_0_20
  movq 24(%rsp), %rdi
  lea str_42(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
elif_0_20:
  movq 40(%rsp), %rdi
  lea str_43(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_0_21
  movq 24(%rsp), %rdi
  lea str_44(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
elif_0_21:
  movq 40(%rsp), %rdi
  lea str_45(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_0_22
  movq 24(%rsp), %rdi
  lea str_46(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
elif_0_22:
  movq 40(%rsp), %rdi
  lea str_47(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_0_23
  movq 24(%rsp), %rdi
  lea str_48(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
elif_0_23:
  movq 40(%rsp), %rdi
  lea str_49(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_0_24
  movq 24(%rsp), %rdi
  lea str_50(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
elif_0_24:
  movq 40(%rsp), %rdi
  lea str_51(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_0_25
  movq 24(%rsp), %rdi
  lea str_52(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
elif_0_25:
  movq 40(%rsp), %rdi
  lea str_53(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je else_0
  movq 24(%rsp), %rdi
  lea str_54(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_0
  jmp endif_0
else_0:
  movq 24(%rsp), %rdi
  movq 40(%rsp), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
endif_0:
.for_1_update:
  movl $1, 48(%rsp)
  movl 32(%rsp), %eax
  movl 48(%rsp), %ebx
  addl %ebx, %eax
  movl %eax, 32(%rsp)
  jmp .for_1_start
.for_1_end:
  movq 24(%rsp), %rax
  leave
  ret
  mov %rbp, %rsp
  pop %rbp
  ret
str_lower:
  push %rbp
  mov %rsp, %rbp
  sub $64, %rsp
  movq %rdi, %rax
  movq %rax, 8(%rsp)
  movq 8(%rsp), %rdi
  mov $0, %al
  call strlen@PLT
  movl %eax, 16(%rsp)
  lea str_0(%rip), %rax
  movq %rax, 24(%rsp)
  movl $0, %eax
  movl %eax, 32(%rsp)
.for_2_start:
  movl 16(%rsp), %ebx
  movl %ebx, 40(%rsp)
  movl 32(%rsp), %eax
  movl 40(%rsp), %ebx
  cmp %rbx, %rax
  setl %al
  movzx %al, %rax
  cmp $0, %rax
  je .for_2_end
  movq 8(%rsp), %rdi
  movl 32(%rsp), %esi
  movl $1, 40(%rsp)
  movl 32(%rsp), %eax
  movl 40(%rsp), %ebx
  addl %ebx, %eax
  mov %rax, %rdx
  mov $0, %al
  call strsub@PLT
  movq %rax, 40(%rsp)
  movq 40(%rsp), %rdi
  lea str_2(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_1_0
  movq 24(%rsp), %rdi
  lea str_1(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
elif_1_0:
  movq 40(%rsp), %rdi
  lea str_4(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_1_1
  movq 24(%rsp), %rdi
  lea str_3(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
elif_1_1:
  movq 40(%rsp), %rdi
  lea str_6(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_1_2
  movq 24(%rsp), %rdi
  lea str_5(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
elif_1_2:
  movq 40(%rsp), %rdi
  lea str_8(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_1_3
  movq 24(%rsp), %rdi
  lea str_7(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
elif_1_3:
  movq 40(%rsp), %rdi
  lea str_10(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_1_4
  movq 24(%rsp), %rdi
  lea str_9(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
elif_1_4:
  movq 40(%rsp), %rdi
  lea str_12(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_1_5
  movq 24(%rsp), %rdi
  lea str_11(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
elif_1_5:
  movq 40(%rsp), %rdi
  lea str_14(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_1_6
  movq 24(%rsp), %rdi
  lea str_13(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
elif_1_6:
  movq 40(%rsp), %rdi
  lea str_16(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_1_7
  movq 24(%rsp), %rdi
  lea str_15(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
elif_1_7:
  movq 40(%rsp), %rdi
  lea str_18(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_1_8
  movq 24(%rsp), %rdi
  lea str_17(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
elif_1_8:
  movq 40(%rsp), %rdi
  lea str_20(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_1_9
  movq 24(%rsp), %rdi
  lea str_19(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
elif_1_9:
  movq 40(%rsp), %rdi
  lea str_22(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_1_10
  movq 24(%rsp), %rdi
  lea str_21(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
elif_1_10:
  movq 40(%rsp), %rdi
  lea str_24(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_1_11
  movq 24(%rsp), %rdi
  lea str_23(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
elif_1_11:
  movq 40(%rsp), %rdi
  lea str_26(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_1_12
  movq 24(%rsp), %rdi
  lea str_25(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
elif_1_12:
  movq 40(%rsp), %rdi
  lea str_28(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_1_13
  movq 24(%rsp), %rdi
  lea str_27(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
elif_1_13:
  movq 40(%rsp), %rdi
  lea str_30(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_1_14
  movq 24(%rsp), %rdi
  lea str_29(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
elif_1_14:
  movq 40(%rsp), %rdi
  lea str_32(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_1_15
  movq 24(%rsp), %rdi
  lea str_31(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
elif_1_15:
  movq 40(%rsp), %rdi
  lea str_34(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_1_16
  movq 24(%rsp), %rdi
  lea str_33(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
elif_1_16:
  movq 40(%rsp), %rdi
  lea str_36(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_1_17
  movq 24(%rsp), %rdi
  lea str_35(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
elif_1_17:
  movq 40(%rsp), %rdi
  lea str_38(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_1_18
  movq 24(%rsp), %rdi
  lea str_37(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
elif_1_18:
  movq 40(%rsp), %rdi
  lea str_40(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_1_19
  movq 24(%rsp), %rdi
  lea str_39(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
elif_1_19:
  movq 40(%rsp), %rdi
  lea str_42(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_1_20
  movq 24(%rsp), %rdi
  lea str_41(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
elif_1_20:
  movq 40(%rsp), %rdi
  lea str_44(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_1_21
  movq 24(%rsp), %rdi
  lea str_43(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
elif_1_21:
  movq 40(%rsp), %rdi
  lea str_46(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_1_22
  movq 24(%rsp), %rdi
  lea str_45(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
elif_1_22:
  movq 40(%rsp), %rdi
  lea str_48(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_1_23
  movq 24(%rsp), %rdi
  lea str_47(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
elif_1_23:
  movq 40(%rsp), %rdi
  lea str_50(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_1_24
  movq 24(%rsp), %rdi
  lea str_49(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
elif_1_24:
  movq 40(%rsp), %rdi
  lea str_52(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je elif_1_25
  movq 24(%rsp), %rdi
  lea str_51(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
elif_1_25:
  movq 40(%rsp), %rdi
  lea str_54(%rip), %rsi
  mov $0, %al
  call strcmp@PLT
  cmp $0, %rax
  je else_1
  movq 24(%rsp), %rdi
  lea str_53(%rip), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
  jmp endif_1
  jmp endif_1
else_1:
  movq 24(%rsp), %rdi
  movq 40(%rsp), %rsi
  mov $0, %al
  call strcat@PLT
  movq %rax, %rbx
  movq %rbx, 48(%rsp)
  movq 24(%rsp), %rax
  movq 48(%rsp), %rbx
  movq %rbx, %rax
  movq %rax, 24(%rsp)
endif_1:
.for_2_update:
  movl $1, 48(%rsp)
  movl 32(%rsp), %eax
  movl 48(%rsp), %ebx
  addl %ebx, %eax
  movl %eax, 32(%rsp)
  jmp .for_2_start
.for_2_end:
  movq 24(%rsp), %rax
  leave
  ret
  mov %rbp, %rsp
  pop %rbp
  ret
.section .data
.section .rodata
str_0: .string ""
str_1: .string "a"
str_2: .string "A"
str_3: .string "b"
str_4: .string "B"
str_5: .string "c"
str_6: .string "C"
str_7: .string "d"
str_8: .string "D"
str_9: .string "e"
str_10: .string "E"
str_11: .string "f"
str_12: .string "F"
str_13: .string "g"
str_14: .string "G"
str_15: .string "h"
str_16: .string "H"
str_17: .string "i"
str_18: .string "I"
str_19: .string "j"
str_20: .string "J"
str_21: .string "k"
str_22: .string "K"
str_23: .string "l"
str_24: .string "L"
str_25: .string "m"
str_26: .string "M"
str_27: .string "n"
str_28: .string "N"
str_29: .string "ñ"
str_30: .string "Ñ"
str_31: .string "o"
str_32: .string "O"
str_33: .string "p"
str_34: .string "P"
str_35: .string "q"
str_36: .string "Q"
str_37: .string "r"
str_38: .string "R"
str_39: .string "s"
str_40: .string "S"
str_41: .string "t"
str_42: .string "T"
str_43: .string "u"
str_44: .string "U"
str_45: .string "v"
str_46: .string "V"
str_47: .string "w"
str_48: .string "W"
str_49: .string "x"
str_50: .string "X"
str_51: .string "y"
str_52: .string "Y"
str_53: .string "z"
str_54: .string "Z"
