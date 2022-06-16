.section .text
.globl GroundObstacle
.type GroundObstacle, @function
GroundObstacle:
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

    # Draw Ground Obstacle
    loop:
    bne t0, x0, next
    li t0, 159
    addi t1, t0, 10       # the width of obstacle 
    mv a0, t0        # x_sta
    li a1, 40       # y_sta
    mv a2, t1         # x_end
    li a3, 80       # y_end
    li a4, 0x7BEF   # dark-grey board
    jal LCD_Fill
    addi t0, t0, 1      # loop factor
    j loop          # loop

    next:
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
