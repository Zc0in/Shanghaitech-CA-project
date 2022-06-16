#   Project 1.2 RVC instructions to RISC-V instructions in RISC-V

#   You can suppose you will get the number of machine codes you should translate from 
#   "lines_of_rvc_codes" and get the 16 or 32 bits-machine codes from "rvc_codes". 
#   Both of them are stored in file "input.S"

#   When we test your project, we will change the input files content.

# DO NOT REMOVE THIS LINE WHEN SUBMITTING
.import input.S

# Your code to convert the RVC codes to RVI codes.
.text
initialization:
la s1 rvc_codes # load the address of rvc_codes to s1
lw s2 lines_of_rvc_codes # load lines_of_rvc_codes to s2
slli t1 s2 2 # t1 = lines_of_rvc_codes * 4
sub sp sp t1 # give sp the space to store order 
add a2 x0 sp # let a2 be the order array
sub sp sp t1 # give sp the space to store order
addi sp sp -4 # set (n+1) length for length array
sw x0 0(sp) # set length array[0] be 0
addi a3 sp 4 # let a3 be the order length array(2 or 4 bytes)
li t1 -1 # int t1 = 0
li a6 0 #init a6 be 0

loop_to_load_order:
addi t1 t1 1 # t1 += 1
blt t1 s2 load_data # if t1 < lines_of_rvc_codes then load the order
bge t1 s2 order_process_init # initialize some para to prepare for the data process

load_data:
lw t4 0(s1) # load 32 bits data from s1

check_16_or_32:
slli t2 t4 30 # get the order[1:0]
li t3 0xC0000000 # t3 = 0xC0000000
beq t2 t3 is_32 # if the end of an order is 11, then it is 32
bne t2 t3 not_32 # if the end of an order is not 11, then it is not 32

is_32:
slli t5 t1 2 # 4 * t1
add t6 t5 a3 # shift a3 to store order length
add t5 t5 a2 # shift a2 to store order
sw t4 0(t5)# load the 32 bits order to a2
addi a6 a6 4# a6 += 4 as temp_int
sw a6 0(t6)# load the length 4 to a3(32 bits)
addi s1 s1 4 # shift left s1 for 4 bytes
j loop_to_load_order # back to loop to continue load

not_32:
srli t2 t4 16 # get the order[31:16]
slli t2 t2 30 # get the order[17:16]
li t3 0xC0000000 # t3 = 0xC0000000
beq t2 t3 16_and_half_32 # if the [17:16] of an order is 11, then it is a combination of 16 and half 32
bne t2 t3 two_16 # if the [17:16] of an order is not 11, then it has two 16 bits orders

16_and_half_32:
slli t5 t1 2 # 4 * t1
add t6 t5 a3 # shift a3 to store order length
add t5 t5 a2 # shift a2 to store order
slli t4 t4 16 # load the 16 bits order
srli t4 t4 16 # load the 16 bits order
sw t4 0(t5)# load the 16 bits order to a2
addi a6 a6 2# a6 += 2 as temp_int
sw a6 0(t6)# load the length 2 to a3(16 bits)
addi s1 s1 2 # shift left s1 for 2 bytes
j loop_to_load_order # back to loop to continue load

two_16:
slli t5 t1 2 # 4 * t1
add t6 t5 a3 # shift a3 to store order length
addi a6 a6 2# a6 += 2 as temp_int
sw a6 0(t6)# load the length 2 to a3(16 bits)
add t5 t5 a2 # shift a2 to store order
slli t6 t4 16 # load the 16 bits order
srli t6 t6 16 # load the 16 bits order
sw t6 0(t5)# load the 16 bits order to a2
addi t1 t1 1 # t1 += 1
bge t1 s2 order_process_init # initialize some para to prepare for the data process
slli t5 t1 2 # 4 * t1
add t6 t5 a3 # shift a3 to store order length
addi a6 a6 2# a6 += 2 as temp_int
sw a6 0(t6)# load the length 2 to a3(16 bits)
add t5 t5 a2 # shift a2 to store order
srli t6 t4 16 # load the 16 bits order
sw t6 0(t5)# load the 16 bits order to a2
addi s1 s1 4 # shift left s1 for 4 bytes
j loop_to_load_order # back to loop to continue load

order_process_init:
li t1 -1 # int t1 = 0
addi a3 a3 -4 # change a3 to the zero position 
addi a6 x0 1 #set a contant 1

order_process_loop:
addi t1 t1 1 # t1 += 1
blt t1 s2 order_process # if t1 < lines_of_rvc_codes then process the order
bge t1 s2 exit_program_0 # exit the program 

order_process:
slli t2 t1 2 # 4 * t1
add t2 t2 a2 # shift a2 to load order
lw a4 0(t2)# load the 16 bits order to a2
slli t4 a4 30 # get the order[1:0]
li t5 0xC0000000 # t5 = 0xC0000000
beq t4 t5 judge_the_type_32 # if the end of an order is 11, then it is 32
bne t4 t5 judge_the_type_16 # if the end of an order is not 11, then it is 16

judge_the_type_32: # change only happened when the order have the offset(branch and jump)
addi sp sp -20 # adjust stack for 5 items
sw t5 16(sp) # save t4 for use afterwards
sw t4 12(sp) # save t4 for use afterwards
sw t3 8(sp) # save t3 for use afterwards
sw t2 4(sp) # save t2 for use afterwards
sw t1 0(sp) # save t1 for use afterwards

