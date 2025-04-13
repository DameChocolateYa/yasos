global main
main:
  lea rax, [str_0]
  push rax
  pop rsi
  mov rdi, 2
  mov rdx, 0
  extern print
  call print
  lea rax, [str_1]
  push rax
  pop rsi
  mov rdi, 2
  mov rdx, 1
  extern print
  call print
  lea rax, [str_2]
  push rax
  pop rsi
  mov rdi, 2
  mov rdx, 0
  extern print
  call print
  mov rax, 0
  push rax
  mov rax, 60
  pop rdi
  syscall

section .rodata
str_0: db "HOLA", 0
str_1: db "", 0
str_2: db "MUNDO", 0
