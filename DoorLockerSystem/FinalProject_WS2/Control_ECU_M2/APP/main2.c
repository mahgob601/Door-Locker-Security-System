#include "../HAL/BUZZER.h"
#include "../HAL/DCMOTOR.h"
#include "../HAL/EXTERNAL_EEPROM.h"
#include "../MCAL/gpio.h"
#include "../MCAL/pwm.h"
#include "../MCAL/timer1.h"
#include "../MCAL/twi.h"
#include "../MCAL/uart.h"
#include "../HAL/LCD.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define PASS_LENGTH 5
#define MC2_READY 0x10
#define CORRECT 1
#define INCORRECT 2
#define PASSWORD_ADDRESS 0x000E

/*
 * Uart config structure
 * Data bits is 8
 * Parity check is disabled
 * ONE_BIT_MODE stop bit
 * bud rate of 9600
 */

UART_ConfigType U_MC2 = {EIGHT_BIT_DATA_MODE,DISABLED_PARITY_MODE,ONE_BIT_MODE, 9600};
TWI_ConfigType twiConfig={0b00000010,400000};


uint8 g_receivedPass[PASS_LENGTH];
static uint8 timerElapse = 0;

/*
void checkNewPass(void);
void motorOFF(void);
void checkOldPass(void);
void unlockDoor(void);
 */
void controlDoor(void);
void checkNewPass(void)
{


	uint8 i;
	uint8 val;
	uint8 check = CORRECT;
	/* Receiving original password*/
	for(i = 0; i < PASS_LENGTH; i++)
	{
		g_receivedPass[i] = UART_receiveByte();
		LCD_integerToString(g_receivedPass[i]);

	}
	/* Receiving original password*/
	for(i = 0; i < PASS_LENGTH; i++)
	{
		if(g_receivedPass[i] != UART_receiveByte())
		{
			check = INCORRECT;

		}

	}
	UART_sendByte(check);

	/*Write password in the eeprom in case of correct*/
	if(check == CORRECT)
	{
		for(i=0;i<PASS_LENGTH;i++)
		{

			_delay_ms(10);

			EEPROM_writeByte(PASSWORD_ADDRESS+i,g_receivedPass[i]);
		}

		LCD_clearScreen();
		for(i=0;i<PASS_LENGTH;i++)
		{


			_delay_ms(10);

			EEPROM_readByte(PASSWORD_ADDRESS+i,&val);
			LCD_integerToString(val);
		}
	}


}

void checkOldPass(void)
{

	uint8 i;
	uint8 receivedByte;
	uint8 ack = 'i';
	uint8 status = CORRECT;
	while(ack != 'c')
	{
		for(i = 0; i < PASS_LENGTH; i++)
		{
			EEPROM_readByte(PASSWORD_ADDRESS+i,&receivedByte);
			if(UART_receiveByte() != receivedByte)
			{
				status = INCORRECT;
			}
			_delay_ms(10);

		}
		UART_sendByte(status);
		ack = UART_receiveByte();
	}

}

void setTimer(void)
{
	Timer1_setCallBack(controlDoor);
	Timer1_ConfigType t1Config={0,0,PS_64_T1,NORM_T1,NONE,TOGGLE_T1};
	Timer1_init(&t1Config);
}
void controlDoor()
{
	timerElapse++;
	if(timerElapse == 30)
	{
		DcMotor_Rotate(CW,100);
		UART_sendByte('U');

	}
	else if(timerElapse == 36)
	{
		DcMotor_Rotate(OFF,0);

	}
	else if(timerElapse == 66)
	{
		DcMotor_Rotate(ACW,100);
		UART_sendByte('L');
	}
	else
	{
		Timer1_deInit();

		DcMotor_Rotate(OFF,0);
		timerElapse = 0;
		UART_sendByte('D');
	}

}
int main(void)
{

	uint8 key;
	UART_init(&U_MC2);
	DcMotor_Init();
	TWI_init(&twiConfig);

	LCD_init();
	LCD_displayString("LOL");
	SREG |= (1<<7);

	while(1)
	{
		key = UART_receiveByte();
		if(key == 'N')
		{
			checkNewPass();
		}
		else if(key == '+')
		{
			checkOldPass();
			LCD_clearScreen();
			LCD_displayString("working on it");
			setTimer();
			controlDoor();

			LCD_clearScreen();
			//LCD_displayString("hafta7 el bab");
		}
	}

}

