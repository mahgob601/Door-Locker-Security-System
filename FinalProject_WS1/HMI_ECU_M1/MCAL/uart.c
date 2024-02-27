/*
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the UART AVR driver
 */

#include "uart.h"
#include <avr/io.h>


/*
 * Description:
 * function initializes UART
 * Configures frame format
 * enables UART
 * Sets up baud rate
 */
void UART_init(const UART_ConfigType * Config_Ptr)
{
	uint16 ubrr_value = 0;

	/* Double transmission speed for asynchronous mode */
	UCSRA = (1<<U2X);


	/*Enable transmit and receive for the UART*/
	UCSRB = (1<<RXEN) | (1<<TXEN);

	/*URSEL bit is set to 1 to specify that we are working on UCRSC to UBRRH*/
	UCSRC_R.Bits.URSEL_Val = 1;

	switch(Config_Ptr->bit_data)
	{
	case FIVE_BIT_DATA_MODE:
		UCSRC_R.Bits.UCSZ1To0_Val = 0;
		UCSRB &= ~(1<<UCSZ2);
		break;
	case SIX_BIT_DATA_MODE:
		UCSRC_R.Bits.UCSZ1To0_Val = 1;
		UCSRB &= ~(1<<UCSZ2);
		break;
	case SEVEN_BIT_DATA_MODE:
		UCSRC_R.Bits.UCSZ1To0_Val = 2;
		UCSRB &= ~(1<<UCSZ2);
		break;
	case EIGHT_BIT_DATA_MODE:
		UCSRC_R.Bits.UCSZ1To0_Val = 3;
		UCSRB &= ~(1<<UCSZ2);
		break;
	case NINE_BIT_DATA_MODE:
		UCSRC_R.Bits.UCSZ1To0_Val = 3;
		UCSRB = UCSRB | (1<<UCSZ2);
		break;
	}

	switch(Config_Ptr->parity)
	{
	case DISABLED_PARITY_MODE:
		UCSRC_R.Bits.UPM1To0_Val = 0;
		break;
	case EVEN_PARITY_MODE:
		UCSRC_R.Bits.UPM1To0_Val = 2;
		break;
	case ODD_PARITY_MODE:
		UCSRC_R.Bits.UPM1To0_Val = 3;
		break;
	}

	switch(Config_Ptr->stop_bit)
	{
	case ONE_BIT_MODE:
		UCSRC_R.Bits.USBS_Val = 0;
		break;
	case TWO_BIT_MODE:
		UCSRC_R.Bits.USBS_Val = 1;
		break;


	}
	UCSRC_R.Bits.UCPOL_Val = 0;
	UCSRC_R.Bits.UMSEL_Val = 0;







	/*
	 * URSEL = 1
	 * UCSZ1:0 = 11 for enabling 8-bit mode
	 */
	ubrr_value = (uint16)(((F_CPU/((Config_Ptr->baud_rate) * 8UL)))-1);

	UBRRH = ubrr_value>>8;
	UBRRL = ubrr_value;
}


/*
 * Description:
 * Send byte to another UART device
 */
void UART_sendByte(const uint8 data)
{
	/*
	 * wait for the data buffer to be empty
	 * so we can send a byte of data
	 */
	while(BIT_IS_CLEAR(UCSRA,UDRE));
	UDR = data;
}

/*
 * Description:
 * Receive byte from another UART device
 */
uint8 UART_receiveByte(void)
{
	while(BIT_IS_CLEAR(UCSRA,RXC));
	return UDR;

}

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str)
{
	uint8 index = 0;
	while(Str[index] != '\0')
	{
		UART_sendByte(Str[index]);
		index++;
	}

}

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;

	Str[i] = UART_receiveByte();

	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_receiveByte();
	}

	Str[i] = '\0';
}


