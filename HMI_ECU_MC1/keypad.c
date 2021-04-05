/*
 * Module: KEYPAD
 *
 * Description: SOURCE FILE FOR THE KEYPAD DRIVER
 *
 *  Created on: Mar 21, 2021
 *      Author: Shams Elgohary
 *******************************************************************************/

#include "keypad.h"

									// 	PRIVATE FUNCTION PROTOTYPES  //

#if N_COL == 3
static uint8 Keypad_4x3_AdjustKeyNumber(uint8 button);

#elif N_COL == 4
static uint8 Keypad_4x4_AdjustKeyNumber(uint8 button);
#endif

									// 			FUNCTIONS		 //

uint8 Keypad_GetPressedKey()
{
	uint8 col,row;

	while(1)
	{
		for (col=0; col < N_COL ; col++)
		{
			KEYPAD_PORT_DIR = (0b00010000<<col);  // Column pins will be outputs one at a time
			KEYPAD_PORT_OUT = (~(0b00010000<<col)); //Enabling Internal pull-ups (except at output pin)


			for (row=0 ; row < N_ROW ; row++)
			{
				if (BIT_IS_CLEAR(KEYPAD_PORT_IN,row)) // Check the row if it equals 0
				{
					#if N_COL == 3
					return Keypad_4x3_AdjustKeyNumber((row*N_COL)+col+1);

					#elif N_COL == 4
					return Keypad_4x4_AdjustKeyNumber((row*N_COL)+col+1);
					#endif
				}
			}
		}
	}
}


static uint8 Keypad_4x3_AdjustKeyNumber(uint8 button)
{
	switch(button)
	{
	case 10: return '*';	 break;
	case 11: return  0; 	 break;
	case 12: return '#';	 break;
	default: return button;

	}
}

static uint8 Keypad_4x4_AdjustKeyNumber(uint8 button)
{
	switch(button)
	{
	case 1:  return '7';	 	break;
	case 2:  return '8';	 	break;
	case 3:  return '9';	 	break;
	case 4:  return '/';	 	break; //ASCII CODE OF DIVIDE
	case 5:  return '4';		break;
	case 6:  return '5';		break;
	case 7:  return '6';	 	break;
	case 8:  return '*';		break;
	case 9:  return '1'; 	 	break;
	case 10: return '2';		break;
	case 11: return '3';	 	break;
	case 12: return '-';	 	break;
	case 13: return 13;		 	break; //ASCII CODE OF ENTER
	case 14: return '0';    	break;
	case 15: return '=';   		break;
	case 16: return '+';		break;
	default: return button;
	}
}

