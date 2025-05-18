global main
section .text
  extern print
start:
  push rbp
  mov rbp, rsp
  movsd xmm0, [rel float_0]
  sub rsp, 8
  movsd [rsp], xmm0

  movsd xmm0, [rsp]
  add rsp, 8
  mov rdi, 1
  mov rdx, 0
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
float_0: dq 2.6678
