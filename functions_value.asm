extern printf

section .rodata
    format_double: db "%f", 0
    
    x: dq 19.5
    const_f1_1: dq 0.6
    const_f1_2: dq 3.0

section .bss
    result: resq 1
    
section .text
global main
main:
    push dword[x + 4]
    push dword[x]
    call f1_value
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
    
; 0.6*x + 3
f1_value:
    push ebp
    mov ebp, esp
    
    mov eax, dword[ebp + 8] ; x
    finit
    
    fld qword[ebp + 8] ; x
    fmul qword[const_f1_1] ; 0.6*x
    fadd qword[const_f1_2] ; 0.6*x + 3
    
    fstp qword[ebp + 8]
   
    pop ebp
    ret

; (x - 2)^3 - 1
f2_value:

; 3 / x
f3_value:
