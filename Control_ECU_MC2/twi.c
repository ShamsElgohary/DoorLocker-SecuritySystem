/*
 * Module: I2C/TWI
 *
 * File Name: twi.c
 *
 * Description: SOURCE FILE FOR THE AVR I2C/TWI DRIVER
 *
 *  Created on: Mar 29, 2021
 *      Author: Shams Elgohary
 *******************************************************************************/

#include "twi.h"


void TWI_init( const I2C_Config *configuration )
{

	TWSR |= ( (TWSR & ~ 0X03) | configuration->clock );			 //ADJUSTS PRESCALE

	TWAR = 	  (~0X01 & configuration->slave_address );			//SLAVE ADDRESS FROM BIT 1 TO BIT 7

	TWBR = configuration->BitRate;

	TWCR = (1<<TWEN);  // TWI ENABLE BIT

}

void TWI_start()
{
    /*
     * START BIT SEND BY ENABLING TWSTA
	 * CLEAR TWINT FLAG BEFORE SENDING START BIT (CLEARED BY WRITING LOGIC 1 TO IT TWINT=1 )
	 * Enable TWI Module
	 */
	TWCR = (1 <<TWSTA) | (1 <<TWEN) | (1 <<TWINT);

    while(BIT_IS_CLEAR(TWCR,TWINT));  //KEEP POLLING UNTIL INT FLAG IS SET (START BIT SENT SUCCESFULLY)
}

void TWI_stop()
{
    /*
	 * Clear the TWINT flag before sending the stop bit TWINT=1
	 * send the stop bit by TWSTO=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void TWI_write(uint8 data)
{
	TWDR=data; 						// TRANSFER DATA
	TWCR = (1 <<TWINT)| (1 <<TWEN); // RESET TWINT FLAG BY WRITING LOGIC 1 TO IT | ENABLE TWI
    while(BIT_IS_CLEAR(TWCR,TWINT));  //KEEP POLLING UNTIL INT FLAG IS SET (DATA SENT SUCCESSFULY)
}

uint8 TWI_read_ACK()
{
	TWCR = (1<<TWEN) | (1<<TWEA) | (1<<TWINT);
	while(BIT_IS_CLEAR(TWCR,TWINT));  //KEEP POLLING UNTIL INT FLAG IS SET (DATA RECIEVED SUCCESSFULY)
	return TWDR;
}

uint8 TWI_read_NACK()
{
	TWCR = (1<<TWEN) | (1<<TWINT);
	while(BIT_IS_CLEAR(TWCR,TWINT));  //KEEP POLLING UNTIL INT FLAG IS SET (DATA RECIEVED SUCCESSFULY)
	return TWDR; 					  //READ DATA
}

uint8 TWI_getStatus()
{
	uint8 status= ( (0XF8) & TWSR );
	return status;
}
