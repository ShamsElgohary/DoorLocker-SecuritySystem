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
#ifndef TWI_H_
#define TWI_H_

								//   LIBRARIES	//
#include "common_macros.h"
#include "std_types.h"
#include "micro_config.h"

/*								// PREPROCESSOR //
#define PRESCALER_1  	TWSR &= ~(1<<TWPS1) & ~(TWPS0)   // TWPS1 TWPS0 0 0
#define PRESCALER_4		TWSR |= (1<<TWPS0)				 // TWPS1 TWPS0 0 1
#define PRESCALER_16	TWSR |= (1<<TWPS1)				 // TWPS1 TWPS0 1 0
#define PRESCALER_64	TWSR |= (1<<TWPS1) | (1<<TWPS0) // TWPS1 TWPS0 1 1
*/

/* I2C Status Bits in the TWSR Register */
#define TW_START         0x08 // start has been sent
#define TW_REP_START     0x10 // repeated start
#define TW_MT_SLA_W_ACK  0x18 // Master transmit ( slave address + Write request ) to slave + Ack received from slave
#define TW_MT_SLA_R_ACK  0x40 // Master transmit ( slave address + Read request ) to slave + Ack received from slave
#define TW_MT_DATA_ACK   0x28 // Master transmit data and ACK has been received from Slave.
#define TW_MR_DATA_ACK   0x50 // Master received data and send ACK to slave
#define TW_MR_DATA_NACK  0x58 // Master received data but doesn't send ACK to slave

								// Types Declaration //
typedef enum
{
	PRESCALE_1,PRESACLE_4,PRESCALE_16,PRESCALE_64
}I2C_Clock;


typedef struct
{
	I2C_Clock clock;
	uint8 BitRate;
	uint8 slave_address;

}I2C_Config;

							// FUNCTION PROTOTYPES //
void TWI_init(const I2C_Config *configuration); 		 // Initialize TWI/I2C
void TWI_start();										 //SEND START BIT
void TWI_stop();										 //SEND STOP BIT
void TWI_write(uint8 data);
uint8 TWI_read_ACK();
uint8 TWI_read_NACK();
uint8 TWI_getStatus();


#endif /* TWI_H_ */
