global main
section .text
  extern free
pr:
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
sc:
  push rbp
  mov rbp, rsp
  mov rdi, 2
  mov rdx, 0
  lea rsi, [ rel str_1]
  push rsi
  call print

  call scani

  push rax
  mov rsi, QWORD [rsp + 0]
  push rsi
  push rax
  call pr
  mov rsp, rbp
  pop rbp
  ret
start:
  push rbp
  mov rbp, rsp
  call sc
  mov rsp, rbp
  pop rbp
  ret
main:
  extern print
  extern scani
  call start
    mov rax, 60
  mov rdi, 0
  syscall

section .rodata
str_0: db "EL INPUT ES: ", 0
str_1: db "PROMPT: ", 0
