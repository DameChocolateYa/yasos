global main
section .text
extern print
sum:
  push rbp
  mov rbp, rsp
  movsd xmm1, QWORD [rbp + 16]
  movsd xmm0, QWORD [rbp + 32]
  addsd xmm0, xmm1
  mov rsp, rbp
  pop rbp
  ret
  mov rsp, rbp
  pop rbp
  ret
start:
  push rbp
  mov rbp, rsp
  movsd xmm0, [rel float_0]
  sub rsp, 16
  movsd [rsp], xmm0
  movsd xmm0, [rel float_1]
  sub rsp, 16
  movsd [rsp], xmm0
  call sum
  mov rax, [rsp]
  add rsp, 16
  mov rax, [rsp]
  add rsp, 16
  sub rsp, 16
  movsd [rsp], xmm0; Creating a new var (z)
  mov rdi, 2
  mov rdx, 0
  lea rsi, [rel str_0]
  call [rel print wrt ..got]
  movsd xmm0, QWORD [rsp + 0]
  mov rdi, 1
  mov rdx, 0
  call [rel print wrt ..got]
  mov rdi, 2
  mov rdx, 3
  call [rel print wrt ..got]
  mov rsp, rbp
  pop rbp
  ret
main:
  call start
  mov rax, 60
  mov rdi, 0
  syscall

section .rodata
str_0: db "", 0
float_0: dq 3.400000
float_1: dq 2.200000
