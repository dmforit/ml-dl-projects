section .data
    const_f3 dq -5.0

section .text
global f3
f3:
    push ebp
    mov ebp, esp
    sub esp, 12
    fld qword[const_f3]
    fld qword[ebp + 8]
    fdivp
    mov esp, ebp
    pop ebp
    ret
    