/*
 * LCD.h
 *
 *  Created on: Oct 4, 2023
 *      Author: Yassin
 */

#ifndef SRC_LCD_H_
#define SRC_LCD_H_
#include "../std_types.h"

/****************************
 * Defines of connections
 ************************/
#define LCD_RS PIN0_ID
#define LCD_E PIN1_ID
#define LCD_DATA_PATH PORTA_ID
#define LCD_CONFIGS_PORT PORTA_ID

// LCD modes and command
#define LCD_DATA_BITS_MODE 4
#define LCD_TWO_LINES_EIGHT_BITS_MODE_COMMAND 0x38
#define LCD_TWO_LINES_FOUR_BITS_MODE_COMMAND 0x28
#define LCD_BLINK_CURSOR 0x0F
#define LCD_CURSOR_OFF 0x0C
#define LCD_CLEAR_DISPLAY 0x01
#define LCD_SET_CURSOR_LOCATION 0x80

#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT1   0x33
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT2   0x32


#if (LCD_DATA_BITS_MODE != 4) && (LCD_DATA_BITS_MODE != 8)

#error "Number of Data bits should be equal to 4 or 8"

#endif

#if (LCD_DATA_BITS_MODE == 4)
#define LCD_DATA_START_BIT PIN2_ID

#endif

/*
 * initalizing the lcd
 */
void LCD_init(void);
void LCD_displayCharacter(uint8 character);
void LCD_sendCommand(uint8 command);
void LCD_clearScreen(void);
void LCD_displayString(const char *str);
void LCD_moveCursor(uint8 row,uint8 col);
void LCD_integerToString(uint8 data);
void LCD_displayStringRowColumn(uint8 row, uint8 column, const char *str);
#endif /* SRC_LCD_H_ */
