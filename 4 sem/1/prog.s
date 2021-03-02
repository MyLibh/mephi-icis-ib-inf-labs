// (a^4+b^3)/(a^2c-b^2d+e)

.data
a:
        .hword 1
b:
        .hword 0
c:
        .hword 0
d:
        .hword 0
e:
        .hword 0
res:
        .space 3

.text
        .global _start

_start:
        adr x0, a
        ldrsh w1, [x0]
        mul w2, w1, w1 // a^2

        adr x0, b
        ldrsh w1, [x0]
        mul w4, w1, w1 // b^2
        mul w5, w1, w4 // b^3

        madd w3, w2, w2, w5 // a^4+b^3
        bvs error

        adr x0, c
        ldrsh w1, [x0]
        adr x0, e
        ldrsh w5, [x0]
        madd w2, w2, w1, w5 // a^2c+e

        adr x0, d
        ldrsh w1, [x0]
        mul w4, w4, w1 // b^2d

        sub w2, w2, w4 // a^2c-b^2d+e
        bne error

        sdiv w3, w3, w4 // result

        adr x0, res
        str w3, [x0]

        mov x0, #0
        b exit

error:
        mov x0, #1

exit:
        mov x8, #93
        svc #0