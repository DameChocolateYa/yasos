global main
section .text
  extern print
start:
  push rbp
  mov rbp, rsp
while_start_1:
  mov rax, 1
  cmp rax, 0
  je while_end_1
  call scani

  push rax
  mov rsi, QWORD [rsp + 0]
  lea rdi, [ rel str_0]
  call strcmp

  cmp rax, 0
  je else_0
  jmp while_end_1
  jmp endif0
else_0:
endif0:
  pop rax
  jmp while_start_1
while_end_1:
  mov rsp, rbp
  pop rbp
  ret
main:
  extern scani
  extern strcmp
  call start
    mov rax, 60
  mov rdi, 0
  syscall

section .rodata
str_0: db "stop", 0
