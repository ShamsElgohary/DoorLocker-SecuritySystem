/*
 *                    SOURCE FILE OF THE CONTROL ELECTRONIC CONTROL UNIT
 *
 *  Created on: Mar 28, 2021
 *      Author: Shams Elgohary
 *******************************************************************************/

#include "timer.h"
#include "external_eeprom.h"
#include "uart.h"
#include "motor.h"

#define DELAY_Keypad 2500
#define DELAY_UART 130
								// GLOBAL VARIABLES //
volatile uint8 Valid_Flag;
								// FUNCTION PROTOTYPES //
void RECEIVE_PW(uint8 PassW[] );
void VERIFY_PW( uint8 PassW[] , uint8 CheckPW[] );


int main()
{
	uint8 PW[4];
	uint8 CHECK[4];

	DDRD |= (1<<PD7) | (1<<PD6); 		//PD6 AND PD7 OUTPUT PINS FOR THE BUZZER
	PORTD &= ~(1<<PD7) & ~(1<<PD6);		// BY DEFAULT OUTPUT PINS ARE 0

	EEPROM_init();
	MOTOR_init();

	UART_Config UConfig = {ASYNCH,ENABLED_EVEN,BIT_1};
	UART_init(&UConfig);

	TimerConfig T1_Configuration={CTC,F_CPU_1024,0,7813};
	Timer1_init(&T1_Configuration); 			 //Initialize timer1
	SREG |= (1<<7);



		/* RECIEVE PASSWORDS AND VERIFY THE PW'S FROM THE HMI ECU */
		do{
		RECEIVE_PW( PW );        	// RECIEVE FIRST PW USER SENDS
		RECEIVE_PW( CHECK );        // RECIEVE VERIFYING PW USER SENDS
		VERIFY_PW (PW,CHECK);		//CHECK IF PW'S SENT FROM THE HMI MATCH
		}while (Valid_Flag == 0);


while(1)
{
/////////////////////////////////////////* PHASE 2 *//////////////////////////////////////////////

		uint8 command=UART_receiveByte();

		if (command=='+')
		{
			/* RECIEVE PASSWORD AND VERIFYING PW FROM THE HMI ECU */
			do{
			RECEIVE_PW( PW );        	// RECIEVE FIRST PW USER SENDS
			RECEIVE_PW( CHECK );        // RECIEVE VERIFYING PW USER SENDS
			VERIFY_PW (PW,CHECK);		//CHECK IF PW'S SENT FROM THE HMI MATCH
			}while (Valid_Flag == 0);
		}


//////////////////////////////////* SAVE PASSWORD IN EEPROM *//////////////////////////

		for (uint8 i = 0; i < 4; i++) {
			// Increment address in EEPROM for every new byte in PW array
			EEPROM_writeByte( (0x0090 + i), PW[i] );
			_delay_ms(DELAY_UART);
		}

/////////////////////////////////////////* PHASE 3 *//////////////////////////////////////////////////


		if (command=='-')			/* RECIEVE "command" OF USER FROM THE HMI ECU */
		{
			/* GET PASSWORD IN EEPROM AND SAVE IT IN A VARIABLE TO CHECK PW USER SENT */
			for (uint8 i = 0; i < 4; i++)
			  {  EEPROM_readByte( (0x0090 + i), (PW+i)); _delay_ms(100); }


			/* 3 CHANCES FOR USER TO ENTER PW CORRECTLY */

			uint8 count=0;
			do{
			count++;
			RECEIVE_PW( CHECK );        			// RECIEVE PW USER SENDS WE WANT TO CHECK
			VERIFY_PW (PW,CHECK);					//CHECK IF PW USER SENT IS CORRECT
			} while(Valid_Flag == 0 && count<3);


			if (Valid_Flag)
			{
			/* DOOR OPENS IN 15 SECONDS (MOVES CLOCKWISE) AND THEN THE DOOR
			 * IS OPENED FOR 3 SECONDS AND THEN CLOSES IN 15 SECONDS */
			SECONDS_T1_MC2 = 0;
			while ( SECONDS_T1_MC2 <= 15)   { 	MOTOR_clockw;       }
			while ( SECONDS_T1_MC2 <= 18)	{   MOTOR_stop;			}
			while ( SECONDS_T1_MC2 <= 33)   {   MOTOR_anti_clockw;  }
			MOTOR_stop;
			}

			else if(!Valid_Flag) 							//ALARM -> ACTIVATE BUZZER
			{
			PORTD |= (1<<PD6);
			PORTD &= ~(1<<PD7);
			SECONDS_T1_MC2 = 0;
			while ( SECONDS_T1_MC2 <= 60);         			// LOCK MC1 FOR 60 SECONDS
			PORTD &= ~(1<<PD6);
			}

		}

	}
}

void RECEIVE_PW(uint8 PassW[] )
{
	for (uint8 i = 0; i < 4; i++) {
		PassW[i]=UART_receiveByte();
	}
	_delay_ms(DELAY_UART);

}


void VERIFY_PW (uint8 PassW[] , uint8 CheckPW[])
{

	for (uint8 i = 0; i < 4; i++)
	{
		if (PassW[i] != CheckPW[i])			//IF ONE CHAR IS DIFFRENT THEN PW IS INVALID
		{
			UART_sendByte(0);
			_delay_ms(DELAY_UART);
			Valid_Flag=0;
			break;
		}
		Valid_Flag=1;
	}

	if(Valid_Flag) {	UART_sendByte(1); _delay_ms(DELAY_UART);}
}
