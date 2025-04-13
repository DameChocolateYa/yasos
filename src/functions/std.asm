section .data
    fmt_int   db "%d", 0         ; formato para int
    fmt_float db "%f", 0         ; formato para float/double
    fmt_str   db "%s", 0         ; formato para strings
    fmt_int_ln   db "%d", 10, 0  ; formato para int con salto de línea
    fmt_float_ln db "%f", 10, 0  ; formato para float con salto de línea
    fmt_str_ln   db "%s", 10, 0  ; formato para string con salto de línea

section .text
    global print
    extern printf

; Uso:
;   rdi = tipo: 0 = int, 1 = double, 2 = char*
;   rsi = valor (int o puntero o puntero al double)
;   rdx = salto de línea: 0 = no, 1 = sí
print:
    ; Verificar si se debe agregar salto de línea (rdx = 1)
    cmp rdx, 1
    je .print_with_newline      ; Si rdx es 1, ir a la rutina de salto de línea
    jmp .print_without_newline  ; Si no, ir a la rutina sin salto de línea

.print_with_newline:
    cmp rdi, 0
    je .print_int_ln

    cmp rdi, 1
    je .print_float_ln

    cmp rdi, 2
    je .print_str_ln

    ret

.print_without_newline:
    cmp rdi, 0
    je .print_int

    cmp rdi, 1
    je .print_float

    cmp rdi, 2
    je .print_str

    ret

.print_int:
    mov rdi, fmt_int        ; formato sin salto de línea
    ; rsi ya contiene el valor (int), no es necesario mov rsi, rsi
    xor rax, rax            ; preparar rax para printf (llamada variádica)
    call printf
    ret

.print_float:
    movq xmm0, [rsi]        ; cargar el double en xmm0 (rsi contiene puntero al double)
    mov rdi, fmt_float      ; formato para float/double
    xor rax, rax            ; preparar rax para printf
    call printf
    ret

.print_str:
    mov rdi, fmt_str        ; formato para string
    ; rsi ya contiene el puntero a la cadena (string), no es necesario mov rsi, rsi
    xor rax, rax            ; preparar rax para printf
    call printf
    extern fflush
    mov rdi, 0      ; stdout
    call fflush
    ret

.print_int_ln:
    mov rdi, fmt_int_ln     ; formato con salto de línea
    ; rsi ya contiene el valor (int)
    xor rax, rax            ; preparar rax para printf
    call printf
    ret

.print_float_ln:
    movq xmm0, [rsi]        ; cargar el double en xmm0 (rsi contiene puntero al double)
    mov rdi, fmt_float_ln   ; formato con salto de línea
    xor rax, rax            ; preparar rax para printf
    call printf
    ret

.print_str_ln:
    mov rdi, fmt_str_ln     ; formato con salto de línea
    ; rsi ya contiene el puntero al string
    xor rax, rax            ; preparar rax para printf
    call printf
    extern fflush
    mov rdi, 0      ; stdout
    call fflush
    ret