andi t1 a4 0b1111111 # get the opcode
li t2 0b1100011 # the opcode of branch
li t3 0b1101111 # the opcode of jump
beq t1 t2 update_offset_32_branch # update the offset of 32 bits branch
beq t1 t3 update_offset_32_jump # update the offset of 32 bits jump
j print_32_order# jump to print

print_32_order:
li a0 1 # print_int
li t1 0x80000000 # the mask
li t2 -1 # counter
li t3 32 # total length

print_order_loop:
addi t2 t2 1 # t2 += 1
blt t2 t3 print_order # if t2 < 32 go print_order
j print_enter # jump to print '\n'

print_order:
and t4 a4 t1
srli a1 t4 31 # get the left num
ecall
slli a4 a4 1 # shift left to update the left num
j print_order_loop # jump back to print_order_loop

print_enter:
li a1 '\n' # print '\n'
li a0 11 # print_character
ecall
j exit_judge_the_type_32 # exit the function

exit_judge_the_type_32:
lw t1 0(sp) # restore t1
lw t2 4(sp) # restore t2
lw t3 8(sp) # restore t3
lw t4 12(sp) # restore t4
lw t5 16(sp) # restore t4
addi sp sp 20 # adjust stack to delete 2 items
j order_process_loop # jump back to calling routine

# get offset
update_offset_32_branch:
li s3 0 #let s3 be the offset
srli t1 a4 31 #get num[31]
ble t1 x0 postive_offset_12 #if imm[12] == 0
slli t1 t1 12 #negative offset
sub s3 s3 t1 #sub the negative part

postive_offset_12:
li a6 0b1 #reset a6
slli t2 a6 7 # the imm[11] positon
and t1 a4 t2 # get imm[11]
slli t1 t1 4 # move to the right position
add s3 s3 t1 # add positive imm[11]
srli t1 a4 25 #the first 7 position
andi t1 t1 0b111111 #get the imm[10:5]
slli t1 t1 5 # move to the right position
add s3 s3 t1 # add positive imm[10:5]
srli t1 a4 8 # the imm[1] position
andi t1 t1 0b1111 #get imm[4:1]
slli t1 t1 1 #move to the right positon
add s3 s3 t1 # add positive imm[4:1] and imm[0] = 0
lw t3 0(sp)# get the order position
slli t3 t3 2 # 4 * t3
add t3 t3 a3 # get the address of the order
lw t1 0(t3) # let t1 be the address of the order
jal ra change_offset #change the offset to correct one
li a6 1 # a6 = 1
slli t2 a6 11 # t2 = 0x800
and t1 s3 t2 #the imm[11]
andi a4 a4 0xFFFFFF7F # let num[7] be 0
srli t1 t1 4 #move imm[11] to num[7]
or a4 a4 t1 #change imm[11]
andi t1 s3 0x7E0 #the imm[10:5]
li a6 0b111111 #a6 = 0x111111
slli t2 a6 25 #move a6 to num[30:25]
not t2 t2 #t2 = 0x81FFFFFF
and a4 a4 t2 # let num[30:25] be 0
slli t1 t1 20 #move imm[10:5] to num[30:25]
or a4 a4 t1 #change imm[10:5]
andi t1 s3 0x1E #the imm[4:1]
li a6 0b1111 #a6 = 0x1111
slli t2 a6 8 #move a6 to num[11:8]
not t2 t2 #t2 = 0xFFFFF0FF
and a4 a4 t2# let num[11:8] be 0
slli t1 t1 7 #move imm[4:1] to num[11:8]
or a4 a4 t1 #change imm[4:1]
j print_32_order # jump to print

update_offset_32_jump:
# get offset
li s3 0 # let s3 be the offset
srli t1 a4 31 # get the high positon 
slli t1 t1 20 # get the imm[20] 
beq t1 x0 postive_offset_20 #if imm[20] == 0
sub s3 s3 t1 #sub the negative part

postive_offset_20:
# imm[19:12]
srli t1 a4 12 # get num[19:12]
andi t1 t1 0b11111111 # get num[19:12]
slli t1 t1 12 #imm[19:12]
add s3 s3 t1 # add positive imm[19:12]
# imm[11]
srli t1 a4 20 # get num[20]
andi t1 t1 0b1 # get num[20]
slli t1 t1 11 #imm[11]
add s3 s3 t1 # add positive imm[11]
# imm[10:1]
srli t1 a4 21 # get num[31:21]
andi t1 t1 0b1111111111 # get num[20:21]
slli t1 t1 1 #imm[10:1]
add s3 s3 t1 # add positive imm[10:1]
# get address
lw t3 0(sp)# get the order position
slli t3 t3 2 # 4 * t3
add t3 t3 a3 # get the address of the order
lw t1 0(t3) # let t1 be the address of the order
# s3 is offset
# t1 is the address of the order
# t3 is the address of the length array
jal ra change_offset #change the offset to correct one
# imm[19:12]
li a6 0b11111111 #a6 = 0x11111111
slli t2 a6 12 #move a6 to num[19:12]
not t2 t2 #t2 = 0xFFF00FFF
and a4 a4 t2 # let num[19:12] be 0
srli t1 s3 12 # t1 = imm[20:12]
and t1 t1 a6 # t1 = imm[19:12]
slli t1 t1 12 # move t1 to the right position 
or a4 a4 t1 #change imm[19:12]
# imm[11]
li a6 0b1 #a6 = 0x1
slli t2 a6 20 #move a6 to num[20]
not t2 t2 #t2 = 0xFFEFFFFF
and a4 a4 t2 # let num[20] be 0
srli t1 s3 11 # t1 = imm[11]
and t1 t1 a6 # t1 = imm[11]
slli t1 t1 20 # move t1 to the right position
or a4 a4 t1 #change imm[11] to num[20]
#imm[10:1]
li a6 0b1111111111 #a6 = 0x11111111
slli t2 a6 21 #move a6 to num[30:21]
not t2 t2 #t2 = 0x801FFFFF
and a4 a4 t2 # let num[30:21] be 0
srli t1 s3 1 # t1 = imm[20:1]
and t1 t1 a6 # t1 = imm[10:1]
slli t1 t1 21 # move t1 to the right position 
or a4 a4 t1 #change imm[10:1] to num[30:21]
# print
j print_32_order # jump to print

