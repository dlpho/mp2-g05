; LBYARCH S25H - G09 (Ho, Pangan)

; assembly part using x86-64

section .data
denom dq 255.0

section .text
bits 64
default rel ; to handle address relocation
global imgCvtGrayIntToDouble

imgCvtGrayIntToDouble:
    
    ; rcx = input[0] ptr
    ; rdx = output[0] ptr
    ; r8  = width
    ; r9  = height
    
    xor r10, r10 ; ctr
    movsd xmm1, [denom] ; 255.0
    mov r11, r8
    imul r11, r9 ; total pixels
    
    ; REMEMBER! input and output are internally
    ; 1d arrays, so ez looping

    looper:
        ; load pixel value, zero extend to 64 bits
        movzx rax, byte [rcx + r10]

        ; convert to double
        cvtsi2sd xmm0, rax

        ; pixel / 255.0
        divsd xmm0, xmm1

        ; move result to output (now 64-bits)
        movsd [rdx + r10 * 8], xmm0

        inc r10
        cmp r10, r11
        jl looper

    ret