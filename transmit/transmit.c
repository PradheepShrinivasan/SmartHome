#define MOBILENODE

#define CHANNEL 0X15
#define PANID   0xABCD
#define NODEID  0X1235

#define TEMP      0x01
#define LIGHT     0x02
#define DATE      0x03
#define TIME      0x04
#define ROOMLIST  0x05
#define TIMECHG   0x06
#define SWHLIST   0x07
#define DATECHG   0x08
#define PRINT1    0xFF
#define A2DLIGHT  0x00

//#define TEMP   0x01
//#define LIGHT  0x02
#define ULTRA  0x03
//#define TIME   0x04
#define SWTCH  0xAA
#define SCHEDULE  0xAB

#define COORADDR 0x1232
#define PIR      0xBB

#include "include.h"


// Function Defns 
int	main(void);			//main

int main(void)
{
    //unsigned char  i,j=0;
    u08 i,j,g,sel,rm,k;
    u08 sw_byte1,sw_byte2,sw_byte3;

    initAll();
    DDRC=0xff;
    rprintf("Hai");
    rprintfCRLF();
    rprintf("Hai");	

    PORTC=0xff;
    rfTxInfo.ackRequest = TRUE;
    rfTxInfo.pPayload = pTxBuffer;
    rfRxInfo.pPayload = pRxBuffer;
    basicRfReceiveOn();


    cbi(PORTA,RST);
    LCDINI();

    Page0:
    LCDGOTOXY(1,0);

    /// draws smart home picture
    for(i=0;i<=62;i++)
    for(j=0;j<=7;j++)
        spi_write(Welup[i][j]);

    for(i=0;i<10;i++)
        _delay_ms(200);

    cbi(PORTA,RST);

    while(1)
    {

        packet.payload[0]=PRINT1;
        for (k=0;k<99;k++)
            packet.payload[k+1]=k+2;

        if(nwkpacketsend(UP,0x1235,0x1245,101,0x1232))// the time is to be got from co
            rprintf("SUCCESS");
        else
            rprintf("Immediate Destination Error");

        for(i=0;i<10;i++)
            _delay_ms(100);

        if(flag==1)
        {
            rprintf("Interrupted");
            nwkpacketrec();
        }

        Mainmenu:
        packet.payload[0]=TIME;
        packet.payload[0]=DATE;
        packet.payload[1]=TIME;
        if(nwkpacketsend(UP,0x1235,0x1245,0x02,0x1232))// the time is to be got from co
            rprintf("SUCCESS");
        else
            rprintf("Immediate Destination Error");

        for(i=0;i<10;i++)
            _delay_ms(100);

        if(flag==1)
        {
            rprintf("Interrupted");
            nwkpacketrec();
        }

        sbi(PORTA,RST);
        LCDBRD();/// draws border

        LCDCLR();// clear lcd
        sprintf(date,"%x-%02x-%02x",packet.payload[0] ,packet.payload[1],packet.payload[2]);
        sprintf(time,"%x:%02x %c",packet.payload[4],packet.payload[3],packet.payload[5]?'P':'A');
        //displays date and time
        LCDSTR(date,4,1,0);
        LCDSTR(time,5,2,0);
        LCDMEN("H O M E",4,4);
        //waiting for key
        Page:
        if(getKey()!=KEY_OK) goto Mainmenu;

        //MENU Page 0 // menu list 
        Menulist:
        pos=0;             //pos=highlighted position //all  room names and total no of rooms

        LCDCLR();
        for(i=0;i<T_1;i++)
            ri[i]=i;   
        
        rmdisp(T_1,0,0);
        while(1)
        {
            g=getKey();

            if(g==KEY_DOWN || g==KEY_UP)
                rmdisp(T_1,g,0);
            else if (g==KEY_BACK)
                goto Mainmenu;
            else if(g==KEY_OK) 
                break;
        }   

        sel=ri[pos];

        if (sel == 3)// set time - time syncronize with central server(cordinator) date time()
        {
            TimeSet();
            goto Mainmenu;
        }

        //ROOMS Disp page 1 pressing of ok

        Roomlist:
        pos=0;
        //Page3:
        LCDCLR();

        for(i=0;i<RNO;i++)
            ri[i]=i;   

        rmdisp(RNO,0,1);
        while(1)
        {
            g=getKey();
            if(g==KEY_UP || g==KEY_DOWN)
                rmdisp(RNO,g,1);
            else if(g==KEY_BACK)
                goto Menulist;
            else if(g==KEY_OK) 
                break;
        }
        rm=ri[pos]+1;

        // selection based values ie room choice->temp ,pressure,control etc
        //SENSOR Status Page 2 

        if(sel==0) 
        {
            //rprintf("Room");
            packet.payload[1]=TEMP;
            packet.payload[0]=LIGHT;
            if(nwkpacketsend(UP,0x1235,0x1245+(rm-1),0x02,0x1232))
                rprintf("SUCCESS");
            else
                rprintf("Immediate Destination Error");

            for(i=0;i<10;i++)
                _delay_ms(100);

            if(flag==1)
            {
                rprintf("Interrupted");
                nwkpacketrec();
            }

            sprintf(CON[1],"%d.%02d",packet.payload[1],(100*((u32)(packet.payload[2])))/256);  //Temp
            sprintf(CON[3],"%d%%",packet.payload[0]);  //Light
            rmdisp(8,0,2);
        }

        while(sel==0)
        {

            g=getKey();
            if(g==KEY_DOWN || g==KEY_UP) rmdisp(10,g,2);
            else if (g==KEY_BACK)  goto Roomlist;
            else if(g==KEY_OK) goto Mainmenu;
        }



        pos=0;   //////////////////////////////////////control page switch display
        if(sel==1)
        {

            j=10;
            for(i=0;i<j;i++)
                ri[i]=i;

            swi(j,0,0);

            while(1)
            {    
                g=getKey();
                if(g==KEY_UP || g==KEY_DOWN)
                swi(j,g,0);
                else if(g==KEY_STAR) 
                { 
                    switc[ri[pos]][6] = switc[ri[pos]][6] ^ 0xff; 
                    swi(j,20,0);
                }
                else if(g==KEY_BACK) goto Roomlist;
                else if(g==KEY_OK)
                {
                    packet.payload[0]=SWTCH;

                    for(i=0;i<2;i++)
                        packet.payload[i+1]= switc[i][6] &1;

                    if(nwkpacketsend(UP,0x1235,0x1245,3,0x1232))// the time is to be got from co
                        rprintf("SUCCESS");
                    else
                        rprintf("Immediate Destination Error");

                    for(i=0;i<10;i++)
                        _delay_ms(100);

                    goto Mainmenu;
                }
            }
            goto Page0;
        }
        else if(sel==4)
        {
            pos=0;
            NameSet(1,rm);
            goto Menulist;
        }
        else if(sel==5)
        {
            j=0;
            for(i=0;i<20;i++)
                rd[j++]=i;
           
            for(i=0;i<=j;i++)
                ri[i]=i;

            rmdisp(j+1,0,4);
            while(1)
            {
                g=getKey();
                if(g==KEY_DOWN || g==KEY_UP)
                rmdisp(j+1,g,4);
                else if(g==KEY_OK) break;
            }

            pos=0;
            NameSet(0,ri[pos]);
            goto Menulist;

        }
        else if(sel==2)
        {
            SchSwitch:

            for(i=0;i<10;i++)
                ri[i]=i;

            swi(10,0,1);
            
            while(1)
            {
                g=getKey();
                if(g==KEY_DOWN || g==KEY_UP)
                    swi(10,g,1);
                else if (g==KEY_STAR)
                {
                    sch[ri[pos]][3]  =sch[ri[pos]][3] ^ 0x01;
                    swi(10,19,1);
                }
                else if (g==KEY_BACK)
                goto Roomlist;
                else if(g==KEY_OK)
                {
                    //for(i=0;i<4;i++)
                    // packet.payload[i+1]= schd[0][i] ;
                    break;
                }
            }

            j=ri[pos];
            SchDisp:
            LCDCLR();
            schd(j,0);
            pos=0;
            while(1)
            {
                g=getKey();
                if(g<=KEY_9 || g==KEY_STAR) 
                    SchTimeSet(g,j);
                else if(g==KEY_DOWN && pos <16) pos =(pos +1);
                else if(g==KEY_UP && pos>0)   pos = (pos-1); 
                schd(j,0);
                if (g==KEY_BACK)
                goto SchSwitch;
                else
                if(g==KEY_OK)
                {
                    g=SchTimeChk(j);
                    if(g==1) goto SchDisp;
                    else
                    {
                        packet.payload[0]=SCHEDULE;
                        packet.payload[1]=sch[j][0];
                        packet.payload[2]=sch[j][1];
                        packet.payload[3]=sch[j][2];
                        packet.payload[4]=sch[j][3];
                        if(nwkpacketsend(UP,0x1235,0x1245,0x05,0x1232))// the time is to be got from co
                        rprintf("SUCCESS");
                        else
                        rprintf("Immediate Destination Error");
                        for(i=0;i<10;i++)
                        _delay_ms(100);

                        goto Mainmenu;
                    }
                }
            }

        }

        cbi(PORTA,4);

        while(1);
        return 0;
    }
}	




    



