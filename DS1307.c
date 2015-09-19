#ifndef _DS1307_C
  #define _DS1307_C

#define DS1307_ID 0xD0	

//#define SEC   0x00
//#define	MINU  0x01	
//#define HOUR    0x02
//#define DAT   0x04
//#define MONTH 0x05
//#define YEAR  0x06

#include "DS1307.h"


void initUart(void)
{
	uartInit();
	 // make all rprintf statements use uart for output
	rprintfInit(uartSendByte);
	 // initialize the timer system
	uartSetBaudRate(2400);
}

//to initialize the i2c subroutine
void initI2c(void)
{
	i2cInit();
	i2cSetBitrate(100);
}


 //to check the working of ds1307 returns 1 if success else 0
u08 ds1307_check(void)
{
	u08 data=0;
	return(!i2cMasterSendNI(DS1307_ID, 1, &data));
}


 //TO GET THE DATA FROM THE DS1307
unsigned char DS1307_get(unsigned char addr)
{
	u08 ret=0;
	i2cMasterTransferNI(DS1307_ID, 1, &addr, 1, &ret);
	return ret;
}



//TO SET THE TIME 
void DS1307_settime(unsigned char hh, unsigned char mm, unsigned char ss)
{
	u08 data[4];
	data[0]=0;data[1]=ss;data[2]=mm;data[3]=hh;
	i2cMasterSendNI(DS1307_ID, 4, data);
}

//  TO SET THE DATE 
void DS1307_setdate(unsigned char dd, unsigned char mm, unsigned char yy)
{
	u08 data[4];
	data[0]=4;data[1]=dd;data[2]=mm;data[3]=yy;
	i2cMasterSendNI(DS1307_ID, 4, data);
}


// USED TO GET VALUES FROM I2C
void i2cMasterTransferNI(u08 deviceAddr, u08 sendlength, u08* senddata, u08 receivelength, u08* receivedata)
{
	// disable TWI interrupt
	cbi(TWCR, TWIE);

	// send start condition
	i2cSendStart();
	i2cWaitForComplete();

	// if there's data to be sent, do it
	if(sendlength)
	{
		// send device address with write
		i2cSendByte( deviceAddr & 0xFE );
		i2cWaitForComplete();
		
		// send data
		while(sendlength)
		{
			i2cSendByte( *senddata++ );
			i2cWaitForComplete();
			sendlength--;
		}
	}

	// if there's data to be received, do it
	if(receivelength)
	{
		// send repeated start condition
		i2cSendStart();
		i2cWaitForComplete();

		// send device address with read
		i2cSendByte( deviceAddr | 0x01 );
		i2cWaitForComplete();

		// accept receive data and ack it
		while(receivelength > 1)
		{
			i2cReceiveByte(TRUE);
			i2cWaitForComplete();
			*receivedata++ = i2cGetReceivedByte();
			// decrement length
			receivelength--;
		}

		// accept receive data and nack it (last-byte signal)
		i2cReceiveByte(FALSE);
		i2cWaitForComplete();
		*receivedata++ = i2cGetReceivedByte();
	}
	
	// transmit stop condition
	// leave with TWEA on for slave receiving
	i2cSendStop();
	while( !(inb(TWCR) & BV(TWSTO)) );

	// enable TWI interrupt
	sbi(TWCR, TWIE);
}

#endif
