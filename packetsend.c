#ifndef PACKETSEND_C
#define PACKETSEND_C


#include "packetsend.h"


u08 cc2420SendPck(BASIC_RF_TX_INFO *pRTI)
	{
     	
	u16 frameControlField;
    u08 packetLength;
    u08 success;
    u08 spiStatusByte;
	u08 status;
	u08 _count;

    // Wait until the transceiver is idle
    while (FIFOP_IS_1 || SFD_IS_1);
    //rprintf("inside send");


	// Flush the TX FIFO just in case...
	cc2420Command(CC2420_SFLUSHTX);
	    // Turn off global interrupts to avoid interference on the SPI interface
    DISABLE_GLOBAL_INT();

    // Turn on RX if necessary
    if (!rfSettings.receiveOn) cc2420Command(CC2420_SRXON);

    // Wait for the RSSI value to become valid
    do {
        status = cc2420GetStatus();
    } while (!(status & (1 << CC2420_RSSI_VALID)));
	  

    // Write the packet to the TX FIFO (the FCS is appended automatically when AUTOCRC is enabled)
    packetLength = pRTI->length + BASIC_RF_PACKET_OVERHEAD_SIZE;
    cc2420WriteFifo((u08*)&packetLength, 1);               // Packet length
    frameControlField = pRTI->ackRequest ? BASIC_RF_FCF_ACK : BASIC_RF_FCF_NOACK;
    cc2420WriteFifo((u08*) &frameControlField, 2);         // Frame control field
    cc2420WriteFifo((u08*) &rfSettings.txSeqNumber, 1);    // Sequence number
    cc2420WriteFifo((u08*) &rfSettings.panId, 2);          // Dest. PAN ID
    cc2420WriteFifo((u08*) &pRTI->destAddr, 2);            // Dest. address
    cc2420WriteFifo((u08*) &rfSettings.nodeId, 2);         // Source address
	cc2420WriteFifo((u08*) pRTI->pPayload, pRTI->length);  // Payload

// TX begins after the CCA check has passed
do {
		cc2420Command(CC2420_STXONCCA);
		status = cc2420GetStatus();
		for(_count=0;_count<=100;_count++);
    } while (!(status & (1 << CC2420_TX_ACTIVE)));

	// Wait for the transmission to begin before exiting (makes sure that this function cannot be called
	// a second time, and thereby cancelling the first transmission (observe the FIFOP + SFD test above).
	while (!SFD_IS_1);
	success = TRUE;

	// Turn interrupts back on
	ENABLE_GLOBAL_INT();

    // Wait for the acknowledge to be received, if any
    if (pRTI->ackRequest) {
		rfSettings.ackReceived = FALSE;

		// Wait for the SFD to go low again
		while (SFD_IS_1);

        // We'll enter RX automatically, so just wait until we can be sure that the ack reception should have finished
        // The timeout consists of a 12-symbol turnaround time, the ack packet duration, and a small margin
        _delay_ms(100);

		// If an acknowledgment has been received (by the FIFOP interrupt), the ackReceived flag should be set
		success = rfSettings.ackReceived;
    }

	// Turn off the receiver if it should not continue to be enabled
    DISABLE_GLOBAL_INT();
	if (!rfSettings.receiveOn) cc2420Command(CC2420_SRFOFF);
    ENABLE_GLOBAL_INT();

    // Increment the sequence number, and return the result
    rfSettings.txSeqNumber++;
    return success;

	}

#endif
