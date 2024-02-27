/*
 * pwm.h
 *
 *  Created on: Oct 14, 2023
 *      Author: Yassin Mahgoub
 *      Description:
 */

#ifndef MCAL_PWM_H_
#define MCAL_PWM_H_
#include "../std_types.h"
#include "avr/io.h"
#include "gpio.h"

/*
 * Macro description:
 * Defining PORTB as the PWM port for ease of reference
 */
#define PWM_T0_PORT PORTB_ID

/*
 * Macro description:
 * Defining PB3/OC0 pin as the PWM OC0 pin for ease of reference
 */
#define PWM_T0_OC0_PIN PB3

/*
 * Function Description:
 * The function decides the compare value based on duty cycle passed
 * The function sets up PB3/OC0 as output pin to allow wave generation (pwm mode)
 * The function initializes timer0 control register and configures it to
 * fast non-inverted pwm mode with 8 as a pre-scaler
 *
 */
void PWM_Timer0_Start(uint8 duty_cycle);
#endif /* MCAL_PWM_H_ */
