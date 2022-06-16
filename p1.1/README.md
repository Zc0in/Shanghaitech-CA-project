It is the project1.1 in cs110 ^-^

The team consists of wangyf7@ and jiky@

The number referance of the risc-v order:

add rd, rd, rs2 -- 1 

add rd, x0, rs2 -- 2

and rd', rd', rs2' -- 3

or rd', rd', rs2' -- 4

xor rd', rd', rs2' -- 5

sub rd', rd', rs2' -- 6

jalr x0, 0 (rs1) -- 7

jalr x1, 0 (rs1) -- 8

addi rd, x0, imm -- 9

addi rd, rd, nzimm -- 10

slli rd, rd, shamt -- 11

srli rd', rd', shamt -- 12

srai rd', rd', shamt -- 13

andi rd', rd', imm -- 14

lui rd, nzimm -- 15

lw rd', offset (rs1') -- 16

sw rs2, offset (rs1') -- 17

beq rs1', x0, offset -- 18

bne rs1', x0, offset -- 19

jal x0, offset -- 20

jal x1, offset -- 21