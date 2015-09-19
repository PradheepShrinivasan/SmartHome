/* core definitions */
#define F_CPU 1000000		// 8MHz processor
#define ZIGBEE	//UART	// change the physical interface
							// data transfer
#define UART

#define SOUND_U_THRESHOLD	5
#define SOUND_L_THRESHOLD 	5

#define TEMP_U_THRESHOLD 	10
#define TEMP_L_THRESHOLD 	10

#define LIGHT_U_THRESHOLD 	6
#define LIGHT_L_THRESHOLD 	6

#define VALID 				 1
#define INVALID 			 0 


/* Hardware Pins */
#define LDR_EN	cbi(PORTC,PC5)
#define LDR_DIS	sbi(PORTC,PC5)

#define LED_ON	sbi(PORTC,PC2)
#define LED_OFF	cbi(PORTC,PC2)

/* ADC Channels */
#define PIEZO	ADC_CH_ADC1//ADC_CH_1_0_DIFF10X //ADC_CH_1_0_DIFF200X 
#define LM335	ADC_CH_ADC3//ADC_CH_3_2_DIFF10X
#define LDR 	ADC_CH_ADC4

/* basic inclusions */
#include <avr/io.h>
#include "global.h"

#include "a2d.h"
#include "a2d.c"


/* constrained inclusions */
#ifdef UART
	#define	 UART_BAUD_RATE	2400

	#include "uart.h"
	#include "uart.c"

	#include "rprintf.h"
	#include "rprintf.c"

	#include "buffer.h"
	#include "buffer.c"
#endif

#ifdef ZIGBEE
	#define NEW

	#include "string.h"

	#include "spi.h"
	#include "spi.c"

	#include "cc2420.h"
	//#include "cc2420.c"

void vdelay(void)
{
	u16 i;
	for(i=0;i<100;i++);
}
#endif

/* Globals */
u08 sensedSound[2];
u08 sensedLight[2];
u08 sensedTemp[2];
u08 updateData;
u08 packetCounter	=	0;


/* damn temp */

#ifdef ZIGBEE

	#define FIFOP_IS_1	(CC2420_CTRL_PORTIN & (1<<CC2420_PIN_FIFOP))
	#define FIFO_IS_1	(CC2420_CTRL_PORTIN & (1<<CC2420_PIN_FIFO))
	#define SFD_IS_1	(CC2420_CTRL_PORTIN & (1<<CC2420_PIN_SFD))

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

void cc2420SendPck(u16 dest_addr,unsigned char lenPck,unsigned char *data)
{
	static unsigned char seq = 0;
    static u16 pan_id = 0xABCD;		//Destiantion PAN ID
    static u16 id=0x1234;			//Source ID
    u08 status,len;

    u08 _count;

    // noack frame control field
    u16 fcf = 0x8841;
   // ack frame control field
   //u16 fcf = 0x8861;

    while(FIFOP_IS_1 || SFD_IS_1);       

    cc2420Command(CC2420_SFLUSHTX);

      // wait for RSSI
   /*do {
      status = cc2420GetStatus();
      delay_us(0x1fff);
   } while(!(status & (1 << CC2420_RSSI_VALID)));*/


      len = lenPck + 11;

   // fill up the fifo with the frame format
   cc2420WriteFifo((u08*)&len, 1 /*sizeof(len)*/);
   cc2420WriteFifo((u08*)&fcf, 2 /*sizeof(fcf)*/);
   cc2420WriteFifo((u08*)&seq, 1 /*sizeof(seq)*/);
   cc2420WriteFifo((u08*)&pan_id, 2 /*sizeof(pan_id)*/);
   cc2420WriteFifo((u08*)&dest_addr, 2 /*sizeof(dest_addr)*/);
   cc2420WriteFifo((u08*)&id, 2 /*sizeof(id)*/);
      cc2420WriteFifo(data,lenPck);
   // wait for transmission

      cc2420Command(CC2420_STXON); 

      do {
      	cc2420Command(CC2420_STXONCCA);
      	status = cc2420GetStatus();
     //delay_us(100);
      	for(_count=0;_count<=100;_count++);
      		uartSendByte('W');
      } while(!(status & (1 << CC2420_TX_ACTIVE)));




	 #ifdef UART
      rprintf("cca \n");
			#endif

      while((SFD_IS_1));
      uartSendByte('S');

    // If ur cc2420 works exclusively as TXmiter... If not
    // comment the following 2 lines.....
      cc2420Command(CC2420_SRFOFF);


      seq++;   

      return 0;
  }



  void cc2420Init(u08 channel, u16 panId, u16 nodeId)
  {
  	spiInit();
  	sbi(DDRB,CC2420_PIN_CS);
  	sbi(PORTB,CC2420_PIN_CS);

  	cbi(CC2420_CTRL_DDR,CC2420_PIN_FIFO);
  	cbi(CC2420_CTRL_DDR,CC2420_PIN_FIFOP);
  	cbi(CC2420_CTRL_DDR,CC2420_PIN_CCA);
  	cbi(CC2420_CTRL_DDR,CC2420_PIN_SFD);
  	sbi(CC2420_CTRL_PORT,CC2420_PIN_FIFO);
  	sbi(CC2420_CTRL_PORT,CC2420_PIN_FIFOP);
  	sbi(CC2420_CTRL_PORT,CC2420_PIN_CCA);
  	sbi(CC2420_CTRL_PORT,CC2420_PIN_SFD);

	// soft-reset chip
  	cc2420WriteReg(CC2420_MAIN,0x0000);
  	cc2420WriteReg(CC2420_MAIN,0xF800);
	// turn on oscillator
  	cc2420Command(CC2420_SXOSCON);

	// prepare registers

	cc2420WriteReg(CC2420_SYNCWORD, 0xA70F);	// Sync word...

	cc2420WriteReg(CC2420_MDMCTRL0,	0x0AF2);	// Turn on automatic packet acknowledg

	cc2420WriteReg(CC2420_MDMCTRL1,	0x0500);	// Set the correlation threshold = 20
	cc2420WriteReg(CC2420_DACTST,0x0000);

	cc2420WriteReg(CC2420_IOCFG0,	0x0040);	// Set the FIFOP threshold to 64

	cc2420WriteReg(CC2420_SECCTRL0,	0x01C4);	// Turn off "Security enable"
	cc2420WriteReg(CC2420_TXCTRL, 0xA0FF);


	// set radio channel
	cc2420SetChannel(channel);

	// wait until oscillator stable
	while(!(cc2420GetStatus() & (1<<CC2420_XOSC16M_STABLE)));

	// set IDs
	cc2420WriteRam(CC2420RAM_SHORTADDR, (u08*)&nodeId, 2);
	cc2420WriteRam(CC2420RAM_PANID, (u08*)&panId, 2);

}

