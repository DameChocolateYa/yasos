global main
section .text
main:
  extern print
  extern scani
  mov rax, 0
  push rax
  lea rax, [str_0]
  push rax
  lea rax, [str_1]
  push rax
  mov rdi, 2
  pop rsi
  mov rdx, 0
  call print
  mov rdx, 3
  call print
  call scani
  push rax
  mov rax, QWORD [ rsp + 16 ]
  push rax
  add rax, 1
  mov QWORD [ rsp + 16], rax
  push rax
  mov rsi, QWORD [rsp + 0]
  push rsi
  mov rdi, 0
  pop rsi
  mov rdx, 0
  call print
  mov rdx, 3
  call print
  call scani
  push rax
  mov rax, QWORD [ rsp + 8 ]
  push rax
  add rax, 1
  mov QWORD [ rsp + 8], rax
  push rax
  mov rsi, QWORD [rsp + 0]
  push rsi
  mov rdi, 0
  pop rsi
  mov rdx, 0
  call print
  mov rdx, 3
  call print
  call scani
  push rax
  mov rax, QWORD [ rsp + 8 ]
  push rax
  add rax, 1
  mov QWORD [ rsp + 8], rax
  push rax
  mov rsi, QWORD [rsp + 0]
  push rsi
  mov rdi, 0
  pop rsi
  mov rdx, 0
  call print
  mov rdx, 3
  call print
  call scani
  push rax
  mov rax, QWORD [ rsp + 8 ]
  push rax
  add rax, 1
  mov QWORD [ rsp + 8], rax
  push rax
  mov rsi, QWORD [rsp + 0]
  push rsi
  mov rdi, 0
  pop rsi
  mov rdx, 0
  call print
  mov rdx, 3
  call print
  call scani
  push rax
  mov rax, QWORD [ rsp + 8 ]
  push rax
  add rax, 1
  mov QWORD [ rsp + 8], rax
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
str_0: db "", 0
str_1: db "Presiona 'Enter' hasta que llegue a 5", 0
