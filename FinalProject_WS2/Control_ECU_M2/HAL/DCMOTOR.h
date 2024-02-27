/*
 * DCMOTOR.h
 *
 *  Created on: Oct 13, 2023
 *      Author: Yassin
 */

#ifndef HAL_DCMOTOR_H_
#define HAL_DCMOTOR_H_
#include "../std_types.h"
#include "../MCAL/gpio.h"


/*
 * input1 is PB0
 * input2 is PB2
 * enable(OC0) pin is PB3
 *
 */
#define DCMOTOR_CONFIG_PORT PORTB_ID
#define DCMOTOR_IN1 PIN0_ID
#define DCMOTOR_IN2 PIN2_ID
#define DCMOTOR_ENABLE PIN3_ID

/*
 * define motor state as enum
 */
typedef enum{
	OFF = 0, ACW, CW
}DcMotor_State;

void DcMotor_Init(void);
void DcMotor_Rotate(DcMotor_State state,uint8 speed);

#endif /* HAL_DCMOTOR_H_ */
