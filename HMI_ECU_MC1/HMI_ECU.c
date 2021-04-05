/*
 *              SOURCE FILE OF THE HUMAN MACHINE INTERFACE ELECTRONIC CONTROL UNIT
 *
 *  Created on: Mar 28, 2021
 *      Author: Shams Elgohary
 *******************************************************************************/

#include "timer.h"
#include "lcd.h"
#include "keypad.h"
#include "uart.h"

#define DELAY_Keypad 2500
#define DELAY_UART 130

								/* Function Prototypes */

void EnterPW(uint8 PW[] );

void SendPW_UART( uint8 PW[] );


int main(void)
{
	uint8 password[4];
	uint8 confirm [4];
	uint8 command;
	volatile uint8 checkPW_Flag;


	LCD_init();
    LCD_clearScreen();

	UART_Config UConfig = {ASYNCH,ENABLED_EVEN,BIT_1};
	UART_init(&UConfig);

	TimerConfig T1_Configuration={CTC,F_CPU_1024,0,7812};  // TIMER1 CONFIGURATIONS
	Timer1_init(&T1_Configuration); 			 //Initialize timer1
	SREG |= (1<<7);


 /////////////////////////////////////*  PHASE 1	*/////////////////////////////////////////////

        do{
        						//  ENTERING PASSWORD //

        LCD_clearScreen();
    	LCD_displayString("Enter Password");
    	EnterPW( password );            					 // SEND ADDRESS OF password[4] TO EnterPW()
    	_delay_ms(DELAY_UART);
    	SendPW_UART ( password );						 	 // SEND PW TO MC2 via UART

   								//  RE-ENTERING PASSWORD //

        LCD_clearScreen();
    	LCD_displayString("Re-enter PW");
    	EnterPW( confirm );
    	_delay_ms(DELAY_UART);
     	SendPW_UART ( confirm );

    							 // VERIFYING PASSWORD //

     	//MC2 WILL SEND -> 1 IF PW'S MATCHES -> 0 PW'S ARE INVALID
     	checkPW_Flag=UART_receiveByte();

     	if (checkPW_Flag == 0)
     	{
     		LCD_clearScreen();
     		LCD_displayStringRowColumn(0,4,"INVALID");
     		_delay_ms(DELAY_Keypad);
     	}

        }while (checkPW_Flag== 0);




        LCD_clearScreen();
    	LCD_displayString("Correct Password");
        _delay_ms(DELAY_Keypad);


 while(1)
 {
/////////////////////////////////////////*  PHASE 2	*/////////////////////////////////////////////

        LCD_clearScreen();									  // '+' -> New Password '-' -> Open door
        LCD_displayStringRowColumn(0,0,"+ : Change PW");
        LCD_displayStringRowColumn(1,0,"- : Open Door");

        /*
         *  SAVE USERS CHOICE IN command
         *  SEND command TO MC2
         */

        command=Keypad_GetPressedKey();
        _delay_ms(DELAY_Keypad);
        UART_sendByte(command);
        _delay_ms(DELAY_UART);


 /////////////////////////////////////// CHANGE PASSWORD ////////////////////////////////////////////

        if (command == '+')
        {
        	do{
            						//  ENTERING PASSWORD  //

            LCD_clearScreen();
        	LCD_displayString("Enter Password");
        	EnterPW( password );            					 // SEND ADDRESS OF password[4] TO EnterPW()
        	SendPW_UART ( password );						 	 // SEND PW TO MC2 via UART

       								//  RE-ENTERING PASSWORD //

            LCD_clearScreen();
        	LCD_displayString("Re-enter PW");
        	EnterPW( confirm );
         	SendPW_UART ( confirm );

        							 // VERIFYING PASSWORD //

         	//MC2 WILL SEND -> 1 IF PW'S MATCHES -> 0 PW'S ARE INVALID
         	checkPW_Flag=UART_receiveByte();
         	_delay_ms(DELAY_UART);

         	if (checkPW_Flag == 0)
         	{
         		LCD_clearScreen();
         		LCD_displayStringRowColumn(0,4,"INVALID");
         		_delay_ms(DELAY_Keypad);
         	}

            }while (checkPW_Flag == 0);

            LCD_clearScreen();
        	LCD_displayString("Password Changed");
            _delay_ms(DELAY_Keypad);
        }


    ///////////////////////////////////////* PHASE 3 *//////////////////////////////////////////////

  ////////////////////////////////////////  OPEN DOOR  //////////////////////////////////////////////

         else if (command == '-')
    	 {

  //////////// ENTER PASSWORD TO CHECK IF PASSWORD MATCHES SAVED PW IN EEPROM AND ACTIVATE MOTOR /////////

    		uint8 count=0;
        	do{
        		count++;
            						//  ENTERING PASSWORD  //

            LCD_clearScreen();
        	LCD_displayString("Enter Password");
        	EnterPW( password );            					 // SEND ADDRESS OF password[4] TO EnterPW()
        	SendPW_UART ( password );						 	 // SEND PW TO MC2 via UART


        							 // VERIFYING PASSWORD //

         	//MC2 WILL SEND -> 1 IF PW'S MATCHES -> 0 PW'S ARE INVALID
         	checkPW_Flag=UART_receiveByte();
         	_delay_ms(DELAY_UART);

         	if (checkPW_Flag == 0)
         	{
         		LCD_clearScreen();
         		LCD_displayStringRowColumn(0,4,"INVALID");
         		_delay_ms(DELAY_Keypad);
         	}

            }while (checkPW_Flag== 0 && count < 3);

  ///////////////////////// SEND 1 TO CONTROL ECU IF PW MATCHES ////////////////////////

    			if(checkPW_Flag)
    				{

    				/* DOOR OPENS IN 15 SECONDS AND STAYS OPENED FOR 3 SECONDS AND STARTS
    				 *  CLOSING AGAIN IN 15 SECONDS */

    				SECONDS_T1_MC1 = 0;
    				LCD_clearScreen(); LCD_displayString("Opening Door");
    				while ( SECONDS_T1_MC1 <= 15);
    				LCD_clearScreen(); LCD_displayString("Door Open");
    				while ( SECONDS_T1_MC1 <= 18);
    				LCD_clearScreen(); LCD_displayString("Closing Door");
    				while ( SECONDS_T1_MC1 <= 33);

    				}


  ///////////////////// SEND 0 TO CONTROL ECU IF PW DOESN'T MATCH TO ACTIVATE BUZZER ///////////////

    			else if(checkPW_Flag==0)
    	    		{
    				LCD_clearScreen(); LCD_displayStringRowColumn(0,5,"ALARM");
    				SECONDS_T1_MC1 = 0;
    				while ( SECONDS_T1_MC1 < 60);       // LOCK MC1 FOR 60 SECONDS
    	    		}

    	   }
   }
  }



void EnterPW(uint8 PW[] )
{
	LCD_goToRowColumn(1, 5);
	for (uint8 i = 0; i < 5; i++) {
		while ((Keypad_GetPressedKey() != 13) && (i == 4));  // POLLING UNTIL ON BUTTON PRESSED

		PW[i] = Keypad_GetPressedKey();
		_delay_ms(DELAY_Keypad);				//DELAY SO KEYPAD DOESN'T REPEAT
		LCD_displayCharacter('*');
	}
	_delay_ms(DELAY_Keypad);
}


void SendPW_UART( uint8 PW[] )
{
		for (uint8 i=0 ; i<4 ; i++)
		{
			UART_sendByte( PW[i] );
			_delay_ms(DELAY_UART);
		}

}
