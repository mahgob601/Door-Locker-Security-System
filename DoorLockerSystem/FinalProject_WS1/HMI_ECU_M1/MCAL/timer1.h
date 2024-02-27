/*
 * timer1.h
 *
 *  Created on: Nov 3, 2023
 *      Author: Yassin
 *      Description: timer1 module header file
 */

#ifndef MCAL_TIMER1_H_
#define MCAL_TIMER1_H_

#include "../std_types.h"


typedef enum
{
	NO_CLOCK = 0,F_CPU_1,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Timer1_Prescaler;

typedef enum{
	NORMAL_MODE, COMPARE_MODE
} Timer1_Mode;

typedef struct {
uint16 initial_value;
uint16 compare_value; // it will be used in compare mode only.
Timer1_Prescaler prescaler;
Timer1_Mode mode;
} Timer1_ConfigType;
void Timer1_init(const Timer1_ConfigType * Config_Ptr);
void Timer1_deInit(void);
void Timer1_setCallBack(void(*a_ptr)(void));

#endif /* MCAL_TIMER1_H_ */
