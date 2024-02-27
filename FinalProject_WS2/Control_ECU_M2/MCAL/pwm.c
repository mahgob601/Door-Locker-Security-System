#include "pwm.h"

/******************************************************************************
 *
 * Module: pwm
 *
 * File Name: pwm.c
 *
 * Description: Source file for the AVR timer0 pwm driver
 *
 * Author: Yassin
 *
 *******************************************************************************/

/* Description:
 * Using PWM to operate dc motor at different speeds
 * Duty cycle of pwm controls the motor speed
 * PWM PIN of Timer0 OC0(PB3) to the enable of the Motor.
 * duty cycle 100% --> OCR0 = 255 Maximum Speed
 * duty cycle 0%   --> Motor Stop
 */

/*
 * Function Description:
 * The function decides the compare value based on duty cycle passed
 * The function sets up PB3/OC0 as output pin to allow wave generation (pwm mode)
 * The function initializes timer0 control register and configures it to
 * fast non-inverted pwm mode with 8 as a pre-scaler
 *
 */
void PWM_Timer0_Start(uint8 duty_cycle)
{

	TCNT0 = 0;
	switch(duty_cycle)
	{
	case 0:
		/*
		 * 0% duty cycle
		 */
		OCR0 = 0;
		break;
	case 100:
		/*
		 * 100% duty cycle
		 */
		OCR0 = 255;
		break;

	}


	GPIO_setupPinDirection(PWM_T0_PORT, PWM_T0_OC0_PIN, PIN_OUTPUT);

	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
	 */

	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);

}
