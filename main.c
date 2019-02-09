/*
 * File:   main.c
 * Author: MICHAEL
 *
 * Created on October 27, 2018, 3:09 PM
 */
/*
 * PORTD0 = lower left LED
 * PORTD1 = lowermost LED
 * PORTD2 = lower right LED
 * PORTD3 = middle LED
 * PORTC6 = upper right LED
 * PORTC7 = upper left LED
 * PORTD4 = uppermost LED
 * 
 * PORTD5 = first digit
 * PORTD6 = second digit
 * PORTD7 = third digit
 */

#include <xc.h>
#include "CountDownClock.h"

uint8_t num_of_overflow = 0;
uint16_t num_of_overflow_1 = 0;
uint8_t count_for_first_digit = 1;
uint8_t count_for_second_digit = 10;
uint16_t count_for_third_digit = 100;

unsigned int start = 0;
void setupTimer(void);
void setPortsUP(void);
void setUP(void);
void setupPinInterrupt(void);

void zero(void);
void one(void);
void two(void);
void three(void);
void four(void);
void five(void);
void six(void);
void seven(void);
void eight(void);
void nine(void);

void digit_one();
void digit_two();
void digit_three();

int value_1 = 0;
int value_2 = 0;
int value_3 = 0;


//=============================================
//=============================================
void __interrupt ISR(){
   
    if(TMR1IF == 1){
            ++num_of_overflow_1;
            PORTEbits.RE2 = 1;
   
        if(num_of_overflow_1 == 200){ 
           PORTEbits.RE2 = 0;
           TMR1IF = 0;
           TMR1ON = 0;
           num_of_overflow_1 = 0;
        }
    TMR1IF = 0;
    }
    
    if(TMR0IF == 1){
        ++num_of_overflow;
        
        if(num_of_overflow == 40 && start == 1){
           value_1 -= 1;
           if(value_1 < 0 && value_2 == 0 && value_3 == 0){
               value_1 = 0;
               count_for_first_digit = 1;
               count_for_second_digit = 10;
               count_for_third_digit = 100;
               start = 0;
              PORTCbits.RC2 = 0;
               TMR1ON = 1;
           }
           else if(value_1 < 0){
               value_1 = 9;
               value_2 -= 1;
               if(value_2 < 0){
                   value_2 = 9;
                   value_3 -= 1;
                   if(value_3 < 0){
                       value_3 = 0;
                       value_2 = 9;
                   }
               }    
           }
          
           num_of_overflow = 0;
        }
      TMR0IF = 0;  
    }
    
    if(INT2IF == 1){
       
       start = 0;
       switch(count_for_third_digit){
               case 0:{value_3 = 0; break;}
               case 100:{value_3 = 1; break;}
               case 200:{value_3 = 2;break;}
               case 300:{value_3 = 3;break;}
               case 400:{value_3 = 4;break;}
               case 500:{value_3 = 5;break;}
               case 600:{value_3 = 6;break;}
               case 700:{value_3 = 7;break;}
               case 800:{value_3 = 8;break;}
               case 900:{value_3 = 9;break;}   
           }
            count_for_third_digit += 100;
           if(count_for_third_digit > 900){count_for_third_digit = 0;}
       
        
       INT2IF = 0; 
    }
    if(INT1IF == 1){
      
       start = 0;
       switch(count_for_second_digit){
               case 0:{value_2 = 0; break;}
               case 10:{value_2 = 1; break;}
               case 20:{value_2 = 2;break;}
               case 30:{value_2 = 3;break;}
               case 40:{value_2 = 4;break;}
               case 50:{value_2 = 5;break;}
               case 60:{value_2 = 6;break;}
               case 70:{value_2 = 7;break;}
               case 80:{value_2 = 8;break;}
               case 90:{value_2 = 9;break;}   
           }
            count_for_second_digit += 10;
           if(count_for_second_digit > 90){count_for_second_digit = 0;}
       
        
       INT1IF = 0; 
    }
    if(INT0IF == 1){
      
      start = 0;
       switch(count_for_first_digit){
               case 0:{value_1 = 0; break;}
               case 1:{value_1 = 1; break;}
               case 2:{value_1 = 2;break;}
               case 3:{value_1 = 3;break;}
               case 4:{value_1 = 4;break;}
               case 5:{value_1 = 5;break;}
               case 6:{value_1 = 6;break;}
               case 7:{value_1 = 7;break;}
               case 8:{value_1 = 8;break;}
               case 9:{value_1 = 9;break;}   
           }
            count_for_first_digit += 1;
           if(count_for_first_digit > 9){count_for_first_digit = 0;}
       
        
       INT0IF = 0; 
    }
    if(RBIF == 1){
        if(PORTBbits.RB4 == 1){
            if(start == 1){
              start = 0;  
            }else{
                start = 1;
            }
            PORTCbits.RC2 = 1; 
        } 
        RBIF = 0;
    }
    
   
}
//=============================================
//=============================================


