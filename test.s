.globl main
.extern print
start:
  push %rbp
  mov %rsp, %rbp
  movsd float_0(%rip), %xmm0
  sub $16, %rsp
  movsd %xmm0, (%rsp) # Creating a new var (pi)
  movsd float_1(%rip), %xmm0
  sub $16, %rsp
  movsd %xmm0, (%rsp) # Creating a new var (e)
  mov $2, %rdi
  mov $0, %rdx
  lea str_0(%rip), %rsi
  call *print@GOTPCREL(%rip)
  movsd 16(%rsp), %xmm0
  mov $1, %rdi
  mov $0, %rdx
  call *print@GOTPCREL(%rip)
  lea str_1(%rip), %rsi
  mov $2, %rdi
  mov $0, %rdx
  call *print@GOTPCREL(%rip)
  mov $2, %rdi
  mov $3, %rdx
  call *print@GOTPCREL(%rip)
  lea str_2(%rip), %rsi
  mov $2, %rdi
  mov $0, %rdx
  call *print@GOTPCREL(%rip)
  movsd 0(%rsp), %xmm0
  mov $1, %rdi
  mov $0, %rdx
  call *print@GOTPCREL(%rip)
  lea str_3(%rip), %rsi
  mov $2, %rdi
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
str_0: .string "Pi: "
str_1: .string "..."
str_2: .string "e: "
str_3: .string "..."
float_0: .double 3.14159
float_1: .double 2.72828
