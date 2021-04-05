#include "motor.h"


/**************************************Functions Definitions******************************************/


void MOTOR_init(void)
{
	/* A , B Outputs of MICROCONTROLLER */
	SET_BIT(MOTOR_PORT_DIR,A);
	SET_BIT(MOTOR_PORT_DIR,B);

	/* Motor Moves clockwise by default */
//	MOTOR_clockw;
}


