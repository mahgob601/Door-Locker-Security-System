#include "LCD.h"
#include "../MCAL/gpio.h"
#include "avr/io.h"
#include "util/delay.h"
#include "../common_macros.h"
/*
 * The file is intended to control LCD
 */
void LCD_init(void)
{
	/* setup the RS as output*/
	GPIO_setupPinDirection(LCD_CONFIGS_PORT, LCD_RS, PIN_OUTPUT);
	/* setup the Enable pin as output*/
	GPIO_setupPinDirection(LCD_CONFIGS_PORT, LCD_E, PIN_OUTPUT);
	_delay_ms(20);
#if(LCD_DATA_BITS_MODE == 8)
	GPIO_setupPortDirection(LCD_DATA_PATH, PORT_OUTPUT);
	LCD_sendCommand(LCD_TWO_LINES_EIGHT_BITS_MODE_COMMAND);
#elif (LCD_DATA_BITS_MODE == 4)
	GPIO_setupPinDirection(LCD_DATA_PATH, LCD_DATA_START_BIT, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PATH, LCD_DATA_START_BIT+1, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PATH, LCD_DATA_START_BIT+2, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PATH, LCD_DATA_START_BIT+3, PIN_OUTPUT);
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT1);
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT2);

	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_COMMAND);
#endif

	LCD_sendCommand(LCD_CURSOR_OFF);
	LCD_sendCommand(LCD_CLEAR_DISPLAY);
}

void LCD_sendCommand(uint8 command)
{
	GPIO_writePin(LCD_CONFIGS_PORT, LCD_RS, LOGIC_LOW);
	_delay_ms(1);
	GPIO_writePin(LCD_CONFIGS_PORT, LCD_E, LOGIC_HIGH);
	_delay_ms(1);

#if(LCD_DATA_BITS_MODE == 8)
	GPIO_writePort(LCD_DATA_PATH,command);
	_delay_ms(1);
	GPIO_writePin(LCD_CONFIGS_PORT, LCD_E, LOGIC_LOW);
	_delay_ms(1);

#elif (LCD_DATA_BITS_MODE == 4)
	GPIO_writePin(LCD_DATA_PATH,LCD_DATA_START_BIT,GET_BIT(command, 4));
	GPIO_writePin(LCD_DATA_PATH,LCD_DATA_START_BIT+1,GET_BIT(command, 5));
	GPIO_writePin(LCD_DATA_PATH,LCD_DATA_START_BIT+2,GET_BIT(command, 6));
	GPIO_writePin(LCD_DATA_PATH,LCD_DATA_START_BIT+3,GET_BIT(command, 7));
	_delay_ms(1);
	GPIO_writePin(LCD_CONFIGS_PORT, LCD_E, LOGIC_LOW);
	_delay_ms(1);

	GPIO_writePin(LCD_CONFIGS_PORT, LCD_E, LOGIC_HIGH);
	_delay_ms(1);

	GPIO_writePin(LCD_DATA_PATH,LCD_DATA_START_BIT,GET_BIT(command, 0));
	GPIO_writePin(LCD_DATA_PATH,LCD_DATA_START_BIT+1,GET_BIT(command, 1));
	GPIO_writePin(LCD_DATA_PATH,LCD_DATA_START_BIT+2,GET_BIT(command, 2));
	GPIO_writePin(LCD_DATA_PATH,LCD_DATA_START_BIT+3,GET_BIT(command, 3));
	_delay_ms(1);
	GPIO_writePin(LCD_CONFIGS_PORT, LCD_E, LOGIC_LOW);
	_delay_ms(1);
#endif

}

void LCD_displayCharacter(uint8 character)
{
	GPIO_writePin(LCD_CONFIGS_PORT, LCD_RS, LOGIC_HIGH);
	_delay_ms(1);
	GPIO_writePin(LCD_CONFIGS_PORT, LCD_E, LOGIC_HIGH);
	_delay_ms(1);

#if(LCD_DATA_BITS_MODE == 8)
	GPIO_writePort(LCD_DATA_PATH,character);
	_delay_ms(1);
	GPIO_writePin(LCD_CONFIGS_PORT, LCD_E, LOGIC_LOW);
	_delay_ms(1);

#elif (LCD_DATA_BITS_MODE == 4)
	GPIO_writePin(LCD_DATA_PATH,LCD_DATA_START_BIT,GET_BIT(character, 4));
	GPIO_writePin(LCD_DATA_PATH,LCD_DATA_START_BIT+1,GET_BIT(character, 5));
	GPIO_writePin(LCD_DATA_PATH,LCD_DATA_START_BIT+2,GET_BIT(character, 6));
	GPIO_writePin(LCD_DATA_PATH,LCD_DATA_START_BIT+3,GET_BIT(character, 7));
	_delay_ms(1);
	GPIO_writePin(LCD_CONFIGS_PORT, LCD_E, LOGIC_LOW);
	_delay_ms(1);

	GPIO_writePin(LCD_CONFIGS_PORT, LCD_E, LOGIC_HIGH);
	_delay_ms(1);

	GPIO_writePin(LCD_DATA_PATH,LCD_DATA_START_BIT,GET_BIT(character, 0));
	GPIO_writePin(LCD_DATA_PATH,LCD_DATA_START_BIT+1,GET_BIT(character, 1));
	GPIO_writePin(LCD_DATA_PATH,LCD_DATA_START_BIT+2,GET_BIT(character, 2));
	GPIO_writePin(LCD_DATA_PATH,LCD_DATA_START_BIT+3,GET_BIT(character, 3));
	_delay_ms(1);
	GPIO_writePin(LCD_CONFIGS_PORT, LCD_E, LOGIC_LOW);
	_delay_ms(1);
#endif
}

void LCD_displayString(const char *str)
{
	uint8 i = 0;
	while(str[i] != '\0')
	{
		LCD_displayCharacter(str[i]);
		i++;
	}
}

void LCD_moveCursor(uint8 row,uint8 col)
{
	uint8 lcd_memory_address;

	/* Calculate the required address in the LCD DDRAM */
	switch(row)
	{
	case 0:
		lcd_memory_address=col;
		break;
	case 1:
		lcd_memory_address=col+0x40;
		break;
	case 2:
		lcd_memory_address=col+0x10;
		break;
	case 3:
		lcd_memory_address=col+0x50;
		break;
	}
	/* Move the LCD cursor to this specific address */
	LCD_sendCommand(lcd_memory_address | LCD_SET_CURSOR_LOCATION);
}

void LCD_displayStringRowColumn(uint8 row, uint8 column, const char *str)
{
	LCD_moveCursor(row,column);
	LCD_displayString(str);

}

void LCD_integerToString(uint8 data)
{
	char buff[26]; /* String to hold the ascii result */
	utoa(data,buff,10); /* Use itoa C function to convert the data to its corresponding ASCII value, 10 for decimal */
	LCD_displayString(buff); /* Display the string */
}

void LCD_clearScreen(void)
{
	LCD_sendCommand(LCD_CLEAR_DISPLAY);

}

