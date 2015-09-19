#define REMOTENODE

#define CHANNEL 0X15
#define PANID   0xABCD
#define NODEID  0X1245


#define TEMP      0x01
#define LIGHT     0x02
#define DATE      0x03
#define TIME      0x04
#define ROOMLIST  0x05
#define TIMECHG   0x06
#define SWHLIST   0x07
#define DATECHG   0x08
#define PRINT1    0Xff


#define ROOM1     0x01
#define ROOM2     0x02
#define ROOM3     0x03

#define COORADDR 0x1232
#define A2DLIGHT  0x00
#define SWTCH 0xAA
#define SCHEDULE 0xAB
#define PIR      0xBB
#define PRINT1    0xFF

void applay(void);

#include "include.h"

#define LDR_PIN 5

#define LDR 0

#define ENABLE_LDR()  cbi(PORTC,LDR_PIN)
#define DISABLE_LDR() sbi(PORTC,LDR_PIN)
// Function Defns 
int main(void);			//main


int main(void)
{
	unsigned char  i;
	u08 j=0,m1,h1,ap;
	u16 counter=0;

	initAll();
	DDRC=0xff;

	sbi(DDRC,LDR_PIN);
	rprintf("Hai");

	PORTC=0xff;
	initI2c();
	a2dInit();

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
    	DS1307_settime(0x51,0x58,0x00);		// Set Time (hh:mm:ss) 
    	DS1307_setdate(0x23,0x03,0x08);
    	rprintf("inside dsdate"); 
    	rprintf("date:%d",DS1307_get(0x00));
    	rprintf("mon:%d",DS1307_get(0x01));
    	rprintf("year:%d",DS1307_get(0x02));


    	rprintf("1307 working"); 
    }
    else 
    {
    	rprintf("1307 failed");
    }

    a2dInit();
    a2dSetPrescaler(ADC_PRESCALE_DIV128);
    a2dSetReference(ADC_REFERENCE_AVCC);

    //Set Date (dd/mm/yy) 
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
    	for(counter = 0;counter < 0xffff; counter++)
    	{
    		if(counter==0)
    		{

    			m1=DS1307_get(0x01);
    			h1=DS1307_get(0x02); 

    			ap=(h1&0x20)?1:0;
    			m1=((m1&0xf0)>>4)*10  + (m1&0x0f);
    			h1=((h1&0x10)>>4)*10  + (h1&0x0f);

    			if(nm==m1   && nh==h1  && ap==nap11)
    			{
    				uartSendByte(0x55);
    				uartSendByte(0xaa);
    				uartSendByte(1);
    				uartSendByte(0xff);
    			}
    			else if(fm==m1   && fh==h1 && ap==fap11)
    			{
    				uartSendByte(0x55);
    				uartSendByte(0xaa);
    				uartSendByte(0);
    				uartSendByte(0xff);

    			}
    		}

    		if(flag==1)
    		{
    			nwkpacketrec();

    			flag=0;
    		}
    	}
    }
}	




