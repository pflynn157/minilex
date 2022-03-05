.data

.text

; Comment 2

_start:
    mov eax, 5
    mov ebx, 20
    int 0x20
    syscall
    ret ; Comment1

