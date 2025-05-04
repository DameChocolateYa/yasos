global main
section .text
  extern print
start:
  push rbp
  mov rbp, rsp
  mov rdi, 2
  mov rdx, 0
  lea rsi, [ rel str_0]
  call print

  mov rax, 1
  push rax
  pop rsi
  mov rdi, 0
  mov rdx, 0
  call print
  mov rdi, 2
  mov rdx, 3
  call print
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