judge_the_type_16:
addi sp sp -20 # adjust stack for 5 items
sw t4 16(sp) # save t4 for use afterward
sw t4 12(sp) # save t4 for use afterwards
sw t3 8(sp) # save t3 for use afterwards
sw t2 4(sp) # save t2 for use afterwards
sw t1 0(sp) # save t1 for use afterwards

check_opcode:
andi t1 a4 0b11 # get the opcode
li t2 0b10 # add jalr slli
li t3 0b01 # addi lui and or xor sub beq bne srli srai andi jal
li t4 0b00 # lw sw
beq t1 t2 further_check_1 # add jalr slli
beq t1 t3 further_check_2 # addi lui and or xor sub beq bne srli srai andi jal
beq t1 t4 further_check_3 # lw sw

further_check_1:
srli t1 a4 13 # get num[15:13]
li t2 0b000 # set t2 0b000 to check
beq t1 t2 slli_process # the order is slli rd, rd, shamt
bne t1 t2 not_slli # the order is not slli rd, rd, shamt

slli_process:
andi t1 a4 0b1111111 # get num[6:0]
srli t1 t1 2 # get shamt[4:0]
srli t2 a4 7 # shift order 7 bits
andi t2 t2 0b11111 # get rd
slli t1 t1 5 # shift left 5 bits
add t1 t1 t2 # 0000000 shamt[4:0] rs1
slli t1 t1 3 # shift left 3 bits
addi t1 t1 1 # 0000000 shamt[4:0] rs1 001
slli t1 t1 5 # shift left 5 bits
add t1 t1 t2 # 0000000 shamt[4:0] rs1 001 rd
slli t1 t1 7 # shift left 57 bits
addi t1 t1 0b0010011 # 0000000 shamt[4:0] rs1 001 rd 0010011
add a4 x0 t1 # store 32 bits order to a4
j print_32_order # jump to print

not_slli:
andi t1 a4 0b1111111 # get num[6:0]
li t2 0b0000010 # judge num[6:0]
beq t1 t2 jalr_check # the order is jalr
bne t1 t2 add_check # the order is add

jalr_check:
srli t1 a4 12 # get num[15:12]
li t2 0b1000 # judge x0
li t3 0b1001 # judge x1
beq t1 t2 rd_00000 # rd = 00000
beq t1 t3 rd_00001 # rd = 00001

rd_00000:
li t2 0b00000 # rd = 00000
j jalr_process # jalr_process

rd_00001:
li t2 0b00001 # rd = 00001
j jalr_process # jalr_process

jalr_process:
srli t1 a4 7 # shift order 7 bits
andi t1 t1 0b11111 # get rs1
slli t1 t1 8 # shift order 8 bits
add t1 t1 t2 # 000000000000 rs1 000 00000/1
slli t1 t1 7 # shift order 7 bits
addi t1 t1 0b1100111 # 000000000000 rs1 000 00000/1 1100111
add a4 x0 t1 # store 32 bits order to a4
j print_32_order # jump to print

add_check:
srli t1 a4 12 # get num[15:12]
li t2 0b1000 # judge x0
li t3 0b1001 # judge x1
beq t1 t2 rs1_00000 # rs1 = 00000
beq t1 t3 rs1_rd # rs1 = rd

rs1_00000:
srli t3 a4 7 # shift order 7 bits
andi t3 t3 0b11111 # get rd
li t2 0b00000 # rs1 = 00000
j add_process # add_process

rs1_rd:
srli t3 a4 7 # shift order 7 bits
andi t3 t3 0b11111 # get rd
add t2 x0 t3 # rs1 = rd
j add_process # add_process

add_process:
srli t1 a4 2 # shift order 2 bits
andi t1 t1 0b11111 # get rs2
slli t1 t1 5 # shift order 5 bits
add t1 t1 t2 # 0000000 rs2 rs1
slli t1 t1 8 # shift order 8 bits
add t1 t1 t3 # 0000000 rs2 rs1 000 rd
slli t1 t1 7 # shift order 7 bits
addi t1 t1 0b0110011 # 0000000 rs2 rs1 000 rd 0110011
add a4 x0 t1 # store 32 bits order to a4
j print_32_order # jump to print