#endif


	
/* Function Defns */
int 	main(void);			//main
void 	initAll(void);		//subsystems
void	portInit(void); 	//all pins
void 	readDataInit(void);	//get all the physical values, initial setting
void 	readData(void);		//get all the physical values
void 	compareData(void);	//compare for limits
void	sendData(void);		//send the data over the interface
void	busyWait(void);		//sw polled delay
char 	convertChar(u08);	//conversion

int main(void)
{
	u08 _count;
	
	initAll();
	//readDataInit();

	#ifdef UART
	rprintf("Hai");
	rprintfCRLF();
	#endif
	
	//rprintf("Hai");	

	while(1)
	{
		//readData();
		//compareData();
		//sendData();
	//	cc2420SendPck(0xBADD, 4, "Vigo");
	//	busyWait();
	//	busyWait();
		
		if((PINC & (1<<0))==0)
		{
			for(_count=0;_count<255;_count++);

				if((PINC & (1<<0))==0)
				{
					while(!(PINC & (1<<0)));
					cc2420SendPck(0xBADD,3,"S1");
				}
			}

			if((PINC & (1<<1))==0)
			{
				for(_count=0;_count<255;_count++);

					if((PINC & (1<<1))==0)
					{
						while(!(PINC & (1<<1)));
						cc2420SendPck(0xBADD,3,"S2");
					}
				}

			}
			while(1);
		}

		void initAll(void)
		{
			a2dInit();
			a2dSetReference(ADC_REFERENCE_AVCC);
			a2dSetPrescaler(ADC_PRESCALE_DIV128);

			portInit();

	#ifdef ZIGBEE
		/* resetting the chip*/
			DDRB |= (1<<3);

			cbi(PORTB,3);
			vdelay();

	cbi(PORTD,VREG);//Vreg OFF New board
	vdelay();
	
	cbi(PORTB,3);
	vdelay();
	
	sbi(PORTD,VREG);//Vreg ON New board
	vdelay();

	sbi(PORTB,3);
	vdelay();
	/*end of hardware reset of the chip*/
	#endif


	#ifdef UART
	uartInit();
	uartSetBaudRate(UART_BAUD_RATE);

	rprintfInit(uartSendByte);
	#endif

	#ifdef ZIGBEE
		cc2420Init(0x15, 0xABCD, 0x1234);//channel,pan id and node id initialization
	#endif


	}

	void portInit(void)
	{
	DDRA=DDRB=DDRC=DDRD=0;		// all inputs initially
	//sbi(DDRC,PC5);				// LDR current source enable
	sbi(DDRC,PC2);				// debug LED

	sbi(DDRB,PB3);				// reset pin of cc2420	
	sbi(DDRD,PD2);				// Vreg pin of cc2420

	/* disable the cc2420 while not in use */
	#ifdef UART
		cbi(PORTB,PB3);			// reset pin active low, chip at reset
		cbi(PORTD,PD2);			// Vreg pin is at low, Vreg disabled
	#endif

	#ifdef ZIGBEE
		DDRD |= ((1<<VREG)|(1<<3) |(1<<4) |(1<<5) |(1<<6) | (1<<7));
		PORTD |= ((1<<VREG)|(1<<3) |(1<<4) |(1<<5) |(1<<6) );
		PORTD &=  ~(1<<VREG); //Vreg OFF
		
		DDRC &= ((1<<0) | (1<<1));		
		PORTC |= ((1<<0) | (1<<1));		//Pull up enabled for PortC 0 1
		

	#endif
}

void busyWait(void)
{
	u16 i;
	LED_OFF;
	for(i=0;i<60000;i++);
		LED_ON;
}


char convertChar(u08 in)
{
	if( in >= 0 && in <= 9)
		return (in	+	'0');
	
	else if (in >= 10 && in <= 15)
		return (in 	+ 	'A' - 10);

	return 0;

}

//PAN ID 0xABCD, NODE ID 0x1233
