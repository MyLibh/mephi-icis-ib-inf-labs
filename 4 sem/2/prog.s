.data
n: .byte 3
m: .byte 3

.align 2
matrix: .word 2, 1, 3, 4, 6, 5, 9, 8, 7

.text
.global _start

_start:
        adr x0, matrix
        adr x3, n
        ldrb w1, [x3]
        adr x3, m
        ldrb w2, [x3]
        bl _mqsort

        mov x0, 0
        mov x8, 93
        svc 0

// x0 - addr
// w1 - n
// w2 - m
_mqsort:
        stp x0, x30, [sp, -16]!
        stp w3, w4, [sp, -8]!

        mov w4, w1
        mov w1, 0
        mov w3, 0

1:
        bl _qsort
        add w3, w3, 1
        add x0, x0, w2, uxtw 2
        cmp w3, w4
        blt 1b

        ldp w3, w4, [sp], 8
        ldp x0, x30, [sp], 16
        ret
        
// x0 - addr
// w1 - start idx
// w2 - size
_qsort:
        str x30, [sp, -8]!
        stp w1, w2, [sp, -8]!

        sub w2, w2, 1 // w2 = size - 1

        cmp w1, w2
        bge 1f

        str w2, [sp, -4]!
        str x0, [sp, -8]!
        bl _make_partition

        mov w2, w0
        ldr x0, [sp], 8
        bl _qsort // [start - w2]

        add w1, w2, 1 // w1 = part + 1
        ldr w2, [sp], 4
        add w2, w2, 1
        bl _qsort // [w2+1 - end]

1:
        ldp w1, w2, [sp], 8
        ldr x30, [sp], 8
        ret

// x0 - addr
// w1 - first
// w2 - last
_make_partition:
        str x30, [sp, -8]!

        ldr w8, [x0, w2, uxtw 2] // w8 = *(matrix + size * 4)
        mov w9, w1 // w4 = first (part)
        mov w10, w1 // w5 = first

1:
        ldr w11, [x0, w10, uxtw 2] // w11 = *(matrix + first * 4)
        cmp w11, w8
        bge 2f

        ldr w12, [x0, w9, uxtw 2]
        str w11, [x0, w9, uxtw 2]
        str w12, [x0, w10, uxtw 2]

        add w9, w9, 1 // part++

2:
        add w10, w10, 1 // first++

        cmp w10, w2 // first < last ?
        blt 1b

        ldr w12, [x0, w9, uxtw 2]
        str w8, [x0, w9, uxtw 2]
        str w12, [x0, w2, uxtw 2]

        mov w0, w9 // partition idx

        ldr x30, [sp], 8
        ret