exit_program_0:
j exit_program # in case the branch is too far

further_check_2:
srli t1 a4 10 # get num[15:10]
li t2 0b100011 # judge and or xor sub
beq t1 t2 is_and_or_xor_sub # the order is and or xor sub
bne t1 t2 not_and_or_xor_sub # the order is not and or xor sub

is_and_or_xor_sub:
srli t1 a4 2 # shift 2 bits
andi t1 t1 0b111 # get rs2
srli t2 a4 7 # shift 7 bits
andi t2 t2 0b111 # get rd
srli t3 a4 5 # shift 5 bits
andi t3 t3 0b11 # get flag to check the order
li t4 0b11 # and
li t5 0b10 # or
li t6 0b01 # xor
beq t3 t4 and_process # is and
beq t3 t5 or_process # is or
beq t3 t6 xor_process # is xor
j sub_process # else is sub

and_process:
li t3 0b01 # set t3 0b01
slli t3 t3 3 # shift 3 bits
add t1 t1 t3 # 01 rs2(3)
slli t1 t1 2 # shift 2 bits
addi t1 t1 0b01 # 0000000 rs2 01
slli t1 t1 3 # shift 3 bits
add t1 t1 t2 # 0000000 rs2 01 rs1(3)
slli t1 t1 3 # shift 3 bits
addi t1 t1 0b111 # 0000000 rs2 rs1 111
slli t1 t1 2 # shift 2 bits
addi t1 t1 0b01 # 0000000 rs2 rs1 111 01
slli t1 t1 3 # shift 3 bits
add t1 t1 t2 # 0000000 rs2 rs1 111 01 rd(3)
slli t1 t1 7 # shift 7 bits
addi t1 t1 0b0110011 # 0000000 rs2 rs1 111 rd 0110011
add a4 x0 t1 # store 32 bits order to a4
j print_32_order # jump to print

or_process:
li t3 0b01 # set t3 0b01
slli t3 t3 3 # shift 3 bits
add t1 t1 t3 # 01 rs2(3)
slli t1 t1 2 # shift 2 bits
addi t1 t1 0b01 # 0000000 rs2 01
slli t1 t1 3 # shift 3 bits
add t1 t1 t2 # 0000000 rs2 01 rs1(3)
slli t1 t1 3 # shift 3 bits
addi t1 t1 0b110 # 0000000 rs2 rs1 110
slli t1 t1 2 # shift 2 bits
addi t1 t1 0b01 # 0000000 rs2 rs1 110 01
slli t1 t1 3 # shift 3 bits
add t1 t1 t2 # 0000000 rs2 rs1 110 01 rd(3)
slli t1 t1 7 # shift 7 bits
addi t1 t1 0b0110011 # 0000000 rs2 rs1 110 rd 0110011
add a4 x0 t1 # store 32 bits order to a4
j print_32_order # jump to print

xor_process:
li t3 0b01 # set t3 0b01
slli t3 t3 3 # shift 3 bits
add t1 t1 t3 # 01 rs2(3)
slli t1 t1 2 # shift 2 bits
addi t1 t1 0b01 # 0000000 rs2 01
slli t1 t1 3 # shift 3 bits
add t1 t1 t2 # 0000000 rs2 01 rs1(3)
slli t1 t1 3 # shift 3 bits
addi t1 t1 0b100 # 0000000 rs2 rs1 100
slli t1 t1 2 # shift 2 bits
addi t1 t1 0b01 # 0000000 rs2 rs1 100 01
slli t1 t1 3 # shift 3 bits
add t1 t1 t2 # 0000000 rs2 rs1 100 01 rd(3)
slli t1 t1 7 # shift 7 bits
addi t1 t1 0b0110011 # 0000000 rs2 rs1 100 rd 0110011
add a4 x0 t1 # store 32 bits order to a4
j print_32_order # jump to print

sub_process:
li t3 0b010000001 # set t3 0b0100000 01
slli t3 t3 3 # shift 3 bits
add t1 t1 t3 # 0100000 01 rs2(3)
slli t1 t1 2 # shift 2 bits
addi t1 t1 0b01 # 0100000 rs2 01
slli t1 t1 3 # shift 3 bits
add t1 t1 t2 # 0100000 rs2 01 rs1(3)
slli t1 t1 3 # shift 3 bits
addi t1 t1 0b000 # 0100000 rs2 rs1 000
slli t1 t1 2 # shift 2 bits
addi t1 t1 0b01 # 0100000 rs2 rs1 000 01
slli t1 t1 3 # shift 3 bits
add t1 t1 t2 # 0100000 rs2 rs1 000 01 rd(3)
slli t1 t1 7 # shift 7 bits
addi t1 t1 0b0110011 # 0100000 rs2 rs1 000 rd 0110011
add a4 x0 t1 # store 32 bits order to a4
j print_32_order # jump to print

not_and_or_xor_sub:
srli t1 a4 13 # shift 13 bits
li t2 0b100 # judge srli srai andi
beq t1 t2 is_srli_srai_andi # is srli srai andi
bne t1 t2 not_srli_srai_andi # not srli srai andi

