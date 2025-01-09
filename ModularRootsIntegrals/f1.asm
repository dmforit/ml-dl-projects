extern exp
global f1

section .data
    const_f1 dq 2.0

section .text
f1:
    push ebp
    mov ebp, esp
    sub esp, 12
    mov eax, dword[ebp + 8]
    mov dword[esp], eax
    mov eax, dword[ebp + 12]
    mov dword[esp + 4], eax
    call exp
    fld qword[const_f1]
    faddp
    mov esp, ebp
    pop ebp
    ret
