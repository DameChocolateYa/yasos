global main
section .text
main:
  extern free
  extern print
  extern scani
  extern stoint
  mov rax, 2
  push rax
  mov rax, 2
  push rax
  pop rbx
  pop rax
  cmp rax, rbx
  sete al
  movzx rax, al
  push rax
  mov rax, 3
  push rax
  pop rbx
  pop rax
  cmp rax, 0
  setne al
  movzx rax, al
  cmp rbx, 0
  setne bl
  and al, bl
  movzx rax, al
  push rax
  mov rax, 3
  push rax
  pop rbx
  pop rax
  cmp rax, rbx
  sete al
  movzx rax, al
  push rax
  pop rax
  cmp rax, 0
  je else_0
  lea rax, [ rel str_0]
  push rax
  mov rdi, 2
  pop rsi
  mov rdx, 0
  call print
  mov rdx, 3
  call print
  jmp endif0
else_0:
endif0:
  mov rax, 60
  mov rdi, 0
  syscall

section .rodata
str_0: db "S", 0