is_srli_srai_andi:
srli t1 a4 2 # shift 2 bits
andi t1 t1 0b11111 # get shamt[4:0]/imm[4:0]
srli t2 a4 7 # shift 7 bits
andi t2 t2 0b111 # get rd
srli t3 a4 10 # shift 10 bits
andi t3 t3 0b11 # get flag to check the order
li t4 0b10 # andi
beq t3 t4 andi_process # is andi
bne t3 t4 srli_srai_process # is srli or srai

srli_srai_process:
slli t3 t3 10 # shift 10 bits
add t3 t3 t1 # 00/01 00000 shamt[4:0]
slli t3 t3 2 # shift 2 bits
addi t3 t3 0b01 # 00/01 00000 shamt[4:0] 01
slli t3 t3 3 # shift 3 bits
add t3 t3 t2 # 00/01 00000 shamt[4:0] 01 rs1(3)
slli t3 t3 3 # shift 3 bits
addi t3 t3 0b101 # 00/01 00000 shamt[4:0] rs1 101
slli t3 t3 2 # shift 2 bits
addi t3 t3 0b01 # 00/01 00000 shamt[4:0] rs1 101 01
slli t3 t3 3 # shift 3 bits
add t3 t3 t2 # 00/01 00000 shamt[4:0] rs1 101 01 rd(3)
slli t3 t3 7 # shift 7 bits
addi t3 t3 0b0010011 # 00/01 00000 shamt[4:0] rs1 101 rd 0010011
add a4 x0 t3 # store 32 bits order to a4
j print_32_order # jump to print

andi_process:
srli t4 a4 12 # shift 12 bits
andi t4 t4 0b1 # get imm[5]
li t5 0b1 # judge the sign bit
beq t4 t5 sign_bit_1 # sign bit is 1
bne t4 t5 sign_bit_0 # sign bit is 0

sign_bit_1:
li t4 0b1111111 # set sign bit imm[11:5]
j andi_process_continue # jump to continue andi process

sign_bit_0:
li t4 0b0000000 # set sign bit imm[11:5]
j andi_process_continue # jump to continue andi process

andi_process_continue:
slli t4 t4 5 # shift 5 bits
add t4 t4 t1 # imm[11:0]
slli t4 t4 2 # shift 3 bits
addi t4 t4 0b01 # imm[11:0] 01
slli t4 t4 3 # shift 3 bits
add t4 t4 t2 # imm[11:0] 01 rs1(3)
slli t4 t4 3 # shift 3 bits
addi t4 t4 0b111 # imm[11:0] rs1 111
slli t4 t4 2 # shift 2 bits
addi t4 t4 0b01 # imm[11:0] rs1 111 01 
slli t4 t4 3 # shift 3 bits
add t4 t4 t2 # imm[11:0] rs1 111 01 rd(3)
slli t4 t4 7 # shift 7 bits
addi t4 t4 0b0010011 # imm[11:0] rs1 111 rd 0010011
add a4 x0 t4 # store 32 bits order to a4
j print_32_order # jump to print

not_srli_srai_andi:
srli t1 a4 13  #get num[15: 13]
li t3 0b010 #funct of addi x0
beq t1 t3 addi_x0_process # the order is addi x0
li t3 0b011 #funct of addi
beq t1 t3 lui_process # the order is lui
li t3 0b000 #funct of addi rd
beq t1 t3 addi_rd_process # the order is addi rd
li t3 0b110 #funct of beq
beq t1 t3 beq_process # the order is beq
li t3 0b111 #funct of bne
beq t1 t3 bne_process # the order is bne
li t3 0b101 #funct of jal x0
beq t1 t3 jal_x0_process # the order is jal x0
li t3 0b001 #funct of 001
beq t1 t3 jal_x1_process # the order is jal x1

addi_x0_process:
li t2 0 # set t2 be sero
srli t1 a4 12 # get num[15:12]
andi t1 t1 0b1 # get imm[5]
beq t1 x0 pos_addi_x0 # positive imm
li t3 0b1111111 # imm[11:5]
slli t3 t3 25 #imm[31:25]
add t2 t2 t3 # change num[31:25]

pos_addi_x0:
# 16num[6:2]-> imm[4:0] -> 32num[24:20]
srli t1 a4 2 # get num[31:2]
andi t3 t1 0b11111 # get imm[4:0]
slli t3 t3 20 #num[24:20]
add t2 t2 t3 # change num[24:20]
# num[19:12] is zero
# 16num[11:7] -> rd(5) -> 32num[11:7]
srli t1 a4 7 # get num[31:7]
andi t3 t1 0b11111 # get rd(5)
slli t3 t3 7 # num[11:7]
add t2 t2 t3 # change num[11:7]
addi t2 t2 0b0010011 #add the opcode
add a4 t2 x0 # change the new order
j print_32_order # jump to print

lui_process:
li t2 0 # set t2 be sero
srli t1 a4 12 # get num[15:12]
andi t1 t1 0b1 # get uzimm[17]
beq t1 x0 pos_lui # positive imm
li t3 0b111111111111111 # imm[31:17]
slli t3 t3 17 #imm[31:17]
add t2 t2 t3 # change num[31:17]

