//this is for room 2

#define REMOTENODE

#define CHANNEL 0X15
#define PANID   0xABCD
#define NODEID  0X1246


#define TEMP      0x01
#define LIGHT     0x02
#define DATE      0x03
#define TIME      0x04
#define ROOMLIST  0x05
#define TIMECHG   0x06
#define SWHLIST   0x07
#define DATECHG   0x08

#define ROOM1     0x01
#define ROOM2     0x02
#define ROOM3     0x03

#define COORADDR 0x1232
#define A2DLIGHT  0x00
#define SWTCH    0xAA
#define SCHEDULE 0xAB
#define PIR      0xBB
#define PRINT1    0xFF


void applay(void);

#include "include.h"
//#include "a2d.h"		// include A/D converter function library

//#include "a2d.c"		// include A/D converter function library

#define LDR_PIN 5

#define LDR 0

#define ENABLE_LDR()  cbi(PORTC,LDR_PIN)
#define DISABLE_LDR() sbi(PORTC,LDR_PIN)

// Function Defns 
int main(void);			//main


int main(void)
{
	unsigned char  i;
	u08 j=1,k;
	initAll();
	DDRC=0xff;
	sbi(DDRC,LDR_PIN);
	rprintf("Hai");
	//rprintfCRLF();
	//rprintf("Hai");	
	PORTC=0xff;
	initI2c();
	a2dInit();
   // rprintf("Hai");

	if(ds1624_check())    
	{
		ds1624_config();
		ds1624_startconvert();
		ds1624_readtemp();
		rprintf("1624 working"); 
	}
	else 
	{
		rprintf("failed");

	}
	if(ds1307_check()) 
	{   
    	DS1307_settime(0x09,0x20,0x00);		// Set Time (hh:mm:ss) 
    	DS1307_setdate(0x23,0x06,0x07);
    	rprintf("1307 working"); 
    }
    else 
    	rprintf("1307 failed");

    a2dInit();
    a2dSetPrescaler(ADC_PRESCALE_DIV128);
    a2dSetReference(ADC_REFERENCE_AVCC);


    rfTxInfo.length = 5;
    rfTxInfo.ackRequest = TRUE;
    rfTxInfo.pPayload = pTxBuffer;
    rfRxInfo.pPayload = pRxBuffer;
    basicRfReceiveOn();

	// so we can receive analog signals
    DDRA = 0x00;
	// make sure pull-up resistors are turned off
    PORTA = 0x00;

    a2dSetPrescaler(ADC_PRESCALE_DIV128);

	// set the a2d reference
	// - the reference is the voltage against which a2d measurements are made
	// - other allowed reference values can be found in a2d.h
    a2dSetReference(ADC_REFERENCE_AVCC);
    DISABLE_LDR();
    while(1)
    {

    	j=a2dConvert8bit(1);
    	j=(255-j)*100/255;
    	if(j==0)
    	{
    		rprintf("MOTION DETECTED");
    		packet.payload[0]=PIR;
         	if(nwkpacketsend(UP,0x1245,0x1235,0x01,0x1232))// the time is to be got from co
         		rprintf("SUCCESS");
         	else
         		rprintf("Immediate Destination Error");
         	for(i=0;i<10;i++)
         		_delay_ms(100);
         }

         if(flag==1)
         {
         	nwkpacketrec();

         	flag=0;
         }
     }
 }	




