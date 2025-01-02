extern exp

section .text
global der_f1
der_f1:
    push ebp
    mov ebp, esp
    sub esp, 12
    mov eax, dword[ebp + 8]
    mov dword[esp], eax
    mov eax, dword[ebp + 12]
    mov dword[esp + 4], eax
    call exp
    mov esp, ebp
    pop ebp
    ret
