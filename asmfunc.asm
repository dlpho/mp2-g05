; LBYARCH S25H - G05 (Ho, Pangan)

; assembly part using x86-64

section .data
denom dq 255.0

section .text
bits 64
default rel ; to handle address relocation
global imgCvtGrayIntToDouble

imgCvtGrayIntToDouble:
    
    ; rcx = input
    ; rdx = output
    ; r8  = width
    ; r9  = height
    
    ; move input to eax, zero extended
    movzx eax, byte [rcx]
    
    ; convert to double prec float
    cvtsi2sd xmm0, eax
    
    ; get 255.0 from memory
    movsd xmm1, [denom]
    
    ; divide input / 255.0
    divsd xmm0, xmm1
    
    ; move quotient to output
    movsd [rdx], xmm0
   
    ret