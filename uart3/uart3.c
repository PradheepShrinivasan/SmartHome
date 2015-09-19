#include<avr/io.h>
#include<avr/interrupt.h>
#include<global.h>

#include<util/delay.h>
#include "string.h"
#include "timer.h"
#include "timer.c"

#include "buffer.h"
#include "buffer.c"

#include "uartsw.c"
#include "uartsw.h"


#include "rprintf.h"
#include "rprintf.c"

#include "ssp.c"



int main()
{
	/*unsigned char str[5]="TARC",i=8;
     //DDRB=0XFF;	
    char myReceivedByte=0;	

		

	uartswInit();                 // initialize UART (serial port)
	rprintfInit(uartswSendByte);
    uartswSetBaudRate(2400);      // set UART speed to 38400 baud

//		rprintf(str);
        //for(i=0;i<4;i++)
//	    uartSendByte(str[i]);

	while(1)
	{
	    //rprintf("Hello");
        while(!uartswReceiveByte(&myReceivedByte));
         
	     //rprintfu08(i);
		 rprintfChar(myReceivedByte-8);
	    //uartswSendByte(myReceivedByte);
		//uartswSendByte(str[1]);
    }*/
char ch='T';
ser2_init();
while(1)
{
if (ser2_ischar())
{
// there's a character waiting, deal
ch = ser2_in();
}
ser2_out(ch);
}
}
