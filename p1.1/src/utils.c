#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
/* change the fscanf */
void myfscanf(FILE* input, char* target){
    int i = 0;
    /* set an temp char to read */
    char tmp; 
    /* target is a 33 length string */
    for (i = 0; i < 32; i++){
        fscanf(input, " %c", &tmp); /* care the space */
        target[i] = tmp; 
    }
    /* end the string */
    target[32] = '\0';
}
/* The definition of bstoi */
int bstoi(const char* string){
    int i = 0, n = 0;
    /* get each position of the string */
    for(i = 0; string[i] != '\0'; i++){
        /* update n */
        n = (n << 1) + (string[i]- '0');
    }
    /* if it is negative */
    if(string[0] == '1'){
        /* minus the sign position */
        n -= (1 << i);
    }
    /* return the number */
    return n;
}
/* The definition of itobs */
void itobs(char* temp_offset, int n, int length){
    /* initialize m a length - 1 long binary number */
    int m = 1 << (length - 2), i = 0;
    /* set 0 */
    memset(temp_offset, '\0', 33);
    for (i = 0; i < length - 1; i++){
        if ((n & m )== 0){
            /* set 0 */
            strcat(temp_offset, "0");
        }
        else{
            /* set 1 */
            strcat(temp_offset, "1");
        }
        /* shift left for 1 step */
        n <<= 1;
    }
    /* return */
    return;
}
/* The definition of substring */
void substring(const char* in_code, char* sub, int start, int end){
    int i = 0, j = 0;
    /* get sub string */
    for (i = start; i < end; i++){
        /* get words */
        sub[j++] = in_code[i];
    }
    /* set \0 */
    sub[j] = '\0';
}
/* The definition of rigister_judger */
int rigister_judger(const char* in_code, int start){
    /* judge whether the register is in x8-x15 */
    char temp[3];
    memset(temp, '\0', 3);
    /* get substr */
    substring(in_code, temp, start, start + 2);
    /* the first two should be 01 */
    if (strcmp(temp, "00") == 0 || strcmp(temp, "10") == 0 || strcmp(temp, "11") == 0){
        /* illegal */
        return 0;
    }
    /* legal */
    return 1;
}
/* The definition of put_together */
int put_together(const char* in_code, int flag){
    int imm_ans = 0;
    /* the situation of beq & bne */
    if (flag == 0){
        char imm[13];
        memset(imm, '\0', 13);
        /* get each number */
        substring(in_code, &imm[0], 0, 1);
        /* get each number */
        substring(in_code, &imm[1], 24, 25);
        /* get each number */
        substring(in_code, &imm[2], 1, 7);
        /* get each number */
        substring(in_code, &imm[8], 20, 24);
        /* shift left */
        imm_ans = 2 * bstoi(imm);
    }
    /* the situation of jal */
    else if (flag == 1){
        char imm[21];
        memset(imm, '\0', 21);
        /* get each number */
        substring(in_code, &imm[0], 0, 1);
        /* get each number */
        substring(in_code, &imm[1], 12, 20);
        /* get each number */
        substring(in_code, &imm[9], 11, 12);
        /* get each number */
        substring(in_code, &imm[10], 1, 11);
        /* shift left */
        imm_ans = 2 * bstoi(imm);
    }
    /* the situation of lw & addi & andi*/
    else if (flag == 2){
        char imm[13];
        memset(imm, '\0', 13);
        /* get each number */
        substring(in_code, &imm[0], 0, 12);
        /* get imm */
        imm_ans = bstoi(imm);
    }
    /* the situation of lui */
    else if (flag == 3){
        char imm[21];
        memset(imm, '\0', 21);
        /* get each number */
        substring(in_code, &imm[0], 0, 20);
        /* get imm */
        imm_ans = bstoi(imm);
    }
    /* the situation of sw */
    else if (flag == 4){
        char imm[13];
        memset(imm, '\0', 13);
        /* get each number */
        substring(in_code, &imm[0], 0, 7);
        /* get each number */
        substring(in_code, &imm[7], 20, 25);
        /* get imm */
        imm_ans = bstoi(imm);
    }
    /* return imm */
    return imm_ans;
}
/* The definition of classify */
int classify(const char* in_code){
    char opcode[8], funct3[4], funct7[8], rs1[6], rs2[6], rd[6], jalr_front[13];
    /* Get opcode */
    substring(in_code, opcode, 25, 32);
    if (strcmp(opcode , "0110011") == 0){
        /* Get funct3 */
        substring(in_code, funct3, 17, 20);
        /* Get funct7 */
        substring(in_code, funct7, 0, 7);
        /* Get rd */
        substring(in_code, rd, 20, 25);
        /* Get rs2 */
        substring(in_code, rs2, 7, 12);
        /* Get rs1 */
        substring(in_code, rs1, 12, 17);
        if (strcmp(funct3, "000") == 0){
            /* sub rd', rd', rs2' -- 6 */
            if (strcmp(funct7, "0100000") == 0){
                /* rs2 and rd should be in x8-x15 */
                if (rigister_judger(in_code, 7) == 0 || rigister_judger(in_code, 20) == 0){
                    /* the order can not be compressed */
                    return 0;
                }
                /* rs1 should be the same as rd */
                else if(strcmp(rs1, rd) != 0){
                    /* the order can not be compressed */
                    return 0;
                }
                /* legal */
                return 6;
            }
            else if (strcmp(funct7, "0000000") == 0){
                /* rs2 and rd should not be x0 */
                if (strcmp(rs2, "00000") == 0 || strcmp(rd, "00000") == 0){
                    /* the order can not be compressed */
                    return 0;
                }
                /* add rd, x0, rs2 -- 2 */
                if (strcmp(rs1, "00000") == 0){
                    /* legal */
                    return 2;
                }
                /* add rd, rd, rs2 -- 1 */
                else{
                    /* rs1 should be the same as rd */
                    if (strcmp(rs1, rd) != 0){
                        /* the order can not be compressed */
                        return 0;
                    }
                    /* legal */
                    return 1;
                }                
            }
        }
        else{
            /* rs2 and rd should be in x8-x15 */
            if (rigister_judger(in_code, 7) == 0 || rigister_judger(in_code, 20) == 0){
                /* the order can not be compressed */
                return 0;
            }
            /* rs1 should be the same as rd */
            else if(strcmp(rs1, rd) != 0){
                /* the order can not be compressed */
                return 0;
            }
            if (strcmp(funct7, "0000000") == 0){
                /* and rd', rd', rs2' -- 3 */
                if (strcmp(funct3, "111") == 0){
                    /* legal */
                    return 3;
                }
                /* or rd', rd', rs2' -- 4 */
                else if (strcmp(funct3, "110") == 0){
                    /* legal */
                    return 4;
                }
                /* xor rd', rd', rs2' -- 5 */
                else if (strcmp(funct3, "100") == 0){
                    /* legal */
                    return 5;
                }
            }
        }
    }
    else if (strcmp(opcode, "1100111") == 0){
        /* Get rs1 */
        substring(in_code, rs1, 12, 17);
        /* Get rd */
        substring(in_code, rd, 20, 25);
        /* Get funct3 */
        substring(in_code, funct3, 17, 20);
        /* Get jalr_front */
        substring(in_code, jalr_front, 0, 12);
        /* rs1 should not be x0 */
        if (strcmp(rs1, "00000") == 0 || strcmp(jalr_front, "000000000000") != 0 || strcmp(funct3, "000") != 0){
            /* illegal */
            return 0;
        }
        /* jalr x0, 0 (rs1) -- 7 */
        if (strcmp(rd, "00000") == 0){
            /* legal */
            return 7;
        }
        /* jalr x1, 0 (rs1) -- 7 */
        else if (strcmp(rd, "00001") == 0){
            /* legal */
            return 8;
        }
        /* other rd */
        else return 0;
    }
    else if (strcmp(opcode, "0010011") == 0){
        /* Get funct3 */
        substring(in_code, funct3, 17, 20);
        /* Get rs1 */
        substring(in_code, rs1, 12, 17);
        /* Get funct7 */
        substring(in_code, funct7, 0, 7);
        /* Get rd */
        substring(in_code, rd, 20, 25);
        /* rd should not be x0 */
        if (strcmp(funct3, "000") == 0){
            if (strcmp(rd, "00000") == 0){
                /* the order can not be compressed */
                return 0;
            }
            /* check the imm */
            if (put_together(in_code, 2) < -32 || put_together(in_code, 2) > 31){
                /* illegal */
                return 0;
            }
            /* addi rd, x0, imm -- 9 */
            if (strcmp(rs1, "00000") == 0){
                /* legal */
                return 9;
            }
            /* addi rd, rd, nzimm -- 10 */
            else{
                /* rs1 should be the same as rd */
                if (strcmp(rs1, rd) != 0 || put_together(in_code, 2) == 0){
                    /* illegal */
                    return 0;
                }
                /* legal */
                return 10;
            }
        }
        else{
            /* rs1 should be the same as rd */
            if (strcmp(rs1, rd) != 0){
                /* illegal */
                return 0;
            }
            /* slli rd, rd, shamt -- 11 */
            if (strcmp(funct3, "001") == 0 && strcmp(funct7, "0000000") == 0 && strcmp(rd, "00000") != 0){
                /* legal */
                return 11;
            }
            else{
                /* rd should be in x8-x15 */
                if (rigister_judger(in_code, 20) == 0){
                    /* the order can not be compressed */
                    return 0;
                }
                if (strcmp(funct3, "101") == 0){
                    /* srli rd', rd', shamt -- 12 */
                    if (strcmp(funct7, "0000000") == 0){
                        /* legal */
                        return 12;
                    }
                    /* srai rd', rd', shamt -- 13 */
                    else if (strcmp(funct7, "0100000") == 0){
                        return 13;
                    }
                }
                /* andi rd', rd', imm -- 14 */
                else if (strcmp(funct3, "111") == 0){
                    /* check the imm */
                    if (put_together(in_code, 2) < -32 || put_together(in_code, 2) > 31){
                        /* illegal */
                        return 0;
                    }
                    /* legal */
                    return 14;
                }
            }
        }
    }
    /* lui rd, nzimm -- 15 */
    else if (strcmp(opcode, "0110111") == 0){
        /* Get rd */
        substring(in_code, rd, 20, 25);
        /* rd should not be 0 or 2 */
        if (strcmp(rd, "00000") == 0 || strcmp(rd, "00010") == 0){
            /* the order can not be compressed */
            return 0;
        }
        /* check the imm */
        if (put_together(in_code, 3) < -32 || put_together(in_code, 3) > 31 || put_together(in_code, 3) == 0){
            /* illegal */
            return 0;
        }
        /* legal */
        return 15;
    }
    /* lw rd', offset (rs1') -- 16 */
    else if (strcmp(opcode, "0000011") == 0){
        /* Get funct3 */
        substring(in_code, funct3, 17, 20);
        /* rs1 and rd should be in x8-x15 */
        if (rigister_judger(in_code, 12) == 0 || rigister_judger(in_code, 20) == 0){
            /* the order can not be compressed */
            return 0;
        }
        /* check the imm */
        if (put_together(in_code, 2) < 0 || put_together(in_code, 2) > 127 || put_together(in_code, 2) % 4 != 0){
            /* illegal */
            return 0;
        }
        if (strcmp(funct3, "010") == 0){
            /* legal */
            return 16;
        }
    }
    /* sw rs2, offset (rs1') -- 17 */
    else if (strcmp(opcode, "0100011") == 0){
        /* Get funct3 */
        substring(in_code, funct3, 17, 20);
        /* rs1 and rs2 should be in x8-x15 */
        if (rigister_judger(in_code, 12) == 0 || rigister_judger(in_code, 7) == 0){
            /* the order can not be compressed */
            return 0;
        }
        /* check the imm */
        if (put_together(in_code, 4) < 0 || put_together(in_code, 4) > 127 || put_together(in_code, 4) % 4 != 0){
            /* illegal */
            return 0;
        }
        if (strcmp(funct3, "010") == 0){
            /* legal */
            return 17;
        }
        
    }
    else if (strcmp(opcode, "1100011") == 0){
        /* Get rs2 */
        substring(in_code, rs2, 7, 12);
        /* Get funct3 */
        substring(in_code, funct3, 17, 20);
        /* if rs2 != x0 */
        if (strcmp(rs2, "00000") != 0){
            return -1;
        }
        /* rs1 should be in x8-x15 */
        if (rigister_judger(in_code, 12) == 0){
            /* the order can not be compressed, but we still need to modify its offset */
            return -1;
        }
        /* The situation that the imm is too large to be compressed */
        if (put_together(in_code, 0) < -256 || put_together(in_code, 0) > 255){
            /* the order can not be compressed, but we still need to modify its offset */
            return -1;
        }
        /* beq rs1', x0, offset -- 18 */
        if (strcmp(funct3, "000") == 0){
            /* legal */
            return 18;
        }
        /* bne rs1', x0, offset -- 19 */
        else if (strcmp(funct3, "001") == 0){
            /* legal */
            return 19;
        }
        else{
            /* the order can not be compressed, but we still need to modify its offset */
            return -1;
        }
    }
    else if (strcmp(opcode, "1101111") == 0){
        /* Get rd */
        substring(in_code, rd, 20, 25);
        /* The situation that the imm is too large to be compressed */
        if (put_together(in_code, 1) < -2048 || put_together(in_code, 1) > 2047){
            /* the order can not be compressed, but we still need to modify its offset */
            return -2;
        }
        /* jal x0, offset -- 20 */
        if (strcmp(rd, "00000") == 0){
            /* legal */
            return 20;
        }
        /* jal x1, offset -- 21 */
        else if (strcmp(rd, "00001") == 0){
            /* legal */
            return 21;
        }
        else{
            /* the order can not be compressed, but we still need to modify its offset */
            return -2;
        }
    }
    /* the order can not be compressed */
    return 0;
}
