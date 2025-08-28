.extern malloc
.globl main
main:
  push %rbp
  mov %rsp, %rbp
  sub $0, %rsp
  mov $4, %rbx
  mov $3, %rbx
  mov $2, %rax
  add %rbx, %rax
  imul %rbx, %rax
  push %rax # Creating a new var (x)
  sub $8, %rsp
  lea str_0(%rip), %rdi
  mov 8 (%rsp), %rsi
  mov $0, %al
  call printbp@PLT
  add $8, %rsp
  mov $0, %rax
  mov %rbp, %rsp
  pop %rbp
  ret
  mov %rbp, %rsp
  pop %rbp
  ret
.section .rodata
str_0: .string "%d\n"
.section .data
