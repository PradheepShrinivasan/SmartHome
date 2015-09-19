#ifndef _CC2420INIT_C
 #define _CC2420INIT_C

#include "cc2420init.h"

void cc2420Init(BASIC_RF_RX_INFO *pRRI,u08 channel, u16 panId, u16 nodeId)
{
    FIFOP_INT_INIT();
    ENABLE_FIFOP_INT();

    DISABLE_GLOBAL_INT();
    spiInit();
    sbi(DDRB,CC2420_PIN_CS);
    sbi(PORTB,CC2420_PIN_CS);

    cbi(CC2420_CTRL_DDR,CC2420_PIN_FIFO);
    cbi(CC2420_CTRL_DDR,CC2420_PIN_FIFOP);
    cbi(CC2420_CTRL_DDR,CC2420_PIN_CCA);
    cbi(CC2420_CTRL_DDR,CC2420_PIN_SFD);
    sbi(CC2420_CTRL_PORT,CC2420_PIN_FIFO);
    sbi(CC2420_CTRL_PORT,CC2420_PIN_FIFOP);
    sbi(CC2420_CTRL_PORT,CC2420_PIN_CCA);
    sbi(CC2420_CTRL_PORT,CC2420_PIN_SFD);

        // soft-reset chip
    cc2420WriteReg(CC2420_MAIN,0x0000);
    cc2420WriteReg(CC2420_MAIN,0xF800);
        // turn on oscillator
    cc2420Command(CC2420_SXOSCON);

    // prepare registers

    cc2420WriteReg(CC2420_SYNCWORD, 0xA70F);    // Sync word...

    cc2420WriteReg(CC2420_MDMCTRL0, 0x0AF2);    // Turn on automatic packet acknowledgment
    cc2420WriteReg(CC2420_MDMCTRL1, 0x0500);    // Set the correlation threshold = 20
    cc2420WriteReg(CC2420_DACTST  , 0x0000);

    cc2420WriteReg(CC2420_IOCFG0,   0x0040);    // Set the FIFOP threshold to 64


    cc2420WriteReg(CC2420_SECCTRL0, 0x01C4);    // Turn off "Security enable"
    cc2420WriteReg(CC2420_TXCTRL, 0xA0FF);


    // set radio channel
    cc2420SetChannel(channel);
    ENABLE_GLOBAL_INT();
    rfSettings.pRxInfo = pRRI;
    rfSettings.panId = panId;
    rfSettings.nodeId = nodeId;
    rfSettings.txSeqNumber = 0;
    rfSettings.receiveOn = FALSE;
    rprintf("1");
    
    // wait until oscillator stable
    while(!(cc2420GetStatus() & (1<<CC2420_XOSC16M_STABLE)));
    rprintf("2");
    
    // set IDs
    DISABLE_GLOBAL_INT();
    cc2420WriteRam(CC2420RAM_SHORTADDR, (u08*)&nodeId, 2);
    cc2420WriteRam(CC2420RAM_PANID, (u08*)&panId, 2);
    ENABLE_GLOBAL_INT();
}
#endif
