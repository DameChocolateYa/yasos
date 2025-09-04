.extern malloc
.globl panic
.globl cuspanic
panic:
  push %rbp
  mov %rsp, %rbp
  sub $16, %rsp
  movq %rdi, %rax
  movq %rax, 8(%rsp)
  lea str_0(%rip), %rdi
  movq 8(%rsp), %rsi
  mov $0, %al
  call println@PLT
  movl $1, %edi
  mov $0, %al
  call exit@PLT
  mov %rbp, %rsp
  pop %rbp
  ret
cuspanic:
  push %rbp
  mov %rsp, %rbp
  sub $32, %rsp
  movq %rdi, %rax
  movq %rax, 8(%rsp)
  movl %esi, %eax
  movl %eax, 16(%rsp)
  lea str_0(%rip), %rdi
  movq 8(%rsp), %rsi
  mov $0, %al
  call println@PLT
  movl 16(%rsp), %edi
  mov $0, %al
  call exit@PLT
  mov %rbp, %rsp
  pop %rbp
  ret
.section .data
.section .rodata
str_0: .string "%s"
