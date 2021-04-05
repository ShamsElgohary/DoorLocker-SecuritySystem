/*
 * Module: KEYPAD
 *
 * Description: HEADER FILE FOR THE KEYPAD DRIVER
 *
 *  Created on: Mar 21, 2021
 *      Author: Shams Elgohary
 *******************************************************************************/

#ifndef KEYPAD_H_
#define KEYPAD_H_
								  /* 	   LIBRARIES		*/
#include "common_macros.h"
#include "std_types.h"
#include "micro_config.h"

								/*      PREPROCCESOR	   */

#define N_COL 4
#define N_ROW 4

							   	/* Keypad Port Configurations */

#define KEYPAD_PORT_OUT PORTA
#define KEYPAD_PORT_IN  PINA
#define KEYPAD_PORT_DIR DDRA

								/*		FUNCTIONS PROTOTYPE    */

uint8 Keypad_GetPressedKey();

#endif /* KEYPAD_H_ */
