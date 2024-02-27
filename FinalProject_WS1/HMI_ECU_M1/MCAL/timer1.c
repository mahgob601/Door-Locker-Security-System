/*
 * timer1.c
 *
 *  Created on: Nov 3, 2023
 *      Author: Yassin
 *      Description: timer1 module source file
 */

#include <avr/io.h>
#include "timer1.h"
#include <avr/interrupt.h>

/*
 * Description:
 * Function to initialize the Timer driver
 * Supporting all modes of operation: Compare and Normal
 * Supporting All configurations of pre-scaler, initial and
 * compare value
 *
 */

static volatile void (*g_callBackPtr) (void) = NULL_PTR;
void Timer1_init(const Timer1_ConfigType * Config_Ptr)
{

	TCCR1A = (1<<FOC1A) | (1<<FOC1B);
	TCNT1 = (Config_Ptr->initial_value);
	//TCNT1L = (Config_Ptr->initial_value) & (0xFF);

	switch(Config_Ptr->prescaler)
	{
	case NO_CLOCK:
		TCCR1B = (0<<CS12) | (0<<CS11) | (0<<CS10);
		break;
	case F_CPU_1:
		TCCR1B = (0<<CS12) | (0<<CS11) | (1<<CS10);
		break;
	case F_CPU_8:
		TCCR1B = (0<<CS12) | (1<<CS11) | (0<<CS10);
		break;
	case F_CPU_64:
		TCCR1B = (0<<CS12) | (1<<CS11) | (1<<CS10);
		break;
	case F_CPU_256:
		TCCR1B = (1<<CS12) | (0<<CS11) | (0<<CS10);
		break;
	case F_CPU_1024:
		TCCR1B = (1<<CS12) | (0<<CS11) | (1<<CS10);
		break;

	}

	/*
	 * In case of CTC mode
	 * enable the compare match interrupt enable
	 */
	if(Config_Ptr->mode == COMPARE_MODE)
	{
		/*WGM12 must be set to 1 in case of CTC mode*/
		TCCR1B |= (1<<WGM12);
		/*loading the compare value to the OCR1 compare reg*/
		OCR1A = (Config_Ptr->compare_value);

		/* enable interrupt Timer/Counter1, Output Compare A Match Interrupt Enable
		 * on compare match with OCR1A
		 */
		TIMSK |= (1<<OCIE1A);
	}

	/*
	 * In case of normal mode
	 * enable the overflow interrupt enable
	 */
	else if (Config_Ptr->mode == NORMAL_MODE)
	{
		/*Timer/Counter1, Overflow Interrupt Enable*/
		TIMSK |= (1<<TOIE1);
	}

}

/*
 * Description:
 * De-initializing the Timer1 by
 * mainly resetting clock select to zero
 * compare reg to zero
 * initial value count to zero
 * disabling all time driven interrupts
 */
void Timer1_deInit(void)
{
	/*setting clock select in TCCR1B = 0 */
	TCCR1B = 0;
	/*setting compare register OCR1A = 0 */
	OCR1A = 0;
	TCNT1 = 0;
	/*disabling overflow and ctc interrupts*/
	TIMSK &= ~((1<<TOIE1) | (1<<OCIE1A));
	g_callBackPtr = NULL_PTR;


}

void Timer1_setCallBack(void(*a_ptr)(void))
{
	g_callBackPtr = a_ptr;

}

// Interrupt Service Routine for timer1 compare mode channel A
ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		(*g_callBackPtr)();
	}
}

ISR(TIMER1_OVF_Vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		(*g_callBackPtr)();
	}

}

