
section .rodata
    const_f1_1: dq 0.6
    const_f1_2: dq 3.0
    
    const_f2_1: dq 2.0
    const_f2_2: dq 1.0
    
    const_f3_1: dq 3.0


section .bss
    result: resq 1

        
section .text
global f1_value, f2_value, f3_value

; (0.6*x + 3) function
f1_value:
    push ebp
    mov ebp, esp
    
    finit ; start and clear x87
    
    fld qword[ebp + 8] ; x
    fmul qword[const_f1_1] ; 0.6*x
    fadd qword[const_f1_2] ; 0.6*x + 3
    
    ; value returned in st0
    
    pop ebp
    ret


; (x - 2)^3 - 1
f2_value:
    push ebp
    mov ebp, esp
    
    finit ; start and clear x87
    
    fld qword[ebp + 8] ; x
    fsub qword[const_f2_1] ; x - 2
    fld st0 ; clone (x - 2)
    fld st0 ; clone (x - 2)
    fmulp ; (x - 2)^2
    fmulp ; (x - 2)^3
    fsub qword[const_f2_2] ; (x - 2)^3 - 1
    
    ; value returned in st0
   
    pop ebp
    ret

        
; 3 / x
f3_value:
    push ebp
    mov ebp, esp
    
    finit ; start and clear x87
    
    fld qword[const_f3_1] ; 3
    fdiv qword[ebp + 8] ; 3 / x
 
    ; value returned in st0
    
    pop ebp
    ret