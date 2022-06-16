#include "lcd/lcd.h"
#include "img.h"
#include <string.h>
#include "utils.h"

enum status{Start, Run, Jump, Squat, End};

void Inp_init(void)
{
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
}

void Adc_init(void) 
{
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_0|GPIO_PIN_1);
    RCU_CFG0|=(0b10<<14)|(1<<28);
    rcu_periph_clock_enable(RCU_ADC0);
    ADC_CTL1(ADC0)|=ADC_CTL1_ADCON;
}

void IO_init(void)
{
    Inp_init(); // inport init
    Adc_init(); // A/D init
    Lcd_Init(); // LCD init
    LCD_Clear(BLACK);
}

typedef struct Block
{
    int exist;
    int choice;
    int remain;
    int fill;
    int pos;
    u8* img;
    int leftx;
    int lefty;
    int rightx;
    int righty;
} block;

void init_block(block* b) {
    b->exist = 0;
    b->choice = 0;
    b->remain = 0;
    b->fill = 0;
    b->img = NULL;
    b->leftx = 0;
    b->lefty = 0;
    b->pos = 0;
    b->rightx = 0;
    b->righty = 0;
}

void set_block(block* b){
    //cactus
    if (b->choice == 1 || b->choice == 2){
        b->img = (b->choice == 1)? cactus1: cactus2;
        b->leftx = 149 - b->pos;
        b->lefty = 41;
        b->rightx = 160 - b->pos;
        b->righty = 60;
        b->fill = 158 - b->pos;
    }
    // pter
    else if (b->choice == 3 || b->choice == 4){
        if(b->pos % 24 < 12) {b->img = pter1;}
        else {b->img = pter2;}
        b->leftx = 141 - b->pos;
        b->rightx = 160 - b->pos;
        b->fill = 150 - b->pos;
        b->lefty = (b->choice == 3)? 26: 41;
        b->righty = (b->choice == 3)? 45: 60;
    }
}

int check_block(block b, int dlx, int dly, int drx, int dry){
    if(b.lefty == 41) {
        return ((dry > b.lefty + 4) && (drx > b.leftx + 4) );
    }
    else return ((dly < b.righty - 2) && (drx > b.leftx + 2 && b.rightx - 2> drx));
    return 0;
}

