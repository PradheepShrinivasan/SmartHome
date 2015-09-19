#ifndef _CC2420INIT_C
 #define _CC2420INIT_C

BASIC_RF_RX_INFO rfRxInfo;
BASIC_RF_TX_INFO rfTxInfo;

u08 pTxBuffer[128];
u08 pRxBuffer[128];
u08 flag=0;

//-------------------------------------------------------------------------------------------------------
//  void cc2420Init(BASIC_RF_RX_INFO *pRRI, UINT8 channel, WORD panId, WORD myAddr)
//
//  DESCRIPTION:
//      Initializes CC2420 for radio communication via the basic RF library functions. Turns on the
//		voltage regulator, resets the CC2420, turns on the crystal oscillator, writes all necessary
//		registers and protocol addresses (for automatic address recognition). Note that the crystal
//		oscillator will remain on (forever).
//
//  ARGUMENTS:
//      BASIC_RF_RX_INFO *pRRI
//          A pointer the BASIC_RF_RX_INFO data structure to be used during the first packet reception.
//			The structure can be switched upon packet reception.
//      UINT8 channel
//          The RF channel to be used (11 = 2405 MHz to 26 = 2480 MHz)
//      WORD panId
//          The personal area network identification number
//      WORD myAddr
//          The 16-bit short address which is used by this node. Must together with the PAN ID form a
//			unique 32-bit identifier to avoid addressing conflicts. Normally, in a 802.15.4 network, the
//			short address will be given to associated nodes by the PAN coordinator.
//-------------------------------------------------------------------------------------------------------
void cc2420Init(BASIC_RF_RX_INFO *pRRI,u08 channel, u16 panId, u16 nodeId);

#endif
