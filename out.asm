global main
section .text
main:
  extern free
  extern print
  extern scani
  extern clsterm
  extern waitk
  call clsterm
  lea rax, [ rel str_0]
  push rax
  mov rsi, QWORD [rsp + 0]
  push rsi
  mov rdi, 2
  pop rsi
  mov rdx, 0
  call print
  mov rdx, 3
  call print
  call waitk
  mov rax, 60
  mov rdi, 0
  syscall

section .rodata
str_0: db "Esto es un mensaje", 0
