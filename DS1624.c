#ifndef _DS1624_C
#define _DS1624_C

#define Addr1624 0x90

#include "DS1624.h"
/*
uC DS1624 DATA(MSB FIRST) COMMENTS

TX  RX      START         Bus Master Initiates a Start condition.
TX  RX      Addr          Bus Master sends DS1624 address; R/W =”0”;
RX  TX      ACK           DS1624 generates acknowledge bit.
TX  RX      ACh           Bus Master sends Access Config command protocol.
RX  TX      ACK           DS1624 generates acknowledge bit. 1
TX  RX      00h           Bus Master sets up DS1624 for continuous conversion.
RX  TX      ACK           DS1624 generates acknowledge bit. 2, 4
TX  RX      STOP          Bus Master initiates the STOP condition.
*/
void ds1624_config(void)
{
	u08 data[2];
	data[0]=0xAC;data[1]=0x00;
	i2cMasterSendNI(Addr1624, 2, data);
}

/*
uC DS1624 DATA(MSB FIRST) COMMENTS
TX  RX      START         Bus Master initiates a Start condition.
TX  RX      Addr          Bus Master sends DS1624 address;R/W =0;
RX  TX      ACK           DS1624 generates acknowledge bit.
TX  RX      EEh           Bus Master sends Start Convert Tcommand protocol.
RX  TX      ACK           DS1624 generates acknowledge bit. 1
TX  RX      STOP          Bus Master initiates the STOP condition.
*/
void ds1624_startconvert(void)
{
	u08 data[1];
	data[0]=0xEE;
	i2cMasterSendNI(Addr1624, 1, data);
}

// used to format and print temperature value
u16 ds1624_readtemp(void)
{
	u08 ret[2];
	s16 T;
	readtemp(Addr1624, 1, 2, ret);
	T = (s16)((ret[0]<<8) | ret[1]);
	return T;

   /*rprintf("Temp is: ");
   rprintfNum(10, 4, FALSE , ' ', T>>8);
   rprintf(".");
   rprintfNum(10, 4, FALSE, '0', (10000*((u32)(T&0x00FF)))/256 );*/
}

// to check if DS1624 is acknowledging or not
u08 ds1624_check(void)
{
	u08 data=0;
	return(!i2cMasterSendNI(Addr1624, 1, &data));
}

/*
uC DS1624 DATA(MSB FIRST) COMMENTS
TX  RX       START        Bus Master initiates a Start condition.
TX  RX       Addr         Bus Master sends DS1624 address;R/W =0;
RX  TX       ACK          DS1624 generates acknowledge bit.
TX  RX       AAh          Bus Master sends Read Temp commandprotocol.
RX  TX       ACK          DS1624 generates acknowledge bit. 1
TX  RX       START        Bus Master initiates a Repeated Startcondition.
TX  RX       Addr         Bus Master sends DS1624 address;R/W =1;
RX  TX       ACK          DS1624 generates acknowledge bit.
RX  TX       <data>       DS1624 sends the MSB byte ofTemperature.
TX  RX       ACK          Bus Master generates acknowledge bit.
RX  TX       <data>       DS1624 sends the LSB byte ofTemperature.
TX  RX       NACK         Bus Master sends “NO ACKNOWLEDGE” bit.
TX  RX       STOP         Bus Master initiates the STOP condition.
*/
void readtemp(u08 deviceAddr, u08 sendlength,u08 receivelength, u08* receivedata)
{
	// disable TWI interrupt
	cbi(TWCR, TWIE);

	// send start condition
	i2cSendStart();
	i2cWaitForComplete();

	// if there's data to be sent, do it

	// send device address with write
	i2cSendByte( Addr1624 & 0xFE );
	i2cWaitForComplete();

	i2cSendByte(0xAA);
	i2cWaitForComplete();	



	// if there's data to be received, do it
	if(receivelength)
	{
		// send repeated start condition
		i2cSendStart();
		i2cWaitForComplete();

		// send device address with read
		i2cSendByte( Addr1624 | 0x01 );
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
		//rprintfu08(*receivedata);
	}
	
	// transmit stop condition
	// leave with TWEA on for slave receiving
	i2cSendStop();
	while( !(inb(TWCR) & BV(TWSTO)) );

	// enable TWI interrupt
	sbi(TWCR, TWIE);
}

#endif
