/*
 * Module: EXTERNAL EEPROM
 *
 * Description: HEADER FILE FOR THE EXTERNAL EEPROM DRIVER
 *
 *  Created on: Mar 29, 2021
 *      Author: Shams Elgohary
 *******************************************************************************/
#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

						   //   LIBRARIES	//
#include "std_types.h"


						//	PREPROCCESOR  //
#define ERROR 0
#define SUCCESS 1

					// 	FUNCTION PROTOTYPES	//
void EEPROM_init(void);
uint8 EEPROM_writeByte(uint16 u16address,uint8 u8data);
uint8 EEPROM_readByte(uint16 u16address,uint8 *u8data);

#endif /* EXTERNAL_EEPROM_H_ */
