global main
section .text
main:
  extern print
  extern itostr

  mov rdi, 2
  call itostr

  mov rdi, 2
  mov rdx, 1
  mov rsi, rax
  call print

  mov rdi, 3
  call itostr

  mov rdi, 2
  mov rdx, 1
  mov rsi, rax
  call print

  mov rax, 60
  mov rdi, 0
  syscall

section .rodata
str_0: db ", ", 0