pos_lui:
# 16num[6:2]-> nzimm[16:12] -> 32num[16:12]
srli t1 a4 2 # get num[31:2]
andi t3 t1 0b11111 # get nzimm[16:12]
slli t3 t3 12 #num[16:12]
add t2 t2 t3 # change num[16:12]
# 16num[11:7] -> rd(5) -> 32num[11:7]
srli t1 a4 7 # get num[31:7]
andi t3 t1 0b11111 # get rd(5)
slli t3 t3 7 # num[11:7]
add t2 t2 t3 # change num[11:7]
addi t2 t2 0b0110111 #add the opcode
add a4 t2 x0 # change the new order
j print_32_order # jump to print

addi_rd_process:
li t2 0 # set t2 be sero
srli t1 a4 12 # get num[15:12]
andi t1 t1 0b1 # get imm[5]
beq t1 x0 pos_addi_rd # positive imm
li t3 0b1111111 # imm[11:5]
slli t3 t3 25 #imm[31:25]
add t2 t2 t3 # change num[31:25]

pos_addi_rd:
# 16num[6:2]-> imm[4:0] -> 32num[24:20]
srli t1 a4 2 # get num[31:2]
andi t3 t1 0b11111 # get imm[4:0]
slli t3 t3 20 #num[24:20]
add t2 t2 t3 # change num[24:20]
# 16num[11:7] -> rd(5) -> 32num[19:15]
srli t1 a4 7 # get num[31:7]
andi t3 t1 0b11111 # get rd(5)
slli t3 t3 15 #num[19:15]
add t2 t2 t3 # change num[19:15]
# num[14:12] is zero
# 16num[11:7] -> rd(5) -> 32num[11:7]
srli t1 a4 7 # get num[31:7]
andi t3 t1 0b11111 # get rd(5)
slli t3 t3 7 #num[11:7]
add t2 t2 t3 # change num[11:7]
addi t2 t2 0b0010011 #add the opcode
add a4 t2 x0 # change the new order
j print_32_order # jump to print

beq_process:
li t2 0 # set t2 be sero
srli t1 a4 12 # get num[15:12]
andi t1 t1 0b1 # get offset[8]
beq t1 x0 pos_beq # positive imm
li t3 0b1 # imm[12]
slli t3 t3 31 #imm[31]
add t2 t2 t3 # change num[31]
li t3 0b1 # imm[11]
slli t3 t3 7 #imm[11] -> num[7]
add t2 t2 t3 # change num[7]
li t3 0b111 # imm[10:8]
slli t3 t3 28 #imm[10] -> num[30:28]
add t2 t2 t3 # change num[30:28]

pos_beq:
# 16num[11:10] -> offset[4:3] -> 32num[11:10]
srli t1 a4 10 # get num[31:10]
andi t3 t1 0b11# get offset[4:3]
slli t3 t3 10 #num[11:10]
add t2 t2 t3 # change num[11:10]
# rs1 missed 01
li t3 0b01 # rs1[4:3]
slli t3 t3 18 # num[19:18]
add t2 t2 t3 # change num[19:18]
# 16num[9:7] -> rs1(3) -> [17:15]
srli t1 a4 7 # get num[31:7]
andi t3 t1 0b111 # get rd(3)
slli t3 t3 15 #num[17:15]
add t2 t2 t3 # change num[17:15]
# 16num[6:5] -> offset[7:6] -> 32num[27:26]
srli t1 a4 5 # get num[31:10]
andi t3 t1 0b11# get offset[4:3]
slli t3 t3 26 #num[27:26]
add t2 t2 t3 # change num[27:26]
# 16num[4:3] -> offset[2:1] -> 32num[9:8]
srli t1 a4 3 # get num[31:3]
andi t3 t1 0b11# get offset[4:3]
slli t3 t3 8 #num[9:8]
add t2 t2 t3 # change num[9:8]
# 16num[2] -> offset[5] -> 32num[25]
srli t1 a4 2 # get num[31:2]
andi t3 t1 0b1# get offset[5]
slli t3 t3 25 #num[25]
add t2 t2 t3 # change num[25]
addi t2 t2 0b1100011 #add the opcode
add a4 t2 x0 # change the new order
j update_offset_32_branch # jump to change offset

bne_process:
li t2 0 # set t2 be sero
srli t1 a4 12 # get num[15:12]
andi t1 t1 0b1 # get offset[8]
beq t1 x0 pos_bne # positive imm
li t3 0b1 # imm[12]
slli t3 t3 31 #imm[31]
add t2 t2 t3 # change num[31]
li t3 0b1 # imm[11]
slli t3 t3 7 #imm[11] -> num[7]
add t2 t2 t3 # change num[7]
li t3 0b111 # imm[10:8]
slli t3 t3 28 #imm[10] -> num[30:28]
add t2 t2 t3 # change num[30:28]

