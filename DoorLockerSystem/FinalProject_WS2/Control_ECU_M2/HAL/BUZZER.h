/*
 * BUZZER.h
 *
 *  Created on: Nov 4, 2023
 *      Author: Yassin
 */

#ifndef HAL_BUZZER_H_
#define HAL_BUZZER_H_

#include "../MCAL/gpio.h"
#include "../std_types.h"

#define BUZZER_PIN PIN5_ID
#define BUZZER_PORT PORTB_ID

void Buzzer_init(void);
void Buzzer_on(void);
void Buzzer_off(void);

#endif /* HAL_BUZZER_H_ */
