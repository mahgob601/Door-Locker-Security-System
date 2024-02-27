/*timer.c
/*
 * timer.c
 *
 *  Created on: Oct 28, 2023
 *      Author: Seif
 */


#include "../MCAL/timer1.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "../common_macros.h"
#include "../MCAL/gpio.h"

#ifdef TIMER0
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr_T0)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtr_T0 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr_T0)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtr_T0 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr_T0)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialization of Timer 1 by:
 * 1. Setup Timer Mode
 * 2. Setup Prescaler
 * 3. Setup the initial value
 * 4. Setup compare value if required
 * 5. Set PWM Pins as output if required
 * 6. Enable Interrupts
 */
void Timer0_init(const Timer0_ConfigType * Config_Ptr)
{
	TCCR0=(TCCR0 & 0xBF)|((Config_Ptr->mode & 0x01)<<6);
	TCCR0=(TCCR0 & 0xF7)|((Config_Ptr->mode & 0x02)<<2);  //configure mode of timer


	TCCR0=(TCCR0 & 0xF8)|(Config_Ptr->prescaler & 0x07); //setup prescaler

	TCNT0=Config_Ptr->initial_value; //define initial value

	if((Config_Ptr->mode!=NORM_T0)&&(Config_Ptr->mode!=CTC_T0))
	{
			GPIO_setupPinDirection(PWM_T0_ENABLE_PORT_ID,PWM_T0_ENABLE_PIN_ID,PIN_OUTPUT); //set pwm pin as output
	}

	if(Config_Ptr->mode!=NORM_T0)
	{
		TCCR0=(TCCR0 & 0XCF)|((Config_Ptr->comp_match & 0x03)<<4); //set compare match event
		OCR0=Config_Ptr->compare_value; //define compare value
		SET_BIT(TIMSK,OCIE0); //enable compare interrupt
	}
	else
	{
		SET_BIT(TIMSK,TOIE0); //enable overflow interrupt
	}
}

/*
 * Description :
 * Functional responsible for Deinitialization of Timer 1 by clearing all registers
 */
void Timer0_deInit(void)
{
	TCCR0=0;
	TCNT0=0;
	OCR0=0;
	CLEAR_BIT(TIMSK,OCIE0);
	CLEAR_BIT(TIMSK,TOIE0);
}

/*
 * Description: Function to set the Call Back function address.
 */
void Timer0_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr_T0 = a_ptr;
}

/*
 * Description: Adjust PWM compare value if input is percentage
 */
void Timer0_pwmAdjust(uint8 duty_cycle)
{
	duty_cycle  = (uint8)(((uint16)255*duty_cycle)/100); //define compare value
	Timer0_ConfigType ConfigPTR={0,duty_cycle,PS_8_T0,FAST_PWM_T0,CLEAR_T0}; //initialize timer as desired
	Timer0_init(&ConfigPTR);
}
#endif


#ifdef TIMER1
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr_T1)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr_T1 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr_T1)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_COMPB_vect)
{
	if(g_callBackPtr_T1 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr_T1)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr_T1 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr_T1)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*


 * Description :
 * Functional responsible for Initialization of Timer 1 by:
 * 1. Setup Timer Mode
 * 2. Setup Prescaler
 * 3. Setup the initial value
 * 4. Setup compare value if required
 * 5. Set PWM Pins as output if required
 * 6. Enable Interrupts
 */
void Timer1_init(const Timer1_ConfigType * Config_Ptr)
{
	TCCR1A=(TCCR1A & 0xFC)|(Config_Ptr->mode & 0x03); //configure timer mode
	TCCR1B=(TCCR1B & 0XE7)|((Config_Ptr->mode & 0x0C)<<1);

	TCCR1B=(TCCR1B & 0xF8)|(Config_Ptr->prescaler & 0x07); //setup prescaler

	TCNT1=Config_Ptr->initial_value; //define initial value


	//define pwm pin as output
	if((Config_Ptr->mode!=NORM_T1)&&(Config_Ptr->mode!=CTC_OCR_T1)&&(Config_Ptr->mode!=CTC_ICR_T1))
	{
		if(Config_Ptr->index==TIMER1A)
			{
			GPIO_setupPinDirection(PWM_T1A_ENABLE_PORT_ID,PWM_T1A_ENABLE_PIN_ID,PIN_OUTPUT);
			}
			else if(Config_Ptr->index==TIMER1B)
			{
				GPIO_setupPinDirection(PWM_T1B_ENABLE_PORT_ID,PWM_T1B_ENABLE_PIN_ID,PIN_OUTPUT);
			}
	}

	if(Config_Ptr->mode!=NORM_T1)
	{
		if(Config_Ptr->index==TIMER1A)
			{
				TCCR1A=(TCCR1A & 0x3F)|((Config_Ptr->comp_match & 0x03)<<6); //set compare match event
				OCR1A=Config_Ptr->compare_value; //define compare value
				SET_BIT(TIMSK,OCIE1A); //enable compare interrupt
			}
			else if(Config_Ptr->index==TIMER1B)
			{
				TCCR1A=(TCCR1A & 0xCF)|((Config_Ptr->comp_match & 0x03)<<4); //set compare match event
				OCR1B=Config_Ptr->compare_value; //define compare value
				SET_BIT(TIMSK,OCIE1B); //enable compare interrupt
			}
	}
	else
	{
		SET_BIT(TIMSK,TOIE1); //enable overflow interrupt
	}
}

