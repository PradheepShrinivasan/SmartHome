#ifndef NWKPACKETSEND_C
 #define NWKPACKETSEND_C

#include "nwkpacketsend.h"

u08 nwkpacketsend(u08 direction,u16 srcaddr,u16 dstaddr, u08 nod, u16 macdst)
{
	u08 i_s,success=1;
	packet.direction = direction;
	packet.srcaddr= srcaddr;
	packet.dstaddr= dstaddr;
	packet.nod=nod;

	rfTxInfo.length  = 6+(packet.nod);
	rfTxInfo.destAddr = macdst;

	pTxBuffer[0]=packet.direction;
	pTxBuffer[1]=packet.srcaddr&0xFF;
	pTxBuffer[2]=packet.srcaddr>>8;
	pTxBuffer[3]=packet.dstaddr&0xFF;
	pTxBuffer[4]=packet.dstaddr>>8;
	pTxBuffer[5]=packet.nod;
	rprintf("send%d",packet.dstaddr);
	for(i_s=6;i_s<packet.nod+6;i_s++) 
		pTxBuffer[i_s]=packet.payload[i_s-6];

	i_s=0;
	while(!cc2420SendPck(&rfTxInfo))
	{
		i_s++;
		_delay_ms(50);
		if(i_s>5)
		{
			success=0; 
			break;
		}
	}
	return success;
}

#endif
