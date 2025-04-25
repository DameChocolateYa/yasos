global main
section .text
main:
  extern free
  extern print
  mov rax, 5
  push rax
  mov rax, QWORD [ rsp + 0]
  push rax
  mov rax, 3
  push rax
  pop rbx
  pop rax
  add rax, rbx
  mov QWORD [ rsp + 0], rax
  push rax
  mov rax, QWORD [ rsp + 0]
  push rax
  mov rax, 2
  push rax
  pop rbx
  pop rax
  sub rax, rbx
  mov QWORD [ rsp + 0], rax
  push rax
  mov rax, QWORD [ rsp + 0 ]
  push rax
  add rax, 1
  mov QWORD [ rsp + 0], rax
  push rax
  mov rax, QWORD [ rsp + 0]
  push rax
  mov rax, 2
  push rax
  pop rbx
  pop rax
  imul rax, rbx
  mov QWORD [ rsp + 0], rax
  push rax
  mov rax, QWORD [ rsp + 0 ]
  push rax
  sub rax, 1
  mov QWORD [ rsp + 0], rax
  push rax
  mov rsi, QWORD [rsp + 0]
  push rsi
  mov rdi, 0
  pop rsi
  mov rdx, 0
  call print
  mov rdx, 3
  call print
  mov rax, 60
  mov rdi, 0
  syscall

section .rodata