/*
 * Description :
 * Functional responsible for Deinitialization of Timer 1 by clearing all registers
 */
void Timer1_deInit(void)
{
	TCCR1A=0;
	TCCR1B=0;
	TCNT1=0;
	OCR1A=0;
	OCR1B=0;
	CLEAR_BIT(TIMSK,OCIE1A);
	CLEAR_BIT(TIMSK,OCIE1B);
	CLEAR_BIT(TIMSK,TOIE1);
}

/*
 * Description: Function to set the Call Back function address.
 */
void Timer1_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr_T1 = a_ptr;
}

/*
 * Description: Adjust PWM compare value if input is percentage
 */
void Timer1_pwmAdjust(uint16 duty_cycle)
{
	duty_cycle  = (uint16)(((uint32)65535*duty_cycle)/100); //define compare value
	Timer1_ConfigType ConfigPTR={0,duty_cycle,PS_8_T1,FAST_PWM_OCR_T1,CLEAR_T1}; //initialize timer as desired
	Timer1_init(&ConfigPTR);
}
#endif


#ifdef TIMER2
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr_T2)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER2_COMP_vect)
{
	if(g_callBackPtr_T2 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr_T2)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER2_OVF_vect)
{
	if(g_callBackPtr_T2 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr_T2)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*


 * Description :
 * Functional responsible for Initialization of Timer 1 by:
 * 1. Setup Timer Mode
 * 2. Setup Prescaler
 * 3. Setup the initial value
 * 4. Setup compare value if required
 * 5. Set PWM Pins as output if required
 * 6. Enable Interrupts
 */
void Timer2_init(const Timer2_ConfigType * Config_Ptr)
{
	TCCR2=(TCCR2 & 0xBF)|((Config_Ptr->mode & 0x01)<<6);
	TCCR2=(TCCR2 & 0xF7)|((Config_Ptr->mode & 0x02)<<2); //set timer mode


	TCCR2=(TCCR2 & 0xF8)|(Config_Ptr->prescaler & 0x07); //setup precaler

	TCNT2=Config_Ptr->initial_value; //define initial value

	if((Config_Ptr->mode!=NORM_T2)&&(Config_Ptr->mode!=CTC_T2))
	{
			GPIO_setupPinDirection(PWM_T2_ENABLE_PORT_ID,PWM_T2_ENABLE_PIN_ID,PIN_OUTPUT); //set pwm as output
	}

	if(Config_Ptr->mode!=NORM_T2)
	{
		TCCR2=(TCCR2 & 0XCF)|((Config_Ptr->comp_match & 0x03)<<4); //set compare match event
		OCR2=Config_Ptr->compare_value; //define compare value
		SET_BIT(TIMSK,OCIE2); //enable compare interrupt
	}
	else
	{
		SET_BIT(TIMSK,TOIE2); //enable overflow interrupt
	}
}

/*
 * Description :
 * Functional responsible for Deinitialization of Timer 1 by clearing all registers
 */
void Timer2_deInit(void)
{
	TCCR2=0;
	TCNT2=0;
	OCR2=0;
	CLEAR_BIT(TIMSK,OCIE2);
	CLEAR_BIT(TIMSK,TOIE2);
}

/*
 * Description: Function to set the Call Back function address.
 */
void Timer2_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr_T2 = a_ptr;
}

/*
 * Description: Adjust PWM compare value if input is percentage
 */
void Timer2_pwmAdjust(uint8 duty_cycle)
{
	duty_cycle  = (uint8)(((uint16)255*duty_cycle)/100); //define compare value
	Timer2_ConfigType ConfigPTR={0,duty_cycle,PS_8_T2,FAST_PWM_T2,CLEAR_T2}; //initialize timer as desired
	Timer2_init(&ConfigPTR);
}
#endif
