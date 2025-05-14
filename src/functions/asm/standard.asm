default rel

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

    color_red db 27, '[31m', 0
    color_yellow db 27, '[33m', 0
    color_purple db 27, '[35m', 0
    color_reset db 27, '[0m', 0

    red_w db "red", 0
    yellow_w db "yellow", 0
    purple_w db "purple", 0
    reset_w db "reset", 0

    test_msg db "BEEP LANG TEST RETURNING A STRING", 0

    waitk_buffer db 0

section .bss
    num_buffer resb 100
    scani_buffer resb 100

section .text
    global print:function
    global clsterm:function
    global colorterm:function
    global itostr:function
    global stoint:function
    global scani:function
    global strcmp:function
    global strdup:function
    global waitk:function
    global isnum:function
    global testret:function

    global strcat:function
    global len:function

    extern printf
    extern sprintf
    extern malloc
    extern realloc
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
    lea rdi, [rel fmt_int]
    xor rax, rax
    call [rel printf wrt ..got]
    extern fflush
    mov rdi, 0
    call [rel fflush wrt ..got]
    ret

.float:
    movq xmm0, [rsi]
    lea rdi, [rel fmt_float]
    xor rax, rax
    call [rel printf wrt ..got]
    ret

.str:
    lea rdi, [rel fmt_str]
    xor rax, rax
    call [rel printf wrt ..got]
    extern fflush
    mov rdi, 0
    call [rel fflush wrt ..got]
    ret

.int_ln:
    lea rdi, [rel fmt_int_ln]
    xor rax, rax
    call [rel printf wrt ..got]
    ret

.float_ln:
    movq xmm0, [rsi]
    lea rdi, [rel fmt_float_ln]
    xor rax, rax
    call [rel printf wrt ..got]
    ret

.str_ln:
    lea rdi, [rel fmt_str_ln]
    xor rax, rax
    call [rel printf wrt ..got]
    extern fflush
    mov rdi, 0
    call [rel fflush wrt ..got]
    ret

.ln:
    lea rdi, [rel fmt_str_ln]
    xor rax, rax
    lea rsi, [rel null_str]
    call [rel printf wrt ..got]
    ret


clsterm:
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel clear_ansi]
    mov rdx, clear_len
    syscall

    ; rdi = integer to convert
    ; Return in rax a pointer to buffer

colorterm:
    push rdi

    lea rsi, [rel red_w]
    call strcmp
    cmp rax, 1
    je .red

    lea rsi, [rel yellow_w]
    call strcmp
    cmp rax, 1
    je .yellow

    lea rsi, [rel purple_w]
    call strcmp
    cmp rax, 1
    je .purple

    lea rsi, [rel reset_w]
    call strcmp
    cmp rax, 1
    je .reset

.red:
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel color_red]
    mov rdx, 5
    syscall
    jmp .done

.yellow:
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel color_yellow]
    mov rdx, 5
    syscall
    jmp .done

.purple:
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel color_purple]
    mov rdx, 5
    syscall
    jmp .done

.reset:
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel color_reset]
    mov rdx, 5
    syscall
    jmp .done

.done:
    pop rdi
    ret

itostr:
    mov rax, rdi

    mov rdi, 20                 ; Save space
    call [rel malloc wrt ..got] ; rax = pointer to buffer
    mov rsi, rax                ; rsi = pointer at the end of the buffer
    add rsi, 19                 ; We point at the end
    mov byte [rsi], 0           ; Null terminator
    dec rsi

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
    lea rax, [rel test_msg]
    ret

scani:
    mov rdi, 20
    call [rel malloc wrt ..got]
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
    push rdi
    push rsi

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
    pop rsi
    pop rdi
    ret

.not_equal:
    xor rax, rax            ; Diferent strings → return 0
    pop rsi
    pop rdi
    ret

strdup: ; Obsolete function!
    ; Input: rdi = pointer to original string
    ; Output: rax = pointer to duplicated string

    push rdi                    ; Save the original pointer
    call [rel strlen wrt ..got] ; strlen(rdi) → rax
    inc rax                     ; Include null terminator
    mov rdi, rax                ; malloc(len)
    call [rel malloc wrt ..got] ; rax = new pointer

    pop rsi                     ; rsi = original pointer
    mov rdi, rax                ; rdi = destination
    mov rdx, rax                ; Save return in rax (destination)
    call [rel memcpy wrt ..got] ; Copy the content

    mov rax, rdi                ; Return duplicated pointer
    ret

waitk:
    ; Wait for user to press a key
    mov rax, 0
    mov rdi, 0
    lea rsi, [waitk_buffer]
    mov rdx, 1
    syscall

isnum:
    mov al, byte [rdi]
    test al, al ; check if rdi is not empty
    je .no

.loop:
    mov al, byte [rdi]
    test al, al ; Are we at the end of the content?
    je .done

    cmp al, '0'
    jl .no ; If it is minor than 0x30, it is not a digit

    cmp al, '9'
    jg .no ; If it is greater than 0x39, it is not a digit

    inc rdi
    jmp .loop

.done:
    mov rax, 1 ; If it is between 0x30 and 0x39, it is a num
    ret
.no:
    xor rax, rax
    ret

; ------------------------------------------
; void strcat(char* s1, char* s2)
; Input:
; rdi = s1
; rsi = s2
; ------------------------------------------

strcat:
    push rsi
    push rdi

    mov rdi, 180
    call [rel malloc wrt ..got] ; Save space on a dinamic buffer
    mov rbx, rax ; the buffer pointer

    pop rdi
    pop rsi

    xor rcx, rcx ; clean counter for str1

.copy_str1:
    mov al, [rdi + rcx] ; load current byte
    cmp al, 0
    je .done_str1 ; if it is null terminator, end the copy of str1

    mov [rbx + rcx], al ; save in buffer the byte
    inc rcx

    jmp .copy_str1

.done_str1:
    xor rdx, rdx ; clean counter for str2

.copy_str2:
    mov al, [rsi + rdx] ; load current byte
    mov [rbx + rcx], al ; save in buffer the byte
    inc rcx
    inc rdx

    cmp al, 0 ; if it is null terminator, end process
    jne .copy_str2

    mov rax, rbx ; return result in rax
    ret

; rdi = str
; rax = len
len:
    xor rax, rax
.loop:
    cmp byte [rdi + rax], 0
    je .done
    inc rax
    jmp .loop
.done:
    ret