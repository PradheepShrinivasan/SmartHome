#ifndef ININT_C
  #define INIT_C
  
#include "init.h"

void initAll(void)
{	
	portInit();
		// resetting the chip
	DDRB |= (1<<3);
	
	cbi(PORTB,3);
	_delay_ms(1);

	cbi(PORTD,VREG);//Vreg OFF New board
	_delay_ms(1);
	
	cbi(PORTB,3);
	_delay_ms(1);
	
	sbi(PORTD,VREG);//Vreg ON New board
	_delay_ms(1);

	sbi(PORTB,3);
	_delay_ms(1);
	//end of hardware reset of the chip

/// baud rate 2400, op frequency 8000000Hz
	uartInit();
	uartSetBaudRate(UART_BAUD_RATE);
	rprintfInit(uartSendByte);

		cc2420Init(&rfRxInfo,CHANNEL, PANID, NODEID);

}

void portInit(void)
{
	DDRA=DDRB=DDRC=DDRD=0;		// all inputs initially
	//sbi(DDRC,PC5);				// LDR current source enable
	sbi(DDRB,PB0);				// debug LED

	sbi(DDRB,PB3);				// reset pin of cc2420	
	sbi(DDRD,PD2);				// Vreg pin of cc2420

	// disable the cc2420 while not in use 	
	DDRD |= ((1<<VREG)|(1<<3) |(1<<4) |(1<<5) |(1<<6) | (1<<7));
	PORTD |= ((1<<VREG)|(1<<3) |(1<<4) |(1<<5) |(1<<6) );
	PORTD &=  ~(1<<VREG); //Vreg OFF
	DDRC &= ((1<<0) | (1<<1));		
	PORTC |= ((1<<0) | (1<<1));		//Pull up enabled for PortC 0 1
}

#endif
