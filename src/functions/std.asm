section .data
    fmt_int   db "%d", 0         ; formato para int
    fmt_float db "%f", 0         ; formato para float/double
    fmt_str   db "%s", 0         ; formato para strings
    fmt_int_ln   db "%d", 10, 0  ; formato para int con salto de línea
    fmt_float_ln db "%f", 10, 0  ; formato para float con salto de línea
    fmt_str_ln   db "%s", 10, 0  ; formato para string con salto de línea

    newline_str db 10, 0
    null_str db "", 0

    clear_ansi db 27,"[H",27,"[2J"  ; "\033[2J\033[H"   clear terminal
    clear_len equ $-clear_ansi

    test_msg db "BEEP LANG TEST RETURNING A STRING", 0

section .bss
    num_buffer resb 20
    scani_buffer resb 100

section .text
    global print
    global clsterm
    global itostr
    global stoint
    global scani

    extern printf
    extern sprintf
    extern testret

; Uso:
;   rdi = tipo: 0 = int, 1 = double, 2 = char*
;   rsi = valor (int o puntero o puntero al double)
;   rdx = salto de línea: 0 = no, 1 = sí, 3 = SOLO salto de linea
print:
    ; Verificar si se debe agregar salto de línea (rdx = 1)
    cmp rdx, 3
    je .ln

    cmp rdx, 1
    je .with_newline
    jmp .without_newline

.with_newline:
    cmp rdi, 0
    je .int_ln

    cmp rdi, 1
    je .float_ln

    cmp rdi, 2
    je .str_ln

    ret

.without_newline:
    cmp rdi, 0
    je .int

    cmp rdi, 1
    je .float

    cmp rdi, 2
    je .str

    ret

.int:
    mov rdi, fmt_int
    xor rax, rax
    call printf
    ret

.float:
    movq xmm0, [rsi]
    mov rdi, fmt_float
    xor rax, rax
    call printf
    ret

.str:
    mov rdi, fmt_str
    xor rax, rax
    call printf
    extern fflush
    mov rdi, 0
    call fflush
    ret

.int_ln:
    mov rdi, fmt_int_ln
    xor rax, rax
    call printf
    ret

.float_ln:
    movq xmm0, [rsi]
    mov rdi, fmt_float_ln
    xor rax, rax
    call printf
    ret

.str_ln:
    mov rdi, fmt_str_ln
    xor rax, rax
    call printf
    extern fflush
    mov rdi, 0
    call fflush
    ret

.ln:
    mov rdi, fmt_str_ln
    xor rax, rax
    mov rsi, null_str
    call printf
    ret


clsterm:
    mov eax, 4
    mov ebx, 1
    mov ecx, clear_ansi
    mov edx, clear_len
    int 0x80
    ret

itostr:
    mov rdx, rdi            ; valor (tercer argumento)
    mov rsi, fmt_int        ; formato "%d" (segundo argumento)
    mov rdi, num_buffer     ; destino del string (primer argumento)
    call sprintf
    lea rax, [num_buffer]   ; devolver dirección del buffer
    ret

stoint:
    xor rax, rax
    xor rcx, rcx
.next_char:
    movzx rcx, byte [rdi]
    cmp rcx, 0
    je .done

    cmp rcx, '0'
    jb .not_digit
    cmp rcx, '9'
    ja .not_digit

    sub rcx, '0'
    imul rax, rax, 10
    add rax, rcx

    inc rdi
    jmp .next_char
.done:
    ret
.not_digit:
    ret

testret:
    lea rax, [test_msg]
    ret

scani:
    mov rax, 0
    mov rdi, 0
    mov rsi, scani_buffer
    mov rdx, 100
    syscall
    test eax, eax
    jz .input_error
    lea rax, [scani_buffer]
    ret
.input_error:
    ret
