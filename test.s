.globl main
.extern print
sum:
  push %rbp
  mov %rsp, %rbp
  movsd 16(%rbp), %xmm1
  movsd 32(%rbp), %xmm0
  addsd %xmm1, %xmm0
  mov %rbp, %rsp
  pop %rbp
  ret
  mov %rbp, %rsp
  pop %rbp
  ret
start:
  push %rbp
  mov %rsp, %rbp
  movsd float_0(%rip), %xmm0
  sub $16, %rsp
  movsd %xmm0, (%rsp)
  movsd float_1(%rip), %xmm0
  sub $16, %rsp
  movsd %xmm0, (%rsp)
  call sum
  mov (%rsp), %r10
  add $16, %rsp
  mov (%rsp), %r10
  add $16, %rsp
  sub $16, %rsp
  movsd %xmm0, (%rsp) # Creating a new var (z)
  mov $2, %rdi
  mov $0, %rdx
  lea str_0(%rip), %rsi
  call *print@GOTPCREL(%rip)
  movsd 0(%rsp), %xmm0
  mov $1, %rdi
  mov $0, %rdx
  call *print@GOTPCREL(%rip)
  mov $2, %rdi
  mov $3, %rdx
  call *print@GOTPCREL(%rip)
  mov %rbp, %rsp
  pop %rbp
  ret
main:
  call start
  mov $60, %rax
  mov $0, %rdi
  syscall
str_0: .string ""
float_0: .double 3.4
float_1: .double 2.2
