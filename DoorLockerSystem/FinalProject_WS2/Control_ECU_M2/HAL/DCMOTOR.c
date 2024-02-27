/*
 * DCMOTOR.c
 *
 *  Created on: Oct 13, 2023
 *      Author: Yassin
 */
#include "DCMOTOR.h"
#include "../MCAL/pwm.h"

/*
 * Initializes dc motor pins
 * and sets it at stop mode
 */
void DcMotor_Init(void)
{
	/*
	 * setup the direction for the two motor pins through the GPIO driver
	 */
	GPIO_setupPinDirection(DCMOTOR_CONFIG_PORT,DCMOTOR_IN1,PIN_OUTPUT);
	GPIO_setupPinDirection(DCMOTOR_CONFIG_PORT,DCMOTOR_IN2,PIN_OUTPUT);


	/*
	 * Stop at the DC-Motor at the beginning through the GPIO driver
	 */
	GPIO_writePin(DCMOTOR_CONFIG_PORT,DCMOTOR_IN1,LOGIC_LOW);
	GPIO_writePin(DCMOTOR_CONFIG_PORT,DCMOTOR_IN2,LOGIC_LOW);
}

/*
 * controlling speed and direction of rotation
 */
void DcMotor_Rotate(DcMotor_State state,uint8 speed)
{
	switch(state)
	{
	case OFF:
		PWM_Timer0_Start(LOGIC_LOW);
		GPIO_writePin(DCMOTOR_CONFIG_PORT,DCMOTOR_IN1,LOGIC_LOW);
		GPIO_writePin(DCMOTOR_CONFIG_PORT,DCMOTOR_IN2,LOGIC_LOW);
		break;
	case CW:
		PWM_Timer0_Start(speed);
		GPIO_writePin(DCMOTOR_CONFIG_PORT,DCMOTOR_IN1,LOGIC_HIGH);
		GPIO_writePin(DCMOTOR_CONFIG_PORT,DCMOTOR_IN2,LOGIC_LOW);
		break;
	case ACW:
		PWM_Timer0_Start(speed);
		GPIO_writePin(DCMOTOR_CONFIG_PORT,DCMOTOR_IN1,LOGIC_LOW);
		GPIO_writePin(DCMOTOR_CONFIG_PORT,DCMOTOR_IN2,LOGIC_HIGH);
		break;

	}


}
