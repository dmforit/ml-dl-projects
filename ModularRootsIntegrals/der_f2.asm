section .data
    const1_f2 dq -2.0
   
section .text
global der_f2
der_f2:
    push ebp
    mov ebp, esp
    fld qword[const1_f2]
    mov esp, ebp
    pop ebp
    ret
    

