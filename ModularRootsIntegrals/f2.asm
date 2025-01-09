section .data
    const1_f2 dq -2.0
    const2_f2 dq 8.0

section .text
global f2
f2:
    push ebp
    mov ebp, esp
    sub esp, 12
    fld qword[ebp + 8]
    fld qword[const1_f2]
    fmulp
    fld qword[const2_f2]
    faddp
    mov esp, ebp
    pop ebp
    ret
    