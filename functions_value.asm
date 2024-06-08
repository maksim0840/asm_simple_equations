extern printf

section .rodata
    format_double: db "%f", 0
    x: dq 0.03
    
    const_f1_1: dq 0.6
    const_f1_2: dq 3.0
    
    const_f2_1: dq 2.0
    const_f2_2: dq 1.0
    
    const_f3_1: dq 3.0

section .bss
    result: resq 1
    
section .text
global main
main:
    push dword[x + 4]
    push dword[x]
    call f3_value
    pop dword[result]
    pop dword[result + 4]
    
    sub esp, 4
    push dword[result + 4]
    push dword[result]
    push format_double
    call printf
    add esp, 16
    
    xor eax, eax
    ret
    
; (0.6*x + 3) function
f1_value:
    push ebp
    mov ebp, esp
    
    finit
    
    fld qword[ebp + 8] ; x
    fmul qword[const_f1_1] ; 0.6*x
    fadd qword[const_f1_2] ; 0.6*x + 3
    
    fstp qword[ebp + 8]
   
    pop ebp
    ret

; (x - 2)^3 - 1
f2_value:
    push ebp
    mov ebp, esp
    
    finit
    
    fld qword[ebp + 8] ; x
    fsub qword[const_f2_1] ; x - 2
    fld st0 ; clone (x - 2)
    fld st0 ; clone (x - 2)
    fmulp ; (x - 2)^2
    fmulp ; (x - 2)^3
    fsub qword[const_f2_2] ; (x - 2)^3 - 1
    
    fstp qword[ebp + 8]
   
    pop ebp
    ret
    
; 3 / x
f3_value:
    push ebp
    mov ebp, esp
    
    finit
    
    fld qword[const_f3_1] ; 3
    fdiv qword[ebp + 8] ; 3 / x
 
    fstp qword[ebp + 8]
    
    pop ebp
    ret