
.intel_syntax noprefix
.section .data
.align 32
one:        .single 1.0, 1.0, 1.0, 1.0
zero:       .float 0

.section    .text
.globl formula2

formula2:
    push rbp
    mov rbp, rsp
    vxorps  xmm0, xmm0, xmm0    # init return xmm function
    cmp rdx, 1                  #the length is less than 1
    jl .done
    vxorps  xmm7, xmm7, xmm7    # init for result of sum(x*y)
    vxorps  xmm8, xmm8, xmm8    # init for result of mul(x*x + y*y - 2xy + 1)
    vmovss  xmm8, [one]
    mov     r8, rdx             # length of float array
    xor     r9, r9              # init float offset
    xor     r10, r10            # init loop counter

.remainder:
    # calculate numenator

    vmovaps  xmm11, [one]
    vmovaps xmm0, [rdi + r9]         # store x in xmm0
    vmovaps xmm1, [rsi + r9]         # store y in xmm1
    vmulps  xmm2, xmm0, xmm1         # calc xy
    vsubps  xmm4, xmm0, xmm1         # calc x-y
    vfmadd231ps xmm11, xmm4, xmm4    #xmm11 = x*x -2*x*y + y*y + 1

    #calc the numenator xy sum
    vshufps xmm3, xmm2, xmm2, 0x4E    # (x1, x0, x3, x2)
    vaddps  xmm2, xmm3, xmm2          # store sum of pairs (x3 + x1, x2 + x0, x1+ x3, x0 + x2)
    vshufps xmm3, xmm2, xmm2, 0XB1    # (x2, x3, x0, x1)
    vaddps  xmm2, xmm3, xmm2          # store sum of quad x0 + x1 + x2 + x3
    vaddss  xmm7, xmm7, xmm2          #adding with the prev sum

    #calc the denumenator multipycation
    vshufps xmm5, xmm11, xmm11, 0X4E    # (x1, x0, x3, x2)
    vmulps  xmm11, xmm5, xmm11          # store sum of pairs (x3 + x1, x2 + x0, x1+ x3, x0 + x2)
    vshufps xmm5, xmm11, xmm11, 0XB1    # (x2, x3, x0, x1)
    vmulps  xmm11, xmm5, xmm11          # store sum of quad x0 + x1 + x2 + x3
    vmulss  xmm8, xmm8, xmm11           #multiply with the prev multipication
    add     r9, 16
    add     r10, 4
    cmp     r10, rdx                    # counter - length > 0 then jump to end
    jl     .remainder
    comiss xmm8, xmmword[zero]          # if there is o in the denumenator not div
    je .done
    # calc numenator / denumenator
    vdivss  xmm0, xmm7, xmm8
    vmovd   rax, xmm0
 .done:
     mov rsp, rbp
     pop rbp
     ret
     
