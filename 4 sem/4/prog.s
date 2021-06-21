.data
MSG_INPUT: .asciz "Enter x, y, e:\n"
FMT_INPUT: .asciz "%lf %d %lf"
MSG_RESULT: .asciz "Result: %.20lf\n"
FMT_MEMBER: .asciz "%d %f\n"
MSG_WRONG_PARAMS: .asciz "Wrong input(less than 3 args or |x|<1)\n"

OUTPUT_FILE: .asciz "output.txt"
O_WRITE: .asciz "w"

OutputFD: .skip 4

.equ MAIN_STACK_SIZE, 40
.equ X_OFFSET, 16
.equ Y_OFFSET, 24
.equ E_OFFSET, 32

.text
.global main

main:
    sub sp, sp, MAIN_STACK_SIZE
    stp x29, x30, [sp]
    mov x29, sp

    ldr x0, =MSG_INPUT
    bl printf

    ldr x0, =FMT_INPUT
    add x1, x29, X_OFFSET
    add x2, x29, Y_OFFSET
    add x3, x29, E_OFFSET
    bl scanf

    cmp x0, 3
    bne error

    // check args
    fmov d1, 1.
    ldr d0, [x29, X_OFFSET]
    fcmp d0, d1
    bge error

    fadd d0, d0, d1
    ldrsw x0, [x29, Y_OFFSET]
    bl pow
    bl print_result

    ldr d0, [x29, X_OFFSET]
    ldrsw x0, [x29, Y_OFFSET]
    ldr d1, [x29, E_OFFSET]
    bl func
    bl print_result

    mov x0, 0
    b 9f

error:
    adr x0, stderr
    ldr x0, [x0]
    ldr x1, =MSG_WRONG_PARAMS
    bl fprintf

    mov x0, 1

9:
    ldp x29, x30, [sp]
    add sp, sp, MAIN_STACK_SIZE
    ret

print_result:
    str x30, [sp, -8]!

    ldr x0, =MSG_RESULT
    bl printf

    ldr x30, [sp], 8
    ret

/*
    d0 - x
    x0 - y

    @return
    d0 - x^y
 */
pow:
    str x30, [sp, -8]!

    cmp x0, 0
    bge 0f

    neg x0, x0
    mov x9, 1

0:
    fmov d1, d0
    fmov d0, 1.

1:
    cbz x0, 2f
    fmul d0, d0, d1
    sub x0, x0, 1
    b 1b

2:
    cmp x9, 1
    bne 9f

    fmov d1, 1.
    fdiv d0, d1, d0

9:
    ldr x30, [sp], 8
    ret

/*
    d0 - x
    d1 - eps
    x0 - y

    @return
    d0 - x^y
 */
func:
    str x30, [sp, -8]!

    bl open_output

    fmov d4, d0 // x
    fmov d0, 1.  // accumulator
    fmov d2, 1. // x^n
    fmov d7, 1. // up_coef
    fmov d8, 1. // down_coef
    fmov d9, 1. // n
    scvtf d10, x0

1:
    fmul d2, d2, d4 // x^n
    fmul d7, d7, d10 // up coef
    fmul d8, d8, d9 // down coef

    //scvtf d5, x1
    //scvtf d6, x2
    fdiv d3, d7, d8

    fmov d11, 1.
    fsub d10, d10, d11 // y
    fadd d9, d9, d11 // n

    fmul d5, d3, d2 // member

    fabs d6, d5
    fcmp d1, d6
    bge 9f // if eps >= |member|

    stp d10, d7, [sp, -16]!
    stp d8, d9, [sp, -16]!
    stp d0, d1, [sp, -16]!
    stp d2, d4, [sp, -16]!
    str d5, [sp, -8]!

    //mov x0, x3
    fcvtzs x0, d9
    sub x0, x0, 1

    fmov d0, d5

    bl save_member

    ldr d5, [sp], 8
    ldp d2, d4, [sp], 16
    ldp d0, d1, [sp], 16
    ldp d8, d9, [sp], 16
    ldp d10, d7, [sp], 16

    fadd d0, d0, d5
    b 1b

9:
    bl close_output

    ldr x30, [sp], 8
    ret

/*
    x0 - n
    d0 - member

*/
save_member:
    str x30, [sp, -8]!

    mov x2, x0
    // adr x0, stdout
    ldr x0, =OutputFD
    ldr x0, [x0]
    ldr x1, =FMT_MEMBER
    bl fprintf

    ldr x30, [sp], 8
    ret


open_output:
    stp x30, x0, [sp, -16]!
    str x1, [sp, -8]!

    ldr x0, =OUTPUT_FILE
    ldr x1, =O_WRITE
    bl fopen

    ldr x1, =OutputFD
    str x0, [x1]

    ldr x1, [sp], 8
    ldp x30, x0, [sp], 16
    ret

close_output:
    stp x30, x0, [sp, -16]!
    str x1, [sp, -8]!


    ldr x0, =OutputFD
    ldr x0, [x0]
    bl fclose

    mov x0, 0
    ldr x1, =OutputFD
    str x0, [x1]

    ldr x1, [sp], 8
    ldp x30, x0, [sp], 16
    ret
