global main
section .text
main:
  extern free
  extern print
  lea rax, [ rel str_0]
  push rax
  mov rdi, 2
  pop rsi
  mov rdx, 0
  call print
  mov rdx, 3
  call print
  mov rax, 60
  mov rdi, 0
  syscall

section .rodata
str_0: db "VIVA ESPAÑA!", 0
