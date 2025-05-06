global main
section .text
  extern print
func:
  push rbp
  mov rbp, rsp
  mov rax, 1
  push rax
while_1_start:
  mov rax, QWORD [rbp + 16]
  mov rbx, rax
  mov rax, QWORD [rsp + 0]
  cmp rax, rbx
  setle al
  movzx rax, al
  cmp rax, 0
  je while_1_end
  mov rdi, 2
  mov rdx, 0
  lea rsi, [ rel str_0]
  call print

  mov rax, QWORD [rsp + 0]
  mov rsi, rax
  mov rdi, 0
  mov rdx, 0
  call print
  lea rax, [ rel str_1]
  mov rsi, rax
  mov rdi, 2
  mov rdx, 0
  call print
  mov rax, QWORD [rbp + 24]
  mov rsi, rax
  mov rdi, 2
  mov rdx, 0
  call print
  mov rdi, 2
  mov rdx, 3
  call print
  mov rax, QWORD [ rsp + 0 ]
  add rax, 1
  mov QWORD [ rsp + 0], rax
  mov QWORD [ rsp + 0 ], rax
  jmp while_1_start
while_1_end:
  add rsp, 0
  pop rax
  sub rsp, 0
  mov rsp, rbp
  pop rbp
  ret
start:
  push rbp
  mov rbp, rsp
  mov rdi, 2
  mov rdx, 0
  lea rsi, [ rel str_2]
  call print

  call scani
  push rax
  mov rdi, 2
  mov rdx, 0
  lea rsi, [ rel str_3]
  call print

  call scani
  mov rdi, rax
  call stoint
  push rax
  mov rax, QWORD [rsp + 8]
  push rax
  pop rdi
  push rdi
  mov rax, QWORD [rsp + 8]
  push rax
  pop rsi
  push rsi
  call func
  mov rsp, rbp
  pop rbp
  ret
main:
  extern scani
  extern stoint
  call start
    mov rax, 60
  mov rdi, 0
  syscall

section .rodata
str_0: db "", 0
str_1: db ".", 0
str_2: db "Msg: ", 0
str_3: db "N: ", 0
