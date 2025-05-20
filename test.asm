global main
section .text
  extern print
start:
  push rbp
  mov rbp, rsp
  mov rax, 0
  sub rsp, 16
  mov [rsp], rax; Creating a new var (i)
while_1_start:
  mov rbx, 3
  mov rax, QWORD [rsp + 0]
  cmp rax, rbx
  setl al
  movzx rax, al
  cmp rax, 0
  je while_1_end
  mov rax, 3
  sub rsp, 16
  mov [rsp], rax; Creating a new var (z)
  mov rax, 2
  sub rsp, 16
  mov [rsp], rax; Creating a new var (y)
  mov rax, QWORD [ rsp + 32 ]
  add rax, 1
  mov QWORD [ rsp + 32], rax
  mov rdi, 2
  mov rdx, 0
  lea rsi, [ rel str_0]
  call [rel print wrt ..got]
  mov rsi, QWORD [rsp + 32]
  mov rdi, 0
  mov rdx, 0
  call [rel print wrt ..got]
  mov rdi, 2
  mov rdx, 3
  call [rel print wrt ..got]
  mov rdi, 2
  mov rdx, 0
  lea rsi, [ rel str_1]
  call [rel print wrt ..got]
  mov rsi, QWORD [rsp + 48]
  mov rdi, 0
  mov rdx, 0
  call [rel print wrt ..got]
  mov rdi, 2
  mov rdx, 3
  call [rel print wrt ..got]
  mov rax, [rsp]
  add rsp, 16
  mov rax, [rsp]
  add rsp, 16
  jmp while_1_start
while_1_end:
  mov rdi, 2
  mov rdx, 0
  lea rsi, [ rel str_2]
  call [rel print wrt ..got]
  mov rsi, QWORD [rsp + 16]
  mov rdi, 0
  mov rdx, 0
  call [rel print wrt ..got]
  mov rdi, 2
  mov rdx, 3
  call [rel print wrt ..got]
  mov rsp, rbp
  pop rbp
  ret
main:
  call start
    mov rax, 60
  mov rdi, 0
  syscall

section .rodata
str_0: db "Z: ", 0
str_1: db "I: ", 0
str_2: db "RESULT: ", 0
