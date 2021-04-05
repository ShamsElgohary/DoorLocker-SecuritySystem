/*
 * Module: EXTERNAL EEPROM
 *
 * Description: SOURCE FILE FOR THE EXTERNAL EEPROM DRIVER
 *
 *  Created on: Mar 29, 2021
 *      Author: Shams Elgohary
 *******************************************************************************/
#include "twi.h"
#include "external_eeprom.h"

void EEPROM_init(void)
{
	I2C_Config config= { PRESCALE_1, 2 , 2 }; 					//TWBR =2 AND ADDRESS = 2
	TWI_init( &config );
}

uint8 EEPROM_writeByte(uint16 u16address,uint8 u8data)
{
		TWI_start();             								// SEND START BIT
		if ( TWI_getStatus() != TW_START) return ERROR;     //If status of TWI isn't start return 0 (ERROR)

		/* WRITE THE SLAVE ADDRESS (DEVICE MEMORY ADDRESS)
		   MEMORY ADDRESS =  1010 (A10 A9 A8)(W=0)          */
		TWI_write( (uint8) ( 0xA0 | ((u16address & 0x0700) >> 7)));
		if ( TWI_getStatus() != TW_MT_SLA_W_ACK) return ERROR;

		TWI_write((uint8) (u16address)); 							//WORD ADDRESS IN EEPROM
		if ( TWI_getStatus() != TW_MT_DATA_ACK) return ERROR;

		TWI_write(u8data);										// WRITE DATA IN WORD ADDRESS IN EEPROM
		if ( TWI_getStatus() != TW_MT_DATA_ACK) return ERROR;

		TWI_stop();												//SEND STOP BIT

		return SUCCESS;
}

uint8 EEPROM_readByte(uint16 u16address,uint8 *u8data)
{
		TWI_start();             								// SEND START BIT
		if ( TWI_getStatus() != TW_START) return ERROR;     //If status of TWI isn't start return 0 (ERROR)

		/* WRITE THE SLAVE ADDRESS (DEVICE MEMORY ADDRESS)
		   MEMORY ADDRESS =  1010 (A10 A9 A8)(W=0)          */
		TWI_write( (uint8) ( 0xA0 | ((u16address & 0x0700) >> 7)));
		if ( TWI_getStatus() != TW_MT_SLA_W_ACK) return ERROR;

		TWI_write((uint8) (u16address)); 							//WORD ADDRESS IN EEPROM
		if ( TWI_getStatus() != TW_MT_DATA_ACK)  return ERROR;

		TWI_start();             								// SEND REPEATED START BIT
		if (TWI_getStatus() != TW_REP_START)     return ERROR;

		/* WRITE THE SLAVE ADDRESS (DEVICE MEMORY ADDRESS) AGAIN THIS TIME WITH READ BIT 1
		   MEMORY ADDRESS =  1010 (A10 A9 A8)(R=1)          */
	    TWI_write((uint8)((0xA0) | ((u16address & 0x0700)>>7) | 1));
	    if (TWI_getStatus() != TW_MT_SLA_R_ACK)   return ERROR;

	    *u8data = TWI_read_NACK();      						// Read Byte from Memory without send ACK
	    if (TWI_getStatus() != TW_MR_DATA_NACK) return ERROR;

	    TWI_stop();  										// SEND STOP BIT

	    return SUCCESS;
}
