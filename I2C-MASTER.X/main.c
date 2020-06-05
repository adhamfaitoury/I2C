/*
 * File:   main.c
 * Author: Faitoury
 *
 * Created on 28 May, 2020, 03:18 
 */

#include "lcd.h"
#include "I2C_Master.h"	
#include <avr/io.h>							/* Include AVR std. library file */
#include <util/delay.h>						/* Include inbuilt defined Delay header file */
#include <string.h>							/* Include string header file */

char Slav_W= 0x20;
char Slav_R= 0x21;
char mast[]="Master";
char SEND[]="sending:";
char RECEV[]="Receiving:";
int max=10;
int i;

int main(void) {
    char buffer [10];
    I2C_Master_Init();
    LCD_Init();
    LCD_String_xy(0,0,mast);
   while(1){
        LCD_String_xy(1,0,SEND);
        Master_Start_Wait(Slav_W);;
        // Transmitting 
        for (i=0 ;i<max;i++){
            sprintf(buffer,"%d",i);
            LCD_String_xy(1,13,buffer);
            Master_Write(i);
            _delay_ms(100);
        }
        // Receiving (Repeated Start)
        LCD_String_xy(1,0,RECEV);
        Master_REStart(Slav_R);
        _delay_ms(5);
        for(i=0 ; i<max ;i++){
            if(i <max-1)
                sprintf(buffer,"%d",Master_Read(ACK));
            else
                sprintf(buffer,"%d",Master_Read(NACK));
            LCD_String_xy(1,13,buffer);
            _delay_ms(100);
                
        }
        Master_Stop();
        
    }
    return 0;
}
