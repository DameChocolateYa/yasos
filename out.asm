global main
section .text
  extern free
  extern print
func:
  push rbp
  mov rbp, rsp
  mov rsi, QWORD [rbp + 24]
  mov rax, rsi
  push rax
  mov rsi, QWORD [rbp + 16]
  mov rax, rsi
  push rax
  pop rbx
  pop rax
  add rax, rbx
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
  mov rsi, QWORD [rsp + 0]
  mov rax, rsi
  push rax
  mov rax, 15
  push rax
  pop rbx
  pop rax
  cmp rax, rbx
  setg al
  movzx rax, al
  push rax
  pop rax
  cmp rax, 0

  je else_0
  mov rdi, 2
  mov rdx, 0
  lea rsi, [ rel str_0]
  call print

  mov rsi, QWORD [rsp + 0]
  mov rax, rsi
  push rax
  pop rsi
  mov rdi, 0
  mov rdx, 0
  call print
  lea rax, [ rel str_1]
  push rax
  pop rsi
  mov rdi, 2
  mov rdx, 0
  call print
  mov rdi, 2
  mov rdx, 3
  call print
  jmp endif0
else_0:
  mov rdi, 2
  mov rdx, 0
  lea rsi, [ rel str_2]
  call print

  mov rsi, QWORD [rsp + 0]
  mov rax, rsi
  push rax
  pop rsi
  mov rdi, 0
  mov rdx, 0
  call print
  lea rax, [ rel str_3]
  push rax
  pop rsi
  mov rdi, 2
  mov rdx, 0
  call print
  mov rdi, 2
  mov rdx, 3
  call print
endif0:
  mov rsp, rbp
  pop rbp
  ret
start:
  push rbp
  mov rbp, rsp
  mov rax, 2
  push rax
  pop rdi
  push rdi
  mov rax, 1
  push rax
  pop rsi
  push rsi
  call func
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
str_1: db " ES MAYOR QUE 15", 0
str_2: db "", 0
str_3: db " ES MENOR QUE 15", 0
