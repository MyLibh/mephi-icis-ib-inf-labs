.text
.global proceed_image_asm

/*
    x0 - data
    x1 - width
    x2 - height
    x3 - channels_num
 */
proceed_image_asm:
    stp x29, x30, [sp, -16]!
	stp x27, x28, [sp, -16]!
	stp x25, x26, [sp, -16]!
	stp x23, x24, [sp, -16]!
	stp x21, x22, [sp, -16]!
	stp x19, x20, [sp, -16]!

    mov x20, x0 // data
    mov x21, x1 // width
    mov x22, x2 // height
    mov x23, x3 // channels_num
	mul x24, x1, x3 // width * channeld_num

    mul x0, x1, x2
    mul x0, x0, x3 // width * height * channels_num
    bl malloc

    mov x10, 0 // i
height_for_start:
    cmp x10, x22
    bge height_for_end

    mov x11, 0 // j
width_for_start:
    cmp x11, x21
    bge width_for_end

	mov x4, x10 // i idx
	mov x5, x11 // j idx
	mul x2, x4, x24 // i * width * channel_num
    madd x2, x5, x23, x2 // j * channels_num + x2

    cbz x10, if_border // if (!i)
    cbz x11, if_border // if (!j)

    add x6, x10, 1
    cmp x6, x22 // if (i + 1 == height)
    beq if_border

    add x6, x11, 1
    cmp x6, x21 // if (j + 1 == width)
    beq if_border

    // (i, j)
    ldr w2, [x20, x2]
    mov x9, 8

	lsr x3, x2, 24
	mul x13, x3, x9 // sum r
	lsr x3, x2, 16
	and x3, x3, 255
	mul x14, x3, x9 // sum g
	lsr x3, x2, 8
	and x3, x3, 255
	mul x15, x3, x9 // sum b
	and x3, x3, 255
	mul x16, x3, x9 // sum a

    // (i - 1, j)
    sub x4, x4, 1
    bl calc

    // (i + 1, j)
    add x4, x4, 2
    bl calc

    // (i + 1, j + 1)
    add x5, x5, 1 
    bl calc

    // (i + 1, j - 1)
    sub x5, x5, 2
    bl calc

    // (i, j - 1)
    sub x4, x4, 1
    bl calc

    // (i - 1, j - 1)
    sub x4, x4, 1
    bl calc

    // (i - 1, j + 1)
    add x5, x5, 2
    bl calc

    // (i, j + 1)
    add x4, x4, 1
    bl calc

    // (i, j)
	sub x5, x5, 1
	mul x2, x4, x24
	madd x2, x4, x23, x2

    // Clip
    mov x1, 0

    mov x12, x13
    bl clip

    mov x12, x14
    bl clip

    mov x12, x15
    bl clip

    mov x12, x16
    bl clip

    b L1

if_border:
    ldr w1, [x20, x2]

L1:
    str w1, [x0, x2]

    add x11, x11, 1 // ++j
    b width_for_start

width_for_end:
    add x10, x10, 1 // ++i
    b height_for_start

height_for_end:
    ldp x19, x20, [sp], 16
	ldp x21, x22, [sp], 16
	ldp x23, x24, [sp], 16
	ldp x25, x26, [sp], 16
	ldp x27, x28, [sp], 16
	ldp x29, x30, [sp], 16
    ret

calc:
    mul x2, x4, x24 // i * width * channel_num
    madd x2, x5, x23, x2 // j * channels_num + x2

    ldr w2, [x20, x2]

    lsr x3, x2, 24
    sub x13, x13, x3 // r
    lsr x3, x2, 16
	and x3, x3, 255
	sub x14, x14, x3 // g
	lsr x3, x2, 8
	and x3, x3, 255
	sub x15, x15, x3 // b
	and x3, x3, 255
	sub x16, x16, x3 // a
    ret

clip:
    cmp x12, 255
    bgt clip_up
    cmp x1, 0
    bge 9f

    mov x12, 0
    b 9f

clip_up:
    mov x12, 255

9:
    lsl x1, x1, 8
    orr x1, x1, x12
    ret

