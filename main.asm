global main
section .text
  extern print
factorial:
  push rbp
  mov rbp, rsp
  mov rax, QWORD [rbp + 16]
  push rax
  mov rax, QWORD [rbp + 16]
  push rax
while_1_start:
  mov rax, 1
  mov rbx, rax
  mov rax, QWORD [rsp + 0]
  cmp rax, rbx
  setg al
  movzx rax, al
  cmp rax, 0
  je while_1_end
  mov rax, 1
  mov rbx, rax
  mov rax, QWORD [rsp + 0]
  sub rax, rbx
  mov rbx, rax
  mov rax, QWORD [ rsp + 8 ]
  imul rax, rbx
  mov QWORD [ rsp + 8], rax
  mov QWORD [ rsp + 8 ], rax
  mov rax, QWORD [ rsp + 0 ]
  sub rax, 1
  mov QWORD [ rsp + 0], rax
  mov QWORD [ rsp + 0 ], rax
  jmp while_1_start
while_1_end:
  add rsp, 1
  pop rax
  sub rsp, 1
  mov rax, QWORD [rsp + 8]
  mov rsp, rbp
  pop rbp
  ret
  mov rsp, rbp
  pop rbp
  ret
start:
  push rbp
  mov rbp, rsp
  mov rdi, 2
  mov rdx, 0
  lea rsi, [ rel str_0]
  call print

  call scani
  mov rdi, rax
  call stoint
  mov QWORD [ rsp + 8 ], rax
  mov rax, 3
  mov QWORD [ rsp + 8 ], rax
  mov rax, QWORD [rsp + 8]
  push rax
  pop rdi
  push rdi
  call factorial
  push rax
  push rax
  mov rdi, 2
  mov rdx, 0
  lea rsi, [ rel str_1]
  call print

  mov rax, QWORD [rsp + 0]
  mov rsi, rax
  mov rdi, 0
  mov rdx, 0
  call print
  mov rdi, 2
  mov rdx, 3
  call print
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
str_0: db "Ponga un num: ", 0
str_1: db "El factorial de ese numero es: ", 0
