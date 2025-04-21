section .data
    fmt_int   db "%d", 0         ; Format for int
    fmt_float db "%f", 0         ; Format for float/double
    fmt_str   db "%s", 0         ; Format for strings
    fmt_int_ln   db "%d", 10, 0  ; Format for int with jump line
    fmt_float_ln db "%f", 10, 0  ; Format for float/double with jump line
    fmt_str_ln   db "%s", 10, 0  ; Format for string with jump line

    newline_str db 10, 0
    null_str db "", 0

    clear_ansi db 27,"[H",27,"[2J"  ; "\033[2J\033[H"   Clear terminal
    clear_len equ $-clear_ansi

    test_msg db "BEEP LANG TEST RETURNING A STRING", 0

    waitk_buffer db 0

section .bss
    num_buffer resb 100
    scani_buffer resb 100

section .text
    global print
    global clsterm
    global itostr
    global stoint
    global scani
    global strcmp
    global strdup
    global waitk

    extern printf
    extern sprintf
    extern testret
    extern malloc
    extern strlen
    extern memcpy

; Uso:
;   rdi = type: 0 = int, 1 = double, 2 = char*
;   rsi = value (int or pointer or pointer to double)
;   rdx = line jump: 0 = no, 1 = yes, 3 = ONLY line jump
print:
    ; Verify if it's necessary add a line jump (rdx = 1)
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
    mov rax, 1
    mov rdi, 1
    mov rsi, clear_ansi
    mov rdx, clear_len
    syscall

    ; rdi = integer to convert
    ; Return in rax a pointer to buffer
itostr:
    push rdi              ; We save the original number in the pile

    mov rdi, 20           ; Save space
    call malloc           ; rax = pointer to buffer
    mov rsi, rax          ; rsi = pointer at the end of the buffer
    add rsi, 19           ; We point at the end
    mov byte [rsi], 0     ; Null terminator
    dec rsi

    pop rax               ; We get back the original number we want to convert

    mov rcx, 0
    mov rbx, 10
    cmp rax, 0
    jge .convert_loop

    neg rax
    mov bl, '-'
    mov [rsi], bl
    dec rsi
.convert_loop:
    xor rdx, rdx
    div rbx
    add dl, '0'
    mov [rsi], dl
    dec rsi
    inc rcx
    test rax, rax
    jnz .convert_loop
    lea rax, [rsi + 1]         ; Ajust at the beggining of the real string
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
    mov rdi, 20
    call malloc
    push rdi

    mov rax, 0
    mov rdi, 0
    pop rsi
    mov rdx, 100
    syscall

    mov rbx, rax
    mov byte [rsi + rbx], 0  ; Null terminator

    dec rbx
    mov al, [rsi + rbx]
    cmp al, 10  ; Verify if it is a jump line
    jne .done
    mov byte [rsi + rbx], 0  ; Delete jump line
.done:
    mov rax, rsi
    ret



strcmp:
.loop:
    mov al, byte [rdi]      ; Load character from s1
    mov bl, byte [rsi]      ; Load character from s2
    cmp al, bl              ; Compare characters
    jne .not_equal          ; If they are equal, exit with code 0
    test al, al             ; We arrive at the end of the string?
    je .equal               ; If they both are cero, they are equal
    inc rdi
    inc rsi
    jmp .loop

.equal:
    mov rax, 1              ; Equal strings → return 1
    ret

.not_equal:
    xor rax, rax            ; Diferent strings → return 0
    ret

strdup: ; Obsolete function!
    ; Input: rdi = pointer to original string
    ; Output: rax = pointer to duplicated string

    push rdi             ; Save the original pointer
    call strlen          ; strlen(rdi) → rax
    inc rax              ; Include null terminator
    mov rdi, rax         ; malloc(len)
    call malloc          ; rax = new pointer

    pop rsi              ; rsi = original pointer
    mov rdi, rax         ; rdi = destination
    mov rdx, rax         ; Save return in rax (destination)
    call memcpy          ; Copy the content

    mov rax, rdi         ; Return duplicated pointer
    ret

waitk:
    ; Wait for user to press a key
    mov rax, 0
    mov rdi, 0
    lea rsi, [waitk_buffer]
    mov rdx, 1
    syscall
