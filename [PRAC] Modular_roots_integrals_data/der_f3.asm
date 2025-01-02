section .data
    const_derf3 dq 5.0

section .text
global der_f3
der_f3:
    push ebp
    mov ebp, esp
    sub esp, 12
    fld qword[const_derf3]
    fld qword[ebp + 8]
    fld qword[ebp + 8]
    fmulp
    fdivp
    mov esp, ebp
    pop ebp
    ret