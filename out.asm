global main
section .text
  extern free
imprimir:
  push rbp
  mov rbp, rsp
  mov rdi, 2
  mov rdx, 0
  lea rsi, [ rel str_0]
  push rsi
  call print

  mov rsi, QWORD [rsp + 32]
  push rsi
  mov rdi, 2
  pop rsi
  mov rdx, 0
  call print
  mov rdi, 2
  mov rdx, 3
  call print
  mov rsp, rbp
  pop rbp
  ret
start:
  push rbp
  mov rbp, rsp
  lea rax, [ rel str_1]
  push rax
  push rax
  call imprimir
  mov rsp, rbp
  pop rbp
  ret
main:
  extern print
  call start
    mov rax, 60
  mov rdi, 0
  syscall

section .rodata
str_0: db "", 0
str_1: db "AH", 0
