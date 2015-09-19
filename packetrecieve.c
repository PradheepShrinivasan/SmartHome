#ifndef PACKETRECIEVE_C
 #define PACKETRECIEVE_C

#include "packetrecieve.h"


ISR(INT1_vect) 
{
	u16 frameControlField;
	u08 length;
	u08 pFooter[2];

    // Clean up and exit in case of FIFO overflow, which is indicated by FIFOP = 1 and FIFO = 0
	if((FIFOP_IS_1) && (!(FIFO_IS_1))) {	   
		cc2420Command(CC2420_SFLUSHRX);
		cc2420Command(CC2420_SFLUSHRX);
		return;
	}

	// Payload length
	cc2420ReadFifo(&length,1);
	length &= BASIC_RF_LENGTH_MASK; // Ignore MSB

    // Ignore the packet if the length is too short
	if (length < BASIC_RF_ACK_PACKET_SIZE) {
		cc2420DumpFifo(length);

    // Otherwise, if the length is valid, then proceed with the rest of the packet
	} else {

        // Register the payload length
		rfSettings.pRxInfo->length = length - BASIC_RF_PACKET_OVERHEAD_SIZE;

        // Read the frame control field and the data sequence number
		cc2420ReadFifo((u08*) &frameControlField, 2);
		rfSettings.pRxInfo->ackRequest = !!(frameControlField & BASIC_RF_FCF_ACK_BM);
		cc2420ReadFifo(&rfSettings.pRxInfo->seqNumber,1);

		// Is this an acknowledgment packet?
		if ((length == BASIC_RF_ACK_PACKET_SIZE) && (frameControlField == BASIC_RF_ACK_FCF) && (rfSettings.pRxInfo->seqNumber == rfSettings.txSeqNumber)) {

 	       	// Read the footer and check for CRC OK
			cc2420ReadFifo((u08*) pFooter, 2);

			// Indicate the successful ack reception (this flag is polled by the transmission routine)
			if (pFooter[1] & BASIC_RF_CRC_OK_BM) rfSettings.ackReceived = TRUE;
			
		// Too small to be a valid packet?
		} else if (length < BASIC_RF_PACKET_OVERHEAD_SIZE) {
			cc2420DumpFifo(length - 3);
			return;

		// Receive the rest of the packet
		} else {

			// Skip the destination PAN and address (that's taken care of by harware address recognition!)
			cc2420DumpFifo(4);

			// Read the source address
			cc2420ReadFifo((u08*) &rfSettings.pRxInfo->srcAddr, 2);

			// Read the packet payload
			cc2420ReadFifo(rfSettings.pRxInfo->pPayload, rfSettings.pRxInfo->length);

			// Read the footer to get the RSSI value
			cc2420ReadFifo((BYTE*) pFooter, 2);
			rfSettings.pRxInfo->rssi = pFooter[0];

			// Notify the application about the received _data_ packet if the CRC is OK
			if (((frameControlField & (BASIC_RF_FCF_BM)) == BASIC_RF_FCF_NOACK) && (pFooter[1] & BASIC_RF_CRC_OK_BM)) {
				//rfSettings.pRxInfo = basicRfReceivePacket(rfSettings.pRxInfo);
				flag=1;
			}
		}
	}

} // SIGNAL(SIG_INTERRUPT0)

void basicRfReceiveOn(void) 
{
	rfSettings.receiveOn = TRUE;
	cc2420Command(CC2420_SRXON);
	cc2420Command(CC2420_SFLUSHRX);
	//rprintf("WW");
	ENABLE_FIFOP_INT();
} // basicRfReceiveOn

void basicRfReceiveOff(void) 
{
	rfSettings.receiveOn = FALSE;
	cc2420Command(CC2420_SRFOFF);
	DISABLE_FIFOP_INT();
} // basicRfReceiveOff


#endif
