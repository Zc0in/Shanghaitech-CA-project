.section .text
.globl draw
.type draw, @function
draw:
	# YOUR CODE HERE
	li a0, 70
	li a1, 70
	li a2, 10
	li a3, 0xFFFF
	jal LCD_DrawCircle

	li a0, 60
	li a1, 60
	li a2, 65
	li a3, 65
	li a4, 0xFFFF
	jal LCD_DrawRectangle

	li a0, 20
	li a1, 20
	li a2, 8
	li a3, 2
	li a4, 0X07FF
	jal LCD_ShowNum