pos_bne:
# 16num[11:10] -> offset[4:3] -> 32num[11:10]
srli t1 a4 10 # get num[31:10]
andi t3 t1 0b11# get offset[4:3]
slli t3 t3 10 #num[11:10]
add t2 t2 t3 # change num[11:10]
# rs1 missed 01
li t3 0b01 # rs1[4:3]
slli t3 t3 18 # num[19:18]
add t2 t2 t3 # change num[19:18]
# 16num[9:7] -> rs1(3) -> [17:15]
srli t1 a4 7 # get num[31:7]
andi t3 t1 0b111 # get rd(3)
slli t3 t3 15 #num[17:15]
add t2 t2 t3 # change num[17:15]
# funct3 001
li t3 0b001 # funct3 = 001
slli t3 t3 12 # num[14:12]
add t2 t2 t3 # change num[14:12]
# 16num[6:5] -> offset[7:6] -> 32num[27:26]
srli t1 a4 5 # get num[31:10]
andi t3 t1 0b11# get offset[4:3]
slli t3 t3 26 #num[27:26]
add t2 t2 t3 # change num[27:26]
# 16num[4:3] -> offset[2:1] -> 32num[9:8]
srli t1 a4 3 # get num[31:3]
andi t3 t1 0b11# get offset[4:3]
slli t3 t3 8 #num[9:8]
add t2 t2 t3 # change num[9:8]
# 16num[2] -> offset[5] -> 32num[25]
srli t1 a4 2 # get num[31:2]
andi t3 t1 0b1# get offset[5]
slli t3 t3 25 #num[25]
add t2 t2 t3 # change num[25]
addi t2 t2 0b1100011 #add the opcode
add a4 t2 x0 # change the new order
j update_offset_32_branch # jump to change offset

jal_x0_process:
li t2 0 # set t2 be sero
srli t1 a4 12 # get num[15:12]
andi t1 t1 0b1 # get offset[11]
beq t1 x0 pos_jal_x0 # positive imm
li t3 0b1 # imm[20]
slli t3 t3 31 # num[31]
add t2 t2 t3 # change num[31]
li t3 0b1 # imm[11]
slli t3 t3 20# num[20]
add t2 t2 t3 # change num[20]
li t3 0b11111111 # imm[19:12]
slli t3 t3 12 # num[19:12]
add t2 t2 t3 # change num[19:12]

pos_jal_x0:
# 16num[11] -> offset[4] -> 32num[24]
srli t1 a4 11 # get num[31:11]
andi t3 t1 0b1# get offset[4]
slli t3 t3 24 #num[24]
add t2 t2 t3 # change num[24]
# 16num[10:9] -> offset[9:8] -> 32num[29:28]
srli t1 a4 9 # get num[31:9]
andi t3 t1 0b11# get offset[9:8]
slli t3 t3 28 #num[29:28]
add t2 t2 t3 # change num[29:28]
# 16num[8] -> offset[10] -> 32num[30]
srli t1 a4 8 # get num[31:8]
andi t3 t1 0b1 # get offset[10]
slli t3 t3 30 #num[30]
add t2 t2 t3 # change num[30]
# 16num[7] -> offset[6] -> 32num[26]
srli t1 a4 7 # get num[31:7]
andi t3 t1 0b1 # get offset[6]
slli t3 t3 26 #num[26]
add t2 t2 t3 # change num[26]
# 16num[6] -> offset[7] -> 32num[27]
srli t1 a4 6 # get num[31:6]
andi t3 t1 0b1 # get offset[7]
slli t3 t3 27 #num[27]
add t2 t2 t3 # change num[27]
# 16num[5:3] -> offset[3:1] -> 32num[23:21]
srli t1 a4 3 # get num[31:3]
andi t3 t1 0b111 # get offset[3:1]
slli t3 t3 21 #num[23:21]
add t2 t2 t3 # change num[23:21]
# 16num[2] -> offset[5] -> 32num[25]
srli t1 a4 2 # get num[31:2]
andi t3 t1 0b1# get offset[5]
slli t3 t3 25 #num[25]
add t2 t2 t3 # change num[25]
addi t2 t2 0b1101111 #add the opcode
add a4 t2 x0 # change the new order
j update_offset_32_jump # jump to change offset

jal_x1_process:
li t2 0 # set t2 be sero
srli t1 a4 12 # get num[15:12]
andi t1 t1 0b1 # get offset[11]
beq t1 x0 pos_jal_x1 # positive imm
li t3 0b1 # imm[20]
slli t3 t3 31 # num[31]
add t2 t2 t3 # change num[31]
li t3 0b1 # imm[11]
slli t3 t3 20# num[20]
add t2 t2 t3 # change num[20]
li t3 0b11111111 # imm[19:12]
slli t3 t3 12 # num[19:12]
add t2 t2 t3 # change num[19:12]

pos_jal_x1:
# 16num[11] -> offset[4] -> 32num[24]
srli t1 a4 11 # get num[31:11]
andi t3 t1 0b1# get offset[4]
slli t3 t3 24 #num[24]
add t2 t2 t3 # change num[24]
# 16num[10:9] -> offset[9:8] -> 32num[29:28]
srli t1 a4 9 # get num[31:9]
andi t3 t1 0b11# get offset[9:8]
slli t3 t3 28 #num[29:28]
add t2 t2 t3 # change num[29:28]
# 16num[8] -> offset[10] -> 32num[30]
srli t1 a4 8 # get num[31:8]
andi t3 t1 0b1 # get offset[10]
slli t3 t3 30 #num[30]
add t2 t2 t3 # change num[30]
# 16num[7] -> offset[6] -> 32num[26]
srli t1 a4 7 # get num[31:7]
andi t3 t1 0b1 # get offset[6]
slli t3 t3 26 #num[26]
add t2 t2 t3 # change num[26]
# 16num[6] -> offset[7] -> 32num[27]
srli t1 a4 6 # get num[31:6]
andi t3 t1 0b1 # get offset[7]
slli t3 t3 27 #num[27]
add t2 t2 t3 # change num[27]
# 16num[5:3] -> offset[3:1] -> 32num[23:21]
srli t1 a4 3 # get num[31:3]
andi t3 t1 0b111 # get offset[3:1]
slli t3 t3 21 #num[23:21]
add t2 t2 t3 # change num[23:21]
# 16num[2] -> offset[5] -> 32num[25]
srli t1 a4 2 # get num[31:2]
andi t3 t1 0b1# get offset[5]
slli t3 t3 25 #num[25]
add t2 t2 t3 # change num[25]
#add rd(00001)
li t3 0b00001 # rd = 00001
slli t3 t3 7 # num[7]
add t2 t2 t3 # get rd
addi t2 t2 0b1101111 #add the opcode
add a4 t2 x0 # change the new order
j update_offset_32_jump # jump to change offset

