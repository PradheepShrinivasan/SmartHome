#ifndef PACKETRECIEVE_H
 #define PACKETRECIEVE_H


//-------------------------------------------------------------------------------------------------------
//  void halRfReceiveOn(void)
//
//  DESCRIPTION:
//      Enables the CC2420 receiver and the FIFOP interrupt. When a packet is received through this
//      interrupt, it will call halRfReceivePacket(...), which must be defined by the application
//-------------------------------------------------------------------------------------------------------
void basicRfReceiveOn(void);



//-------------------------------------------------------------------------------------------------------
//  void halRfReceiveOff(void)
//
//  DESCRIPTION:
//      Disables the CC2420 receiver and the FIFOP interrupt.
//-------------------------------------------------------------------------------------------------------
void basicRfReceiveOff(void);



//-------------------------------------------------------------------------------------------------------
//  ISR(SIG_INTERRUPT0) - CC2420 FIFOP interrupt service routine
//
//  DESCRIPTION:
//		When a packet has been completely received, this ISR will extract the data from the RX FIFO, put
//		it into the active BASIC_RF_RX_INFO structure, and call basicRfReceivePacket() (defined by the
//		application). FIFO overflow and illegally formatted packets is handled by this routine.
//
//      Note: Packets are acknowledged automatically by CC2420 through the auto-acknowledgment feature.
//-------------------------------------------------------------------------------------------------------
// ISR(SIG_INTERRUPT0)


//-------------------------------------------------------------------------------------------------------
//  BASIC_RF_RX_INFO* basicRfReceivePacket(BASIC_RF_RX_INFO *pRRI)
//
//  DESCRIPTION:
//      This function is a part of the basic RF library, but must be declared by the application. Once
//		the application has turned on the receiver, using basicRfReceiveOn(), all incoming packets will
//		be received by the FIFOP interrupt service routine. When finished, the ISR will call the
//		basicRfReceivePacket() function. Please note that this function must return quickly, since the
//		next received packet will overwrite the active BASIC_RF_RX_INFO structure (pointed to by pRRI).
//
//  ARGUMENTS:
//		BASIC_RF_RX_INFO *pRRI
//	      	The reception structure, which contains all relevant info about the received packet.
//
//  RETURN VALUE:
//     BASIC_RF_RX_INFO*
//			The pointer to the next BASIC_RF_RX_INFO structure to be used by the FIFOP ISR. If there is
//			only one buffer, then return pRRI.
//-------------------------------------------------------------------------------------------------------
u08 flag=0;


#endif


