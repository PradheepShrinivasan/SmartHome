#include<avr/io.h>
#include<global.h>
#include<buffer.h>
#include<buffer.c>
#include<rprintf.h>
#include<rprintf.c>
#include<uart.h>
#include<uart.c>

int main(void);

int main()
{
    //unsigned char str[5]="TARC";
    unsigned int i=0,j;
    u08 S1=0x55,S2=0xaa;

    uartInit();                 // initialize UART (serial port)
    rprintfInit(uartSendByte);
    uartSetBaudRate(2400);      // set UART speed to 9600 baud

    while(1)
    {
        uartSendByte(S1);
        for(j=0;j<20000;j++);
            uartSendByte(S2);
        for(j=0;j<20000;j++);

    }

}
