global main
section .text
main:
  extern print
  extern scani
  extern itostr
  extern stoint
  mov rax, 3
  push rax
  mov rsi, QWORD [rsp + 0]
  push rsi
  mov rdi, 0
  pop rsi
  mov rdx, 0
  call print
  mov rdx, 3
  call print
  mov rax, 60
  mov rdi, 0
  syscall
