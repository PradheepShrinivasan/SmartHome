#ifndef NWKPACKETREC_C
  #define NWKPACKETREC_C

#include "nwkpacketrec.h"

void nwkpacketrec(void)
{
    u08 i;
    packet.direction   =rfSettings.pRxInfo->pPayload[0];
    packet.srcaddr     =rfSettings.pRxInfo->pPayload[1];
    packet.srcaddr    |=(rfSettings.pRxInfo->pPayload[2]<<8);
    packet.dstaddr     =rfSettings.pRxInfo->pPayload[3];
    packet.dstaddr    |=(rfSettings.pRxInfo->pPayload[4]<<8);
    packet.nod         =rfSettings.pRxInfo->pPayload[5];
    for(i=6;i<rfSettings.pRxInfo->length;i++)
        packet.payload[i-6]  =rfSettings.pRxInfo->pPayload[i];
    
    //rprintf("Inside NPR %d",packet.srcaddr);
    rprintf("Inside NPR %d",packet.dstaddr);
    if(packet.dstaddr==NODEID)
    {
        // see what is asked in the payload and then send the same
        #ifdef COORDINATOR
        rprintf("inside ifdef");    
        nwkpacketsend(DOWN,NODEID,packet.srcaddr,packet.nod,packet.srcaddr);
        #endif

        #ifdef  REMOTENODE
        applay();
        _delay_ms(100);
        nwkpacketsend(DOWN,NODEID,packet.srcaddr,packet.nod,COORADDR);
        #endif
        
        #ifdef  MOBILENODE
        if(packet.payload[0]==PIR)
        {
            rprintf("MOTION DETECTED");
            pirmotion();
        }
        #endif

    }
    else
    {
            //rprintf("forwarding to %d",packet.dstaddr);
        if(nwkpacketsend(packet.direction,packet.srcaddr,packet.dstaddr, packet.nod, packet.dstaddr))
        {
            rprintf("SUCCESS");
        }
        else
        {
            packet.payload[0]='N';
            packet.payload[1]='A';
            nwkpacketsend(NOTWORKING,NODEID,packet.srcaddr, packet.nod, packet.srcaddr);
            rprintf("FAILURE");
        }
    }
}

#if defined(COORDINATOR) || defined(REMOTENODE)  

void applay(void)
{
    u08 i,j,data,temp[128],payloadptr=0,val,sbyte1,sbyte2,ctmin,ct;
    u16 temper,k,ct1;

 //rprintf("inside applay");
    for(i=0;i<packet.nod;i++)
    {
        temp[i]=packet.payload[i];
    //rprintf("\n");
    //rprintfChar(temp[i]);
    //rprintf("NOD");
    //rprintf("%d",packet.nod);
    }

    rprintf("%d",packet.nod);
 // rprintfu08(packet.nod);
    for(i=0;i<packet.nod;i++)
    {
   // rprintf("inside case");
        data=temp[i];
        rprintf("\n");
        rprintfu08(data);

        switch (data)
        {

            case TEMP:
           //rprintf("inside temp");
            temper=ds1624_readtemp();
            packet.payload[payloadptr]=temper>>8;
            packet.payload[payloadptr+1]=temper&0xFF;
            payloadptr=payloadptr+2;
            /*rprintf("Temp is: ");
            rprintfNum(10, 4, FALSE , ' ', temper>>8);
            rprintf(".");
            rprintfNum(10, 4, FALSE, '0', (10000*((u32)(temper&0x00FF)))/256 );*/
            break;

            case LIGHT:
           // rprintf("inside light");
            j=a2dConvert8bit(A2DLIGHT);
            j=(255-j)*100/255;
            packet.payload[payloadptr]=j;
            payloadptr++;
            break;

            case DATE:   
            packet.payload[payloadptr]  =DS1307_get(0x04);
            packet.payload[payloadptr+1]=DS1307_get(0x05);
            packet.payload[payloadptr+2]=DS1307_get(0x06);
            payloadptr=payloadptr+3;
            break;

            case TIME:
            rprintf("inside time"); 
            packet.payload[payloadptr]  =DS1307_get(0x01);
            val= DS1307_get(0x02);
            rprintf("%d",val);
            packet.payload[payloadptr+1]=val&0x1F;
            packet.payload[payloadptr+2]=(val>>5)&1;//
            payloadptr=payloadptr+2;
            break;

            case ROOMLIST:
            strcpy(&packet.payload[payloadptr],"ROOM1");
            payloadptr=payloadptr+6;
            strcpy(&packet.payload[payloadptr],"ROOM2");
            payloadptr=payloadptr+6;
            strcpy(&packet.payload[payloadptr],"ROOM3");
            payloadptr=payloadptr+6;
            break;

            case TIMECHG:
            rprintf("INside TImechg");
            rprintf("%d",temp[i+1]);
            rprintf("%d",temp[i+2]);
            rprintf("--%x--",(temp[i+2]&0x5F) + ( temp[i+3] << 5));

            //call the function set time and set date  in DS1307
            DS1307_settime( 0x40 | (temp[i+2]&0x5F) + ( temp[i+3] << 5) ,temp[i+1],0);     // Set Time (hh:mm:ss) 
            i=i+3;
            break;

            case DATECHG:
            rprintf("INside Datechg");
            rprintf("%d",temp[i+1]);
            rprintf("%d",temp[i+2]);
            rprintf("%d",temp[i+3]);
            DS1307_setdate(temp[i+1],temp[i+2],temp[i+3]);
            i=i+3;
            break;

            //list of switches in any room
            case SWTCH:
            rprintf("INside Switch");
         
            uartSendByte(0x55);
            uartSendByte(0xaa);
            uartSendByte(temp[i+1]);
            uartSendByte(temp[i+2]);
            i=i+2;
            break;//break for case SWITCHLIST

            case SCHEDULE:
            rprintf("Inside Schedule");
            nh=( (temp[i+1] & 0xf0) >> 4 );
            nm=( (temp[i+1] & 0x0f) << 2 ) | ((temp[i+2] & 0xc0) >> 6);
            nap11= (temp[i+2] & 0x20)?1:0;
            fh=( (temp[i+3] & 0xf0) >> 4 );
            fm=( (temp[i+3] & 0x0f) << 2 ) | ((temp[i+4] & 0xc0) >> 6);
            fap11= (temp[i+4] & 0x20)?1:0;
            i=i+4;
            break;

            case PRINT1:
            for(k=1;k<99;k++)
                rprintf("%d",temp[k]);
            i=i+100;
            break;

            case PIR:
            rprintf("MOTION DETECTED");
        }

    }
    packet.nod=payloadptr+1;
}

#endif
#endif



