/*
 * File:   Slave_main.c
 * Author: Faitoury
 *
 * Created on 28 May, 2020, 6:17 pm
 */


#define F_CPU 16000000UL							/* Define CPU clock Frequency e.g. here its 8MHz */
#include <avr/io.h>								/* Include AVR std. library file */
#include <util/delay.h>							/* Include inbuilt defined Delay header file */
#include <stdio.h>								/* Include standard I/O header file */
#include <string.h>								/* Include string header file */
#include "lcd.h"					/* Include LCD header file */
#include "I2C_Slave.h"					/* Include I2C slave header file */

char slv[]= "Slave";
char recev[]="Receiving:";
char send[]="Sending:";

char Slave_Address =0x21 ;

int main(void)
{
	char buffer[10];
	int max = 10;
	
	LCD_Init();
	I2C_Slave_Init(Slave_Address);
	
	LCD_String_xy(0, 0, slv);
	
	while (1)
	{
		switch(Slave_Listen())				/* Check for any SLA+W or SLA+R */
		{
			case 0:
			{
				LCD_String_xy(1, 0,recev );
				do
				{
					sprintf(buffer, "%d", max);
					LCD_String_xy(1, 13, buffer);
					max = Slave_Receive();           /* Receive data byte*/
				} while (max != -1);			/* Receive until STOP/REPEATED START received */
				max = 0;
				break;
			}
			case 1:
			{
				int Ack_status;
				LCD_String_xy(1, 0, send);
				do
				{
					Ack_status = Slave_Transmit(max);	/* Send data byte */
					sprintf(buffer, "%d    ",max);
					LCD_String_xy(1, 13, buffer);
					max++;
				} while (Ack_status == 0);		/* Send until Acknowledgment is received */
				break;
			}
			default:
				break;
		}
	}
}