void main(void) {
    setUP();
    while(1){
        digit_one();
        __delay_ms(1);
        digit_two();
        __delay_ms(1);
        digit_three();
        __delay_ms(1); 
    }
    return;
}


//==============================================
//==============================================
void setPortsUP(void){
    //setting internal oscillator = 8000000
    IRCF0 = 1;
    IRCF1 = 1;
    IRCF2 = 1;
    
    TRISD = 0;
    TRISCbits.RC6 = 0;
    TRISCbits.RC7 = 0;
    TRISCbits.RC2 = 0;
    TRISEbits.RE2 = 0;
   
    PORTD = 0;
    PORTCbits.RC6 = 0;
    PORTCbits.RC7 = 0;
    PORTEbits.RE2 = 0;
    PORTCbits.RC2 = 0;   
}
//===============================================
void setupTimer(){
    IPEN = 0;
    GIE = 1;
    PEIE = 1;
    
    TMR0IE = 1;
    TMR0IF = 0;
    TMR0IP = 0;
    
    T08BIT = 0;
    TMR0 = 15536;
    T0CS = 0;
    T0PS0 = 0;
    T0PS1 = 0;
    TMR0ON = 1;
    
    TMR1IE = 1;
    TMR1IF = 0;
    TMR1IP = 0;
    TMR1CS = 0;
    T1CKPS0 = 0;
    T1CKPS1 = 0;
    TMR1 = 15536;    
}
//================================================
void setUP(void){
    setupPinInterrupt();
    setPortsUP();
    setupTimer();   
}
//================================================
void zero(void){
    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 0;
    PORTDbits.RD2 = 0;
    PORTDbits.RD3 = 0;
    PORTCbits.RC6 = 0;
    PORTCbits.RC7 = 0;
    PORTDbits.RD4 = 0;
    
    PORTDbits.RD0 = 1;
    PORTDbits.RD1 = 1;
    PORTDbits.RD2 = 1;
    PORTDbits.RD3 = 0;
    PORTCbits.RC6 = 1;
    PORTCbits.RC7 = 1;
    PORTDbits.RD4 = 1;
}
//================================================
void one(void){
    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 0;
    PORTDbits.RD2 = 0;
    PORTDbits.RD3 = 0;
    PORTCbits.RC6 = 0;
    PORTCbits.RC7 = 0;
    PORTDbits.RD4 = 0;
    
    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 0;
    PORTDbits.RD2 = 1;
    PORTDbits.RD3 = 0;
    PORTCbits.RC6 = 1;
    PORTCbits.RC7 = 0;
    PORTDbits.RD4 = 0;
}
//================================================
void two(void){
   PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 0;
    PORTDbits.RD2 = 0;
    PORTDbits.RD3 = 0;
    PORTCbits.RC6 = 0;
    PORTCbits.RC7 = 0;
    PORTDbits.RD4 = 0;
    
    PORTDbits.RD0 = 1;
    PORTDbits.RD1 = 1;
    PORTDbits.RD2 = 0;
    PORTDbits.RD3 = 1;
    PORTCbits.RC6 = 1;
    PORTCbits.RC7 = 0;
    PORTDbits.RD4 = 1; 
}
//================================================
void three(void){
    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 0;
    PORTDbits.RD2 = 0;
    PORTDbits.RD3 = 0;
    PORTCbits.RC6 = 0;
    PORTCbits.RC7 = 0;
    PORTDbits.RD4 = 0;
    
    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 1;
    PORTDbits.RD2 = 1;
    PORTDbits.RD3 = 1;
    PORTCbits.RC6 = 1;
    PORTCbits.RC7 = 0;
    PORTDbits.RD4 = 1;
}
//================================================
void four(void){
    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 0;
    PORTDbits.RD2 = 0;
    PORTDbits.RD3 = 0;
    PORTCbits.RC6 = 0;
    PORTCbits.RC7 = 0;
    PORTDbits.RD4 = 0;
    
    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 0;
    PORTDbits.RD2 = 1;
    PORTDbits.RD3 = 1;
    PORTCbits.RC6 = 1;
    PORTCbits.RC7 = 1;
    PORTDbits.RD4 = 0;
}
//================================================
void five(void){
    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 0;
    PORTDbits.RD2 = 0;
    PORTDbits.RD3 = 0;
    PORTCbits.RC6 = 0;
    PORTCbits.RC7 = 0;
    PORTDbits.RD4 = 0;
    
    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 1;
    PORTDbits.RD2 = 1;
    PORTDbits.RD3 = 1;
    PORTCbits.RC6 = 0;
    PORTCbits.RC7 = 1;
    PORTDbits.RD4 = 1;
}
//================================================
void six(void){
    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 0;
    PORTDbits.RD2 = 0;
    PORTDbits.RD3 = 0;
    PORTCbits.RC6 = 0;
    PORTCbits.RC7 = 0;
    PORTDbits.RD4 = 0;
    
    PORTDbits.RD0 = 1;
    PORTDbits.RD1 = 1;
    PORTDbits.RD2 = 1;
    PORTDbits.RD3 = 1;
    PORTCbits.RC6 = 0;
    PORTCbits.RC7 = 1;
    PORTDbits.RD4 = 1;
}
//================================================
void seven(void){
    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 0;
    PORTDbits.RD2 = 0;
    PORTDbits.RD3 = 0;
    PORTCbits.RC6 = 0;
    PORTCbits.RC7 = 0;
    PORTDbits.RD4 = 0;
    
    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 0;
    PORTDbits.RD2 = 1;
    PORTDbits.RD3 = 0;
    PORTCbits.RC6 = 1;
    PORTCbits.RC7 = 0;
    PORTDbits.RD4 = 1;
}
//================================================
void eight(void){
    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 0;
    PORTDbits.RD2 = 0;
    PORTDbits.RD3 = 0;
    PORTCbits.RC6 = 0;
    PORTCbits.RC7 = 0;
    PORTDbits.RD4 = 0;
    
    PORTDbits.RD0 = 1;
    PORTDbits.RD1 = 1;
    PORTDbits.RD2 = 1;
    PORTDbits.RD3 = 1;
    PORTCbits.RC6 = 1;
    PORTCbits.RC7 = 1;
    PORTDbits.RD4 = 1;
}
//================================================
void nine(void){
    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 0;
    PORTDbits.RD2 = 0;
    PORTDbits.RD3 = 0;
    PORTCbits.RC6 = 0;
    PORTCbits.RC7 = 0;
    PORTDbits.RD4 = 0;
    
    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 1;
    PORTDbits.RD2 = 1;
    PORTDbits.RD3 = 1;
    PORTCbits.RC6 = 1;
    PORTCbits.RC7 = 1;
    PORTDbits.RD4 = 1;
}
//================================================
void setupPinInterrupt(void){
    RBIE = 1;
    RBIP = 1;
    RBIF = 0;
    GIE = 1;
    
    INT0IE = 1;
    INTEDG0 = 0;
    INT0IF = 0;
    
    INT1IE = 1;
    INTEDG1 = 0;
    INT1IP = 1;
    INT1IF = 0;
    
    INT2IE = 1;
    INTEDG2 = 0;
    INT2IP = 1;
    INT2IF = 0;
}
//================================================
void digit_one(){
    switch(value_1){
               case 0: {zero();break;}
               case 1:{one(); break;}
               case 2:{two();break;}
               case 3:{three();break;}
               case 4:{four();break;}
               case 5:{five();break;}
               case 6:{six();break;}
               case 7:{seven();break;}
               case 8:{eight();break;}
               case 9:{nine();break;}   
           }
    PORTDbits.RD5 = 1;
}
//================================================
void digit_two(){
    switch(value_2){
               case 0: {zero();break;}
               case 1:{one(); break;}
               case 2:{two();break;}
               case 3:{three();break;}
               case 4:{four();break;}
               case 5:{five();break;}
               case 6:{six();break;}
               case 7:{seven();break;}
               case 8:{eight();break;}
               case 9:{nine();break;}   
           }
    PORTDbits.RD6 = 1;
}
//================================================
void digit_three(){
    switch(value_3){
               case 0: {zero();break;}
               case 1:{one(); break;}
               case 2:{two();break;}
               case 3:{three();break;}
               case 4:{four();break;}
               case 5:{five();break;}
               case 6:{six();break;}
               case 7:{seven();break;}
               case 8:{eight();break;}
               case 9:{nine();break;}   
           }
    PORTDbits.RD7 = 1;
}
//================================================
