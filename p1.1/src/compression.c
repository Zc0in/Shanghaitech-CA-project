#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "compression.h"
#include "utils.h"

/* The definition of compress */
void compress(char* in_code, int class,const int row,const int* offset_table){
    /* init the output order */
    char compressed_order[17];
    /* init temp rd */ 
    char temp_rd[15];
    /* init offset */
    char temp_offset[33];
    /* clear the temp order */
    memset(temp_offset, '\0', 33);
    memset(compressed_order, '\0', 17);
    memset(temp_rd, '\0', 15);
   /* The order can not be compressed */
    if (class == 0){
        return;
    }
    /* add rd, rd, rs2 -- 1 */
    /* add rd, x0, rs2 -- 2 */
    else if (class == 1 || class == 2){
        /* check the funct */
        if (class == 1){
            strcat(compressed_order, "1001");
        }
        /* check the funct */
        else{
            strcat(compressed_order, "1000");
        }
        /* get rd */
        substring(in_code, temp_rd, 20, 25); 
        /* cat rd to compressed_order */
        strcat(compressed_order, temp_rd);
        /* get rs2 */
        substring(in_code, temp_rd, 7, 12);
        /* cat rd to compressed_order */
        strcat(compressed_order, temp_rd);
        strcat(compressed_order, "10");
        /* clear the incode */
        memset(in_code, '\0', 33);
        /* reset the order */
        strcpy(in_code, compressed_order);
        /* return */
        return;
    }
    /* jalr x0, 0 (rs1) -- 7 */
    /* jalr x1, 0 (rs1) -- 8 */
    else if (class == 7 || class == 8){
        /* check the funct */
        if (class == 7){
            strcat(compressed_order, "1000");
        }
        /* check the funct */
        else{
            strcat(compressed_order, "1001");
        }
        /* get rs1 */
        substring(in_code, temp_rd, 12, 17);
        /* cat rs1 to coompress_order */
        strcat(compressed_order, temp_rd);
        /* cat offset and opcode */
        strcat(compressed_order, "0000010");
        /* clear the incode */
        memset(in_code, '\0', 33);
        /* reset the order */
        strcpy(in_code, compressed_order);
        /* return */
        return;
    }

    /*  xor rd', rd', rs2' -- 5 
        sub rd', rd', rs2' -- 6 
        or rd', rd', rs2' -- 4 
        and rd', rd', rs2' -- 3  */

    else if (class >= 3 && class <= 6){
        /* cat the funct */
        strcat(compressed_order, "100011");
        /* get rd */
        substring(in_code, temp_rd, 22, 25); 
        /* cat rd to compressed_order */
        strcat(compressed_order, temp_rd);
        /* check class */
        if(class == 3){
            strcat(compressed_order, "11");
        }
        /* check class */
        else if(class == 4){
            strcat(compressed_order, "10");
        }
        /* check class */
        else if(class == 5){
            strcat(compressed_order, "01");
        }
        /* check class */
        else{
            strcat(compressed_order, "00");
        }
        /* get rs2 */
        substring(in_code, temp_rd, 9, 12);
        /* cat rd to compressed_order */
        strcat(compressed_order, temp_rd);
        strcat(compressed_order, "01");
        /* clear the incode */
        memset(in_code, '\0', 33);
        /* reset the order */
        strcpy(in_code, compressed_order);
        /* return */
        return;
    }
    /* addi rd, x0, imm -- 9 
        addi rd, rd, nzimm -- 10 
        slli rd, rd, shamt -- 11 
       lui rd, nzimm -- 15 */
    else if ((class >= 9 && class <= 11) || class == 15){
        /* cat the funct */
        strcat(compressed_order, "0");
        if(class == 9){
            /* addi rd, x0, imm -- 9 */
            strcat(compressed_order, "10");
        }
        else if(class == 15){
            /* lui rd, nzimm -- 15 */
            strcat(compressed_order, "11");
        }
        else{
            /*addi rd, rd, nzimm -- 10
              slli rd, rd, shamt -- 11 */
            strcat(compressed_order, "00");
        }
        /* get nzimm[17] */
        if(class == 15) {
            compressed_order[3] = in_code[14];
        }
        /* or imm[5] */
        else{
            compressed_order[3] = in_code[6];
        }
        /* get rd */
        substring(in_code, temp_rd, 20, 25);
        /* cat rd to compressed_order */
        strcat(compressed_order, temp_rd);
        /* get shamt[4:0] or imm[4:0] */
        if(class == 15){
            /* lui */
            substring(in_code, temp_rd, 15, 20);
        }
        else{
            /* other class */
            substring(in_code, temp_rd, 7, 12);
        }
        strcat(compressed_order, temp_rd);
        /* opcode */
        if(class == 11){
            /* slli */
            strcat(compressed_order, "10");
        }
        else{
            /* addi lui */
            strcat(compressed_order, "01");
        }
        memset(in_code, '\0', 33);
        /* clear the incode */
        strcpy(in_code, compressed_order);
        /* reset the order */
        return;
        /* return */
    }

    /* srli rd', rd', shamt -- 12
        srai rd', rd', shamt -- 13 
        andi rd', rd', imm -- 14 */
    else if (class >= 12 && class <= 14){
        /* cat the funct */
        strcat(compressed_order, "100");
        /* get rd */
        /* check class */
        if(class == 14){
            /* andi: imm[5] */
            compressed_order[3] = in_code[6];
            /* add "10" */
            strcat(compressed_order, "10");
        }
        else{
            /* shamt[5] must be zero */
            compressed_order[3] = '0';
            /* check class */
            if(class == 12){
                /* srli */
                strcat(compressed_order, "00");
            }
            else{
                /* srai */
                strcat(compressed_order, "01");
            }
        }
        /* get rd */
        substring(in_code, temp_rd, 22, 25);
        /* cat rd to compressed_order */
        strcat(compressed_order, temp_rd);
        /* get shamt[4:0] or imm[4:0] */
        substring(in_code, temp_rd, 7, 12);
        strcat(compressed_order, temp_rd);
        /* get opcode */
        strcat(compressed_order, "01");
        /* clear the incode */
        memset(in_code, '\0', 33);
        /* reset the order */
        strcpy(in_code, compressed_order);
        /* return */
        return;
    }
    /* lw rd', offset (rs1') -- 16 */
    else if(class == 16){
        /* add funct */
        strcat(compressed_order, "010");
        /* get offset[5:3] */
        substring(in_code, temp_rd, 6, 9);
        /* add offset[5:3] */
        strcat(compressed_order, temp_rd);
        /* get rs1 */
        substring(in_code, temp_rd, 14, 17);
        /* add rs1 */
        strcat(compressed_order, temp_rd);
        /*offset[2]*/
        compressed_order[9] = in_code[9];
        /* offset[6] */
        compressed_order[10] = in_code[5];
        /* get rd */
        substring(in_code, temp_rd, 22, 25);
        /* cat rd to compressed_order */
        strcat(compressed_order, temp_rd);
        /* add opcode */
        strcat(compressed_order, "00");
        /* clear the incode */
        memset(in_code, '\0', 33);
        /* reset the order */
        strcpy(in_code, compressed_order);
        /* return */
        return;
    }

    /* sw rs2, offset (rs1') -- 17 */
    else if(class == 17){
        /* add funct */
        strcat(compressed_order, "110");
        /* get offset[5] */
        compressed_order[3] = in_code[6];
        /* get offset[4:3] */
        substring(in_code, temp_rd, 20, 22);
        /* add offset[4:3] */
        strcat(compressed_order, temp_rd);
        /* get rs1 */
        substring(in_code, temp_rd, 14, 17);
        /* add rs1 */
        strcat(compressed_order, temp_rd);
        /*offset[2]*/
        compressed_order[9] = in_code[22];
        /* offset[6] */
        compressed_order[10] = in_code[5];
        /* get rs2 */
        substring(in_code, temp_rd, 9, 12);
        /* cat rs2 to compressed_order */
        strcat(compressed_order, temp_rd);
        /* add opcode */
        strcat(compressed_order, "00");
        /* clear the incode */
        memset(in_code, '\0', 33);
        /* reset the order */
        strcpy(in_code, compressed_order);
        /* return */
        return;
    }
    /* bne rs1', x0, offset -- 19 */
    /* beq rs1', x0, offset -- 18 */
    else if(class == 18 || class == 19){
        /* add funct */
        if(class == 18){
            /* beq */
            strcat(compressed_order, "110");
        }
        else{
            /* bne */
            strcat(compressed_order, "111");
        }
        /* get offset */
        /* imm[12]*/
        temp_offset[0] = in_code[0];
        /* imm[11] */
        temp_offset[1] = in_code[24];
        /* imm[10:5] */
        substring(in_code, temp_rd, 1, 7);
        strcat(temp_offset, temp_rd);
        /* clear temp_rd */
        memset(temp_rd, '\0', 15);
        /* imm[4:1] */
        substring(in_code, temp_rd, 20, 24);
        strcat(temp_offset, temp_rd);
        /* get offset */
        change_offset(temp_offset, row, offset_table, 10);
        /* offset[8] */
        compressed_order[3] = temp_offset[0];
        /* offset[4:3] */
        compressed_order[4] = temp_offset[4];
        compressed_order[5] = temp_offset[5];
        /* get rs1 */
        memset(temp_rd, '\0', 15);
        substring(in_code, temp_rd, 14, 17);
        /* add rs1 */
        strcat(compressed_order, temp_rd);
        /* offset [7:6] */
        compressed_order[9] = temp_offset[1];
        compressed_order[10] = temp_offset[2];
        /* offset [2:1] */
        compressed_order[11] = temp_offset[6];
        compressed_order[12] = temp_offset[7];
        /* offset [5] */
        compressed_order[13] = temp_offset[3];
        /* add opcode */
        strcat(compressed_order, "01");
        /* clear the incode */
        memset(in_code, '\0', 33);
        /* reset the order */
        strcpy(in_code, compressed_order);
        /* return */
        return;
    }
    
    /* jal x0, offset -- 20 */
    /* jal x1, offset -- 21 */
    else if(class == 20 || class == 21){
        /* add funct */
        if(class == 20){
            /* jal x0, offset */
            strcat(compressed_order, "101");
        }
        else{
            /* jal x1, offset */
            strcat(compressed_order, "001");
        }
        /* get offset */
        /* offset[20] */
        temp_offset[0] = in_code[0];
        /* offset[19:12] */
        substring(in_code, temp_rd, 12, 20);
        strcat(temp_offset, temp_rd);
        /* offset[11] */
        temp_offset[9] = in_code[11];
        /* offset[10:1]*/
        substring(in_code, temp_rd, 1, 11);
        strcat(temp_offset, temp_rd);
        /* got offset */
        change_offset(temp_offset, row, offset_table, 13);
        /* offset[11] */
        compressed_order[3] = temp_offset[0];
        /* offset[4] */
        compressed_order[4] = temp_offset[7];
        /* offset[9:8] */
        compressed_order[5] = temp_offset[2];
        compressed_order[6] = temp_offset[3];
        /* offset[10] */
        compressed_order[7] = temp_offset[1];
        /* offset[6:7]*/
        compressed_order[8] = temp_offset[5];
        compressed_order[9] = temp_offset[4];
        /* offset[3:1]*/
        compressed_order[10] = temp_offset[8];
        compressed_order[11] = temp_offset[9];
        compressed_order[12] = temp_offset[10];
        /* offset[5] */
        compressed_order[13] = temp_offset[6];
        /* add opcode */
        strcat(compressed_order,"01");
        /* clear the incode */
        memset(in_code, '\0', 33);
        /* reset the order */
        strcpy(in_code, compressed_order);
        /* return */
        return;
    }
    /* beq bne 32 bits */
    else if(class == -1){
        int i = 0, j = 2;
        /* get the original offset */
        int n = put_together(in_code, 0) / 4;
        /* change the new offset */
        if(n == 0) {
            /* n is still zero */
            n = 0;
        }
        else if(row == 0){
            /* jump from the first */
            n = offset_table[n - 1];
        }
        else if(row + n == 0) {
            /* back to the first */
            n = 0 - offset_table[row - 1];
        }
        else{
            /* back to others */
            n = offset_table[row + n - 1] - offset_table[row - 1];
        }
        /* set the new offset */ 
        itobs(temp_offset, n, 14);
        /* offset[12] */
        in_code[0] = temp_offset[0];
        /* offset[10:5] */
        for(i = 1; i < 7; i++){
            in_code[i] = temp_offset[j++];
        }
        /* offset[4:1] */
        j = 8;
        /* reset j*/
        for(i = 20; i < 24; i++){
            in_code[i] = temp_offset[j++];
        }
        /* offset[11] */
        in_code[24] = temp_offset[1];
        /* return */
        return;
    }
    /* jal 32 bits */
    else if(class == -2){
        int i = 0, j = 10;
        /* get the original offset */
        int n = put_together(in_code, 1) / 4;
        /* change the new offset */
        if(n == 0) {
            /* n is still zero */
            n = 0;
        }
        else if(row == 0){
            /* jump from the first */
            n = offset_table[n - 1];
        }
        else if(row + n == 0) {
            /* back to the first */
            n = 0 - offset_table[row - 1];
        }
        else{
            /* back to others */
            n = offset_table[row + n -1] - offset_table[row - 1];
        }
        /* set the new offset */ 
        itobs(temp_offset, n, 22);
        /* offset[20] */
        in_code[0] = temp_offset[0];
        /* offset[10:1] */
        for(i = 1; i < 11; i++){
            in_code[i] = temp_offset[j++];
        }
        /* offset[11]*/
        in_code[11] = temp_offset[9];
        /* offset[19:12] */
        j = 1;
        /* reset j*/
        for(i = 12; i < 20; i++){
            in_code[i] = temp_offset[j++];
        }
        /* return */
        return;
    }
}

void change_offset(char* temp_offset, const int row, const int* offset_table, int final_length){
    /* get the original offset */
    int n = bstoi(temp_offset) / 2;
    /* change the new offset */
    if(n == 0) {
        /* n is still zero */
        n = 0;
    }
    else if(row == 0){
        /* jump from the first */
        n = offset_table[n-1];
    }
    else if(row + n == 0) {
        /* back to the first */
        n = 0 - offset_table[row-1];
    }
    else{
        /* back to others */
        n = offset_table[row + n -1] - offset_table[row -1];
    }
    /* set the new offset */ 
    itobs(temp_offset, n, final_length);
    /* return */
    return;
}