int main(void)
{
    IO_init();         // init OLED
    // YOUR CODE HERE
    enum status before = Start;
    int cnt = 0, remain_run = 0, remain_jump = 0, remain_squat = 0, jump_up = 1, jump_pos = 0;
    int dlx = 10, dly = 0, drx = 0, dry = 0;
    int gap = 0, flag = 0;

    block b1, b2;
    init_block(&b1);
    init_block(&b2);

    float score = 0;

    
    while (1){
        if (before == Start){
            startUI();
            if ((Get_Button(0) == 1 || Get_Button(1) == 1)){
                before = Run;
                LCD_Clear(BLACK);
            }
            continue;
        }
        if (before == End){
            loseUI((int)score);
            LCD_ShowString(45, 10, "Game Over", RED);
            break;
        }
        //block
        if (cnt == 0){
            b1.exist = 1;
            b1.choice = rand() % 4 + 1;
            b1.pos = 0; b1.remain = 0;
        }
        if (flag == 0){
           gap = rand() % 40 + 60; 
        }
        if (cnt > (gap * 2) && flag == 0){
            b2.exist = 1;
            b2.choice = rand() % 4 + 1;
            b2.pos = 0; b2.remain = 0;
            flag = 1;
        }
        
        if (b1.exist == 1){
            if(b1.pos >= 150){
                b1.exist = 0;
            }else{
                set_block(&b1);
            }
        }

        if (b2.exist == 1){
            if(b2.pos >= 150){
                b2.exist = 0;
                flag = 0;
            }else{
                set_block(&b2);
            }
        }

        // score
        if (cnt % 20 == 0){
            LCD_ShowNum1(120, 0, ++score, 6, YELLOW);
        }

        // ground
        LCD_ShowPic(1, 61, 160, 70, g1, 1, cnt);
        cnt = (cnt + 1) % 320;
        if((jump_pos !=0 && Get_Button(0) == 1)){
            LCD_Fill(10, 41, 36, 60, BLACK);
        }
        else if ((Get_Button(0) == 0 && Get_Button(1) == 0)){
            if (before != Run  && jump_pos == 0){
                before = Run;
                LCD_Fill(10, 41, 36, 60, BLACK);
            }  
        }
        else if (Get_Button(0) == 0 && Get_Button(1) == 1){            
            if (before != Jump){
                before = Jump;
                LCD_Fill(10, 41, 36, 60, BLACK);
            }
        }
        else if (Get_Button(0) == 1 && Get_Button(1) == 0){
            if (before == Run){
                before = Squat;
                LCD_Fill(10, 41, 36, 60, BLACK);
            }
        }
        LCD_Fill(0, 11, 9, 60, BLACK);
        if(before == Run){
            dly = 41; drx = 29; dry = 60;
            if (remain_run < 10){
                LCD_ShowPic(10, 41, 29, 60, trex1, 0, cnt);
            } 
            else if (10 <= remain_run && remain_run < 20){
                LCD_ShowPic(10, 41, 29, 60, trex2, 0, cnt);
            } 
            else if (20 <= remain_run && remain_run < 30){
                LCD_ShowPic(10, 41, 29, 60, trex3, 0, cnt);
            } 
            remain_run = (remain_run + 1) % 30;
        }
        if (before == Squat){
            dly = 46; drx = 36; dry = 60;
            if (remain_squat < 10){
                LCD_ShowPic(10, 41, 36, 60, trex4, 0, cnt);
            } 
            else if (10 <= remain_squat && remain_squat < 20){
                LCD_ShowPic(10, 41, 36, 60, trex5, 0, cnt);
            }  
            remain_squat = (remain_squat + 1) % 20;
        }
        
        else if (before == Jump){
            if(jump_pos >= 30){
                jump_pos = 28;
                jump_up = 0;
            }

            if(jump_up && jump_pos < 30){
                int i = jump_pos;
                LCD_ShowPic(10, 41 - i, 29, 60 - i, trex3, 0, cnt);  
                dly = 41 - i; drx = 29; dry = 60 - i;
                if (++remain_jump > 5){
                    remain_jump = 0;
                    jump_pos += 3;
                    LCD_Fill(10, 60 - i - 2, 29, 60 - i, BLACK);
                }   
            }
            else if(!jump_up && jump_pos > 0){
                int i = jump_pos;
                LCD_ShowPic(10, 41 - i, 29, 60 - i, trex3, 0, cnt);
                dly = 41 - i; drx = 29; dry = 60 - i;
                if (++remain_jump > 5){
                    remain_jump = 0;
                    jump_pos -= 3;
                    LCD_Fill(10, 41 - i, 29, 43 - i, BLACK);
                } 
                if(jump_pos <= 0){
                    jump_pos = 0;
                    jump_up  = 1;
                    before = Run;
                }
            }
        }

        if(b1.exist){
            LCD_ShowPic(b1.leftx, b1.lefty, b1.rightx, b1.righty, b1.img, 0, cnt);
            b1.remain++;
            if (b1.remain > 3){
                b1.remain = 0;
                b1.pos += 3;
                LCD_Fill(b1.fill, b1.lefty, b1.rightx, b1.righty, BLACK);
            }

            if(check_block(b1, dlx, dly, drx, dry) == 1){
                before = End;
            }
        }

        if(b2.exist){
            LCD_ShowPic(b2.leftx, b2.lefty, b2.rightx, b2.righty, b2.img, 0, cnt);
            b2.remain++;
            if (b2.remain > 3){
                b2.remain = 0;
                b2.pos += 3;
                LCD_Fill(b2.fill, b2.lefty, b2.rightx, b2.righty, BLACK);
            }
            if(check_block(b2, dlx, dly, drx, dry) == 1){
                before = End;
            }
        }
    }
}
