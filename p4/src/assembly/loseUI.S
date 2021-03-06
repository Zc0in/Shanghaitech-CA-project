.data
    lose: .string "Score: "
.section .text
.globl loseUI
.type loseUI, @function
loseUI:
    addi sp, sp, -52
    sw ra, 0(sp)    # store ra
    sw s0, 4(sp)    # store s0
    sw s1, 8(sp)    # store s1
    sw s2, 12(sp)   # store s2
    sw s3, 16(sp)   # store s3
    sw s4, 20(sp)   # store s4
    sw s5, 24(sp)   # store s5
    sw s6, 28(sp)   # store s6
    sw s7, 32(sp)   # store s7
    sw s8, 36(sp)   # store s8
    sw s9, 40(sp)   # store s9
    sw s10, 44(sp)  # store s10
    sw s11, 48(sp)  # store s11

    # Save score arg
    mv s0, a0
    
    # Flush the whole screen
    li a0, 0        # black background
    jal LCD_Clear


    # draw rect
    li  a0, 0
    li  a1, 0
    li  a2, 159
    li  a3, 79
    li  a4, 0x07E0 # Color
    jal LCD_DrawRectangle

    # draw rect
    li  a0, 14
    li  a1, 14
    li  a2, 144
    li  a3, 64
    li  a4, 0x0000 # Color
    jal LCD_DrawRectangle

    # Show string
	la t0, lose
    li a0, 35       # set x-corrdinate
    li a1, 32       # set y-corrdinate
    mv a2, t0
    li a3, 0b1111100000000000   # Red
    jal LCD_ShowString

    # Show score
    li a0, 90      # set x-corrdinate
    li a1, 32       # set y-corrdinate
    mv a2, s0
    li a3, 4
    li a4, 0b1111100000000000   # Red
    jal LCD_ShowNum

    lw ra, 0(sp)    # load ra
    lw s0, 4(sp)    # load s0
    lw s1, 8(sp)    # load s1
    lw s2, 12(sp)   # load s2
    lw s3, 16(sp)   # load s3
    lw s4, 20(sp)   # load s4
    lw s5, 24(sp)   # load s5
    lw s6, 28(sp)   # load s6
    lw s7, 32(sp)   # load s7
    lw s8, 36(sp)   # load s8
    lw s9, 40(sp)   # load s9
    lw s10, 44(sp)  # load s10
    lw s11, 48(sp)  # load s11
    addi sp, sp, 52

    ret