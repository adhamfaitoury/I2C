/*
 * File:   I2C.c
 * Author: M3MO
 *
 * Created on 28 ????, 2020, 03:40 ?
 */


#include <xc.h>
#include"I2C_Master.h"


void I2C_Master_Init() {
    TWSR |=(TWPS0<<0);
    TWBR = ((F_CPU/SCL_CLK) - 16) / (2*pow(4,TWPS0));
}

int Master_Start(char Address) {

    int status;
    TWCR |= (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT))); /*Wait until TWI finish its current job (start condition) */
    status = TWSR & 0xF8;
    if (status != 0x08)
        return 0;
    TWDR = Address;
    TWCR = (1 << TWEN) | (1 << TWINT);

    while (!(TWCR & (1 << TWINT)));
    status = TWSR & 0xF8;
    if (status == 0x18)
        return 1;
    if (status == 0x20)
        return 2;
    else
        return 3;
}

void Master_Start_Wait(char write_address)						/* I2C start wait function */
{
	uint8_t status;											/* Declare variable */
	while (1)
	{
		TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);				/* Enable TWI, generate start condition and clear interrupt flag */
		while (!(TWCR & (1<<TWINT)));						/* Wait until TWI finish its current job (start condition) */
		status = TWSR & 0xF8;								/* Read TWI status register with masking lower three bits */
		if (status != 0x08)									/* Check weather start condition transmitted successfully or not? */
		continue;											/* If no then continue with start loop again */
		TWDR = write_address;								/* If yes then write SLA+W in TWI data register */
		TWCR = (1<<TWEN)|(1<<TWINT);						/* Enable TWI and clear interrupt flag */
		while (!(TWCR & (1<<TWINT)));						/* Wait until TWI finish its current job (Write operation) */
		status = TWSR & 0xF8;								/* Read TWI status register with masking lower three bits */
		if (status != 0x18 )								/* Check weather SLA+W transmitted & ack received or not? */
		{
			Master_Stop();										/* If not then generate stop condition */
			continue;										/* continue with start loop again */
		}
		break;												/* If yes then break loop */
	}
}

int Master_REStart(char Address) {

    int status;
    TWCR |= (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT))); /*Wait until TWI finish its current job (start condition) */
    status = TWSR & 0xF8;
    if (status != 0x10)
        return 0;
    TWDR = Address;
    TWCR = (1 << TWEN) | (1 << TWINT);

    while (!(TWCR & (1 << TWINT)));
    status = TWSR & 0xF8;
    if (status == 0x40)
        return 1;
    if (status == 0x48)
        return 2;
    else
        return 3;
}

int Master_Write(char Address) {
    int status;
    TWDR = Address;
    TWCR = (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
    status = TWSR & 0xF8;
    if (status == 0x28)
        return 0;
    if (status == 0x30)
        return 1;
    else
        return 2;
}

int Master_Read(char status) {
    switch (status) {
        case ACK:
            TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
            while (!(TWCR & (1 << TWINT)));
            return TWDR;

        case NACK:
            TWCR = (1 << TWEN) | (1 << TWINT);
            while (!(TWCR & (1 << TWINT)));
            return TWDR;
        default:

            printf("please Enter ACK or NACK");
    }
    return 1;
}

void Master_Stop() {
    TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
    while (TWCR & (1 << TWSTO)); //wait till stop is finished
}

