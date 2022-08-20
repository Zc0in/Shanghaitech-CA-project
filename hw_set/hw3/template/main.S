# DO NOT REMOVE THIS LINE WHEN SUBMITTING
.import input.S

.text  # code starts here

.globl main
main:
    # load str1 to a2
    la a2 str1  
    # load str2 to a3
    la a3 str2
    # load len1 to a4
    lw a4 len1
    # load len2 to a5
    lw a5 len2
    # go to the fuction(initialization)
    jal func
    # LCS
    jal LCS
    # print
    li a0 1
    ecall
    # exit
    li a0 10
    ecall
func:
    # initialize a6
    li a6 0
    # initialize a7
    addi a7 a5 -1
    jr ra
LCS:
    # judge to exit
    bge a6 a4 exit
    # initialize t1
    add t1 x0 a6
    # initialize t2
    add t2 x0 a7
    # initialize current len
    li s3 0
loop1:
    # t1 < len1
    blt t1 a4 further_check
    # t1 >= len1
    bge t1 a4 restart
further_check:
    # t2 < len2
    blt t2 a5 loop2
    # t2 >= len2
    bge t2 a5 restart
loop2:
    slli t3 t1 2
    add s1 a2 t3
    # get str1[t1]
    lw t4 0(s1)
    slli t5 t2 2
    add s2 a3 t5
    # get str2[t2]
    lw t6 0(s2)
    # str1[t1] == str2[t2]
    beq t4 t6 renew
    # str1[t1] != str2[t2]
    bne t4 t6 restore
step:
    # t1++
    addi t1 t1 1
    # t2++
    addi t2 t2 1
    # loop continue
    j loop1
restart:
    # a7 > 0
    blt x0 a7 update_a7
    # a7 <= 0
    bge x0 a7 update_a6   
update_a7:
    # a7--
    addi a7 a7 -1
    # loop continue
    j LCS
update_a6:
    # a6++
    addi a6 a6 1
    # loop continue
    j LCS
renew:
    # current len++
    addi s3 s3 1
    # max < len
    blt a1 s3 get_max
    # loop continue
    j step
restore:
    # current len = 0
    li s3 0
    # loop continue
    j step
get_max:
    # max = len
    add a1 x0 s3
    # loop continue
    j step
exit:
    # jump back to main
    jr ra
