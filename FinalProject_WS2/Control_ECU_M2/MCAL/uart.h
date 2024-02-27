/*
 * uart.h
 *
 *  Created on: Nov 2, 2023
 *      Author: Yassin
 *      Description: header file for UART Module driver
 */

#ifndef MCAL_UART_H_
#define MCAL_UART_H_

#include "../std_types.h"
#include "../common_macros.h"


/*
 * Bit-fields for the register UCSRC for easier access
 * UCPOL_Val = UCPOL
 * UCSZ1To0_Val = UCSZ1:0
 * USBS_Val = USBS
 * UPM1To0_Val = UPM1:0
 * UMSEL_Val  = UMSEL
 * URSEL_Val = URSEL
 */
typedef union{
	uint8 Byte;
	struct{
		uint8 UCPOL_Val:1;
		uint8 UCSZ1To0_Val:2;
		uint8 USBS_Val: 1;
		uint8 UPM1To0_Val: 2;
		uint8 UMSEL_Val: 1;
		uint8 URSEL_Val: 1;

	}Bits;

}UCSRC_R_TYPE;

/********************************************************************************
 * definitions
 ********************************************************************************/
#define UCSRC_R (*(UCSRC_R_TYPE* volatile const) 0x40)


/********************************************************************************
 * Type definitions for config structure
 ********************************************************************************/

/*
 * Description:
 * Data bits may be 5,6,7,8 or 9-bits depending on the set value
 */
typedef enum{
	FIVE_BIT_DATA_MODE, SIX_BIT_DATA_MODE, SEVEN_BIT_DATA_MODE, EIGHT_BIT_DATA_MODE, NINE_BIT_DATA_MODE
}UART_BitData ;

/*
 * Description:
 * We can either allow parity checking or not
 * DISABLED means no parity check
 * EVEN_PARITY_MODE allows even parity check
 * ODD_PARITY_MODE allows odd parity check
 */
typedef enum{
	DISABLED_PARITY_MODE,EVEN_PARITY_MODE,ODD_PARITY_MODE
}UART_Parity ;


/*
 * Description:
 * Stop condition at end of UART communication
 * can be either 1 or 2 bits to indicate the end
 */
typedef enum{
	ONE_BIT_MODE = 0, TWO_BIT_MODE
}UART_StopBit ;

typedef uint32 UART_BaudRate;

/*
 * Description:
 * Configuration structure typedef contains:
 * bit_data : no. of data bit
 * parity: allow for checking (even or odd) or not
 * stop_bit: 1 or 2 stop bits
 * baud_rate: baud_rate value (most commonly used is 9600)
 */

typedef struct{
UART_BitData bit_data;
UART_Parity parity;
UART_StopBit stop_bit;
UART_BaudRate baud_rate;
}UART_ConfigType;

/***************************
 * Functions  Prototypes
 ***************************/
void UART_init(const UART_ConfigType * Config_Ptr);
void UART_sendByte(const uint8 data);
uint8 UART_receiveByte(void);
void UART_sendString(const uint8 *Str);
void UART_receiveString(uint8 *Str);


#endif /* MCAL_UART_H_ */
