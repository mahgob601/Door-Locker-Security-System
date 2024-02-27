
#include "../HAL/LCD.h"
#include "../HAL/Keypad.h"
#include "../MCAL/uart.h"
#include "../MCAL/timer1.h"
#include <util/delay.h> /* For the delay functions */

#define PASS_LENGTH 5
#define MC2_READY 0x10
#define CORRECT 1
#define INCORRECT 2

/*
 * Uart config structure
 * Data bits is 8
 * Parity check is disabled
 * ONE_BIT_MODE stop bit
 * bud rate of 9600
 */
UART_ConfigType U_MC1 = {EIGHT_BIT_DATA_MODE,DISABLED_PARITY_MODE,ONE_BIT_MODE, 9600};

uint8 key;
uint8 g_pass[PASS_LENGTH+1];
uint8 g_confirmPass[PASS_LENGTH];
uint8 matching = 0;
volatile uint8 pass_comp_flag = 0;

uint8 createPassword(void);
void askForPassword(void);
int main(void)
{

	uint8 doorStatus;

	//LCD_init();
	LCD_init();
	UART_init(&U_MC1);

	UART_sendByte('N');
	while(!createPassword())
	{
		UART_sendByte('N');
	}


	while(1)
	{
		LCD_clearScreen();
		LCD_displayString("+ : Open Door");
		LCD_moveCursor(1,0);
		LCD_displayString("- : Change Pass");
		key = KEYPAD_getPressedKey();
		_delay_ms(500);
		if(key == '+')
		{
			UART_sendByte('+');
			askForPassword();
			doorStatus = UART_receiveByte();

			if(doorStatus == 'U')
			{
				LCD_clearScreen();
				LCD_displayString("Door is Unlocking");
				LCD_moveCursor(0,0);
			}
			while(UART_receiveByte() != 'L');
			LCD_clearScreen();
			LCD_displayString("Door is Locking");
			LCD_moveCursor(0,0);
			while(UART_receiveByte() != 'D');

			/*doorStatus = UART_receiveByte();
			if(doorStatus == 'U')
			{
				LCD_clearScreen();
				LCD_displayString("Door is Unlocking");
			}

			doorStatus = UART_receiveByte();
			if(doorStatus == 'L')
			{
				LCD_clearScreen();
				LCD_displayString("Door is locking");
			}*/

		}
		else if(key == '-')
		{

		}


	}




}

void askForPassword(void)
{
	uint8 i;
	uint8 getStatus = INCORRECT;
	LCD_clearScreen();
	LCD_displayString("plz enter pass: ");
	LCD_moveCursor(1,0);

	while(getStatus != CORRECT)
	{
		for(i = 0; i<PASS_LENGTH;i++)
		{
			key = KEYPAD_getPressedKey();
			if(key>=0 && key <=9)
			{
				LCD_integerToString(key);
			}
			else{
				LCD_displayCharacter(key);
			}
			_delay_ms(200);
			LCD_moveCursor(1,i);
			UART_sendByte(key);
			LCD_displayCharacter('*');
			_delay_ms(500);
		}
		key = KEYPAD_getPressedKey();
		_delay_ms(500);

		if(key == '=')
		{
			getStatus = UART_receiveByte();

			if(getStatus == INCORRECT)
			{
				UART_sendByte('i');
			}
			else
			{
				UART_sendByte('c');
				LCD_clearScreen();
				LCD_displayString("YOU LOGGED IN CORRECT PASSWORD !!!");
			}


		}


	}





}

uint8 createPassword(void)
{
	uint8 check;
	uint8 i = 0;
	LCD_clearScreen();
	LCD_displayString("plz enter pass: ");
	LCD_moveCursor(1,0);


	for(i = 0; i<PASS_LENGTH;i++)
	{
		key = KEYPAD_getPressedKey();
		if(key>=0 && key <=9)
		{
			LCD_integerToString(key);
		}
		else{
			LCD_displayCharacter(key);
		}
		g_pass[i] = key;
		_delay_ms(200);
		LCD_moveCursor(1,i);
		UART_sendByte(key);
		LCD_displayCharacter('*');
		_delay_ms(500);
	}
	key = KEYPAD_getPressedKey();
	_delay_ms(500);

	if(key == '=')
	{
		LCD_clearScreen();
		g_pass[PASS_LENGTH] = '#';
		LCD_displayString("plz re-enter the");
		LCD_moveCursor(1,0);
		LCD_displayString("same pass: ");
		for(i = 0; i<PASS_LENGTH;i++)
		{
			key = KEYPAD_getPressedKey();
			if(key>=0 && key <=9)
			{
				LCD_integerToString(key);
			}
			else{
				LCD_displayCharacter(key);
			}
			g_confirmPass[i] = key;
			_delay_ms(200);
			LCD_moveCursor(1,i+11);
			UART_sendByte(key);
			LCD_displayCharacter('*');
			_delay_ms(500);
		}
		key = KEYPAD_getPressedKey();
		_delay_ms(500);



		if(UART_receiveByte() == CORRECT)
		{
			LCD_clearScreen();
			LCD_displayString("Correct!");
			_delay_ms(300);


		}
		else{
			LCD_clearScreen();
			LCD_displayString("Not Matching");
			_delay_ms(300);
			return 0;

		}
	}
	return 1;

}
