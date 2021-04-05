/*
 * Module: UART
 *
 * Description: HEADER FILE FOR THE AVR UART DRIVER
 *
 *  Created on: Mar 24, 2021
 *      Author: Shams Elgohary
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "common_macros.h"
#include "std_types.h"
#include "micro_config.h"

// UART Driver Baud Rate //
#define USART_BAUDRATE 9600

typedef enum
{
	ASYNCH,SYNCH
}UART_Mode;

typedef enum
{
	DISABLED,RESERVED,ENABLED_EVEN,ENABLED_ODD
}UART_ParityMode;

typedef enum
{
	BIT_1,BIT_2
}UART_StopBit;

typedef struct
{
	UART_Mode mode;
	UART_ParityMode ParityBits;
	UART_StopBit StopBits;

}UART_Config;
							// FUNCTION PROTOTYPES //

void UART_init( UART_Config *config );
void UART_sendByte(uint8 data);
uint8 UART_receiveByte();
void UART_sendString(const char *str);
void UART_receiveString(char *str);


#endif /* UART_H_ */
