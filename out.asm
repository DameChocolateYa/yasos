global _start
_start:
  mov rax, 32
push rax
  mov rax, 34
push rax
push QWORD [rsp + 0]

  mov rax, 60
pop rdi
  syscall
