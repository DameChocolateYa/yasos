global main
section .text
main:
  extern free
  extern print
  extern scani
  mov rdi, 2
  mov rdx, 0
  lea rsi, [ rel str_0]
  push rsi
  call print
  call scani
  push rax
  mov rdi, 2
  mov rdx, 0
  lea rsi, [ rel str_1]
  push rsi
  call print
  mov rsi, QWORD [rsp + 8]
  push rsi
  mov rdi, 2
  pop rsi
  mov rdx, 0
  call print
  mov rdi, 2
  mov rdx, 3
  call print
  mov rax, 60
  mov rdi, 0
  syscall

section .rodata
str_0: db "Prompt: ", 0
str_1: db "Output: ", 0
