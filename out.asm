global main
section .text
main:
  extern free
  extern print
  extern scani
  extern strcmp
  extern clsterm
  call clsterm
  lea rax, [ rel str_0]
  push rax
  mov rdi, 2
  pop rsi
  mov rdx, 0
  call print
  call scani
  push rax
  mov rsi, QWORD [rsp + 0]
  push rsi
  lea rax, [ rel str_1]
  push rax
  pop rdi
  pop rsi
  call strcmp
  push rax
  mov rax, 1
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
  lea rax, [ rel str_2]
  push rax
  mov rdi, 2
  pop rsi
  mov rdx, 0
  call print
  mov rdx, 3
  call print
  jmp endif0
else_0:
  lea rax, [ rel str_3]
  push rax
  mov rdi, 2
  pop rsi
  mov rdx, 0
  call print
  mov rdx, 3
  call print
endif0:
  lea rax, [ rel str_4]
  push rax
  mov rdi, 2
  pop rsi
  mov rdx, 0
  call print
  mov rdx, 3
  call print
  mov rax, 60
  mov rdi, 0
  syscall

section .rodata
str_0: db "QUIRES DEPORTAR A CARLOS A RUMANIA (y/n): ", 0
str_1: db "y", 0
str_2: db "OK... DEPORTANDO A CARLOS...", 0
str_3: db "ME LA SUDA TU PUTA OPINION... DEPORTANDO A CARLOS...", 0
str_4: db "FIN DEL PROGRAMA DE DEPORTACION!", 0
