global main
section .text
  extern print
start:
  push rbp
  mov rbp, rsp
  mov rbx, 4
  mov rax, 3
  add rax, rbx
  push rax
  mov rdi, 2
  mov rdx, 0
  lea rsi, [ rel str_0]
  call [rel print wrt ..got]
  mov rax, QWORD [rsp + 0]
  mov rsi, rax
  mov rdi, 0
  mov rdx, 0
  call [rel print wrt ..got]
  mov rdi, 2
  mov rdx, 3
  call [rel print wrt ..got]
  mov rsp, rbp
  pop rbp
  ret
main:
  extern strcat
  extern scani
  call start
    mov rax, 60
  mov rdi, 0
  syscall

section .rodata
str_0: db "", 0