further_check_3:
srli t1 a4 13 # get num[15:13]
li t2 0b010 # lw
li t3 0b110 # sw
beq t1 t2 lw_process # the order is lw
beq t1 t3 sw_process # the order is sw

lw_process:
srli t1 a4 2 # shift 2 bits
andi t1 t1 0b111 # get rd
srli t2 a4 5 # shift 5 bits
andi t2 t2 0b1 # get offset[6]
srli t3 a4 6 # shift 6 bits
andi t3 t3 0b1 # get offset[2]
srli t4 a4 7 # shift 7 bits
andi t4 t4 0b111 # get rs1
srli t5 a4 10 # shift 10 bits
andi t5 t5 0b111 # get offset[5:3]
slli t2 t2 3 # shift 3 bits
add t2 t2 t5 # imm[11:3]
slli t2 t2 1 # shift 1 bits
add t2 t2 t3 # imm[11:2]
slli t2 t2 4 # shift 4 bits
addi t2 t2 0b01 # imm[11:0] 01
slli t2 t2 3 # shift 3 bits
add t2 t2 t4 # imm[11:0] 01 rs1(3)
slli t2 t2 3 # shift 3 bits
addi t2 t2 0b010 # imm[11:0] rs1 010
slli t2 t2 2 # shift 2 bits
addi t2 t2 0b01 # imm[11:0] rs1 010 01
slli t2 t2 3 # shift 3 bits
add t2 t2 t1 # imm[11:0] rs1 010 01 rd(3)
slli t2 t2 7 # shift 7 bits
addi t2 t2 0b0000011 # imm[11:0] rs1 010 rd 0000011
add a4 x0 t2 # store 32 bits order to a4
j print_32_order # jump to print

sw_process:
srli t1 a4 2 # shift 2 bits
andi t1 t1 0b111 # get rs2
srli t2 a4 5 # shift 5 bits
andi t2 t2 0b1 # get offset[6]
srli t3 a4 6 # shift 6 bits
andi t3 t3 0b1 # get offset[2]
srli t4 a4 7 # shift 7 bits
andi t4 t4 0b111 # get rs1
srli t5 a4 10 # shift 10 bits
andi t5 t5 0b11 # get offset[4:3]
srli t6 a4 12 # shift 12 bits
andi t6 t6 0b1 # get offset[5]
slli t2 t2 1 # shift 3 bits
add t2 t2 t6 # imm[11:5]
slli t2 t2 2 # shift 2 bits
addi t2 t2 0b01 # imm[11:5] 01
slli t2 t2 3 # shift 3 bits
add t2 t2 t1 # imm[11:5] 01 rs2(3)
slli t2 t2 2 # shift 2 bits
addi t2 t2 0b01 # imm[11:5] rs2 01
slli t2 t2 3 # shift 3 bits
add t2 t2 t4 # imm[11:5] rs2 01 rs1(3)
slli t2 t2 3 # shift 3 bits
addi t2 t2 0b010 # imm[11:5] rs2 rs1 010
slli t2 t2 2 # shift 2 bits
add t2 t2 t5 # imm[11:5] rs2 rs1 010 imm[4:3]
slli t2 t2 1 # shift 1 bits
add t2 t2 t3 # imm[11:5] rs2 rs1 010 imm[4:2]
slli t2 t2 9 # shift 9 bits
addi t2 t2 0b0100011 # imm[11:5] rs2 rs1 010 imm[4:0] 0100011
add a4 x0 t2 # store 32 bits order to a4
j print_32_order # jump to print

# Exit with error code 0
exit_program:
add	a1 x0 x0
addi a0 x0 17 #exit2
ecall

change_offset:
# s3 is offset
# t1 is the address of the order
# t3 is the address of the length array
addi sp sp -4 # store ra
sw ra 0(sp) # store ra
add s4 t1 s3 # s4 is the target positon
add t2 t3 x0 # t2 = t3
blt s4 t1 negative_offset # if target position < temp positon move order to left
li t4 4 # positive offset
j find_target_loop # jump to find the target

negative_offset:
li t4 -4 # negative offset

find_target_loop:
lw t5 0(t2) # get tmp address
beq s4 t5 find_target_end # if tmp address == target positon then finish
add t2 t4 t2 # next element in length array
j find_target_loop # next address

find_target_end:
sub s3 t2 t3 # the new offset is tmp address - orignal address
lw ra 0(sp) # get ra
addi sp sp 4 # return sp
jr ra # back