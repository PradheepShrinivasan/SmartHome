#ifndef PACKETSEND_H
#define PACKETSEND_H

//-------------------------------------------------------------------------------------------------------
//  BYTE basicRfSendPacket(BASIC_RF_TX_INFO *pRTI)
//
//  DESCRIPTION:
//		Transmits a packet using the IEEE 802.15.4 MAC data packet format with short addresses. CCA is
//		measured only once before backet transmission (not compliant with 802.15.4 CSMA-CA).
//		The function returns:
//			- When pRTI->ackRequest is FALSE: After the transmission has begun (SFD gone high)
//			- When pRTI->ackRequest is TRUE: After the acknowledgment has been received/declared missing.
//		The acknowledgment is received through the FIFOP interrupt.
//
//  ARGUMENTS:
//      BASIC_RF_TX_INFO *pRTI
//          The transmission structure, which contains all relevant info about the packet.
//
//  RETURN VALUE:
//		BOOL
//			Successful transmission (acknowledgment received)
//-------------------------------------------------------------------------------------------------------
u08 cc2420SendPck(BASIC_RF_TX_INFO *pRTI);



#endif
