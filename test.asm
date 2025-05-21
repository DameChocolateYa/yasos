global main
section .text
extern print
sum:
  push rbp
  mov rbp, rsp
  movsd xmm1, QWORD [rbp + 16]
  movsd xmm0, QWORD [rbp + 32]
  addsd xmm0, xmm1
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
  lea rsi, [rel str_0]
  call [rel print wrt ..got]
  call [rel scani wrt ..got]
  sub rsp, 16
  mov [rsp], rax; Creating a new var (msg)
  mov rbx, 1
  mov rdi, QWORD [rsp + 0]
  call [rel isfloat wrt ..got]
  cmp rax, rbx
  setne al
  movzx rax, al
  cmp rax, 0
  je else_0
  mov rdi, 2
  mov rdx, 0
  lea rsi, [rel str_1]
  call [rel print wrt ..got]
  mov rdi, 2
  mov rdx, 3
  call [rel print wrt ..got]
  mov rax, 60
  mov rdi, 1
  syscall
  jmp endif0
else_0:
endif0:
  mov rdi, QWORD [rsp + 0]
  call [rel stofl wrt ..got]
  sub rsp, 16
  movsd [rsp], xmm0; Creating a new var (x)
  mov rdi, 2
  mov rdx, 0
  lea rsi, [rel str_2]
  call [rel print wrt ..got]
  call [rel scani wrt ..got]
  mov QWORD [ rsp + 16 ], rax     ; Editing var value (msg)
  mov rbx, 1
  mov rdi, QWORD [rsp + 16]
  call [rel isfloat wrt ..got]
  cmp rax, rbx
  setne al
  movzx rax, al
  cmp rax, 0
  je else_1
  mov rdi, 2
  mov rdx, 0
  lea rsi, [rel str_3]
  call [rel print wrt ..got]
  mov rdi, 2
  mov rdx, 3
  call [rel print wrt ..got]
  mov rax, 60
  mov rdi, 1
  syscall
  jmp endif1
else_1:
endif1:
  mov rdi, QWORD [rsp + 16]
  call [rel stofl wrt ..got]
  sub rsp, 16
  movsd [rsp], xmm0; Creating a new var (y)
  movsd xmm0, QWORD [rsp + 16]
  sub rsp, 16
  movsd [rsp], xmm0
  movsd xmm0, QWORD [rsp + 16]
  sub rsp, 16
  movsd [rsp], xmm0
  call sum
  mov rax, [rsp]
  add rsp, 16
  mov rax, [rsp]
  add rsp, 16
  sub rsp, 16
  movsd [rsp], xmm0; Creating a new var (result)
  mov rdi, 2
  mov rdx, 0
  lea rsi, [rel str_4]
  call [rel print wrt ..got]
  movsd xmm0, QWORD [rsp + 0]
  mov rdi, 1
  mov rdx, 0
  call [rel print wrt ..got]
  mov rdi, 2
  mov rdx, 3
  call [rel print wrt ..got]
  mov rsp, rbp
  pop rbp
  ret
main:
  extern scani
  extern stofl
  extern isfloat
  call start
  mov rax, 60
  mov rdi, 0
  syscall

section .rodata
str_0: db "Num1 (float): ", 0
str_1: db "EXPECTED FLOAT: ", 0
str_2: db "Num2 (float): ", 0
str_3: db "EXPECTED FLOAT: ", 0
str_4: db "Result: ", 0
