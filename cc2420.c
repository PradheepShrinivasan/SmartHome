#ifndef CC2420_C
#define CC2420_C


#include "cc2420.h"
void cc2420WriteReg(u08 addr, u16 data)
{
		// assert chip select
	cbi(PORTB, CC2420_PIN_CS);
		// send command
	spiTransferByte(addr);
		// write data
	spiTransferByte(data>>8);
	spiTransferByte(data);
		// release chip select
	sbi(PORTB, CC2420_PIN_CS);
}

void cc2420WriteRam(u16 addr, u08* data, u16 nBytes)
{
		// assert chip select
	cbi(PORTB, CC2420_PIN_CS);
	// send command
	spiTransferByte(0x80|addr);
	spiTransferByte((addr>>1) & 0xC0);
	while(nBytes--)
		spiTransferByte(*data++);
		// release chip select
	sbi(PORTB, CC2420_PIN_CS);
}

void cc2420WriteFifo(u08* data, u16 nBytes)
{
		// assert chip select
	cbi(PORTB, CC2420_PIN_CS);
		// send command
	spiTransferByte(CC2420_TXFIFO);
	while(nBytes--)
		spiTransferByte(*data++);
		// release chip select
	sbi(PORTB, CC2420_PIN_CS);
}

void cc2420SetChannel(u08 channel)
{
	u16 f;
	
	// Derive frequency programming from the given channel number
	f = (u16)(channel-11);	// Subtract the base channel 
	f = f + (f << 2);    	// Multiply with 5, which is the channel spacing
	f = f + 357 + 0x4000;	// 357 is 2405-2048, 0x4000 is LOCK_THR = 1

    // Write it to the CC2420
	cc2420WriteReg(CC2420_FSCTRL, f);
}


void cc2420Command(u08 cmd)
{
		// assert chip select
	cbi(PORTB, CC2420_PIN_CS);
		// send command
	spiTransferByte(cmd);
		// release chip select
	sbi(PORTB, CC2420_PIN_CS);
}


u08 cc2420GetStatus(void)
{
		// assert chip select
	cbi(PORTB, CC2420_PIN_CS);
		// get status
	spiTransferByte(CC2420_SNOP);
		// release chip select
	sbi(PORTB, CC2420_PIN_CS);
		// return the received data
	return inb(SPDR);
}

void cc2420ReadFifo(u08* data, u16 nBytes)
{
	// assert chip select
	cbi(PORTB, CC2420_PIN_CS);
	// send command
	spiTransferByte(CC2420_RXFIFO | 0x40);
	while(nBytes--)
		*data++ = spiTransferByte(0x00);
	// release chip select
	sbi(PORTB, CC2420_PIN_CS);
}

void cc2420DumpFifo(u16 nBytes)
{
	// assert chip select
	cbi(PORTB, CC2420_PIN_CS);
	// send command
	spiTransferByte(CC2420_RXFIFO | 0x40);
	while(nBytes--)
		spiTransferByte(0x00);
	// release chip select
	sbi(PORTB, CC2420_PIN_CS);
}

#endif
