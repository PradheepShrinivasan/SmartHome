#define COORDINATOR

#define CHANNEL 0X15
#define PANID   0xABCD
#define NODEID  0X1232

#define TEMP      0x01
#define LIGHT     0x02
#define DATE      0x03
#define TIME      0x04
#define ROOMLIST  0x05
#define CHGTIME   0x06
#define SWHLIST   0x07
#define SWTCH     0XAA


#define ROOM1     0x01
#define ROOM2     0x02
#define ROOM3     0x03

#define A2DLIGHT  0x00


#define COORADDR 0x1232
void applay(void);
#include "include.h"


// Function Defns 
int main(void);			//main


int main(void)
{
	unsigned char  i,j=0;	
	initAll();

	DDRC=0xff;	
	PORTC=0xff;

	rfTxInfo.length = 5;
	rfTxInfo.ackRequest = TRUE;
	rfTxInfo.pPayload = pTxBuffer;
	rfRxInfo.pPayload = pRxBuffer;
	basicRfReceiveOn();

	while(1)
	{
		if(flag==1)
		{
			rprintf("Interrupted");
			nwkpacketrec();
			rprintf("returned");

			flag=0;
		}
	}
}	


