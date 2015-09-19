#ifndef _DEFINES_H
 #define _DEFINES_H

// Interrupts
#define ENABLE_GLOBAL_INT()         do { asm ("sei\n\t" ::); } while (0)
#define DISABLE_GLOBAL_INT()        do { asm ("cli\n\t" ::); } while (0)

#ifdef __AVR_ATmega644__

#define FIFOP_INT_INIT()            do { EICRA |= 0x0C; CLEAR_FIFOP_INT(); } while (0)

// FIFOP on external interrupt 1
#define ENABLE_FIFOP_INT()          do { EIMSK |= 0x02; } while (0)
#define DISABLE_FIFOP_INT()         do { EIMSK &= 0x7D; } while (0)
#define CLEAR_FIFOP_INT()           do { EIFR |= 0x02; } while (0)

#else

// Rising edge trigger for external interrupt 0 (FIFOP)
#define FIFOP_INT_INIT()            do { MCUCR |= 0x0C; CLEAR_FIFOP_INT(); } while (0)

// FIFOP on external interrupt 1
#define ENABLE_FIFOP_INT()          do { GICR |= 0x80; } while (0)
#define DISABLE_FIFOP_INT()         do { GICR &= 0x7F; } while (0)
#define CLEAR_FIFOP_INT()           do { GIFR |= 0x80; } while (0)

#endif

#define FIFOP_IS_1	(CC2420_CTRL_PORTIN & (1<<CC2420_PIN_FIFOP))
#define FIFO_IS_1	(CC2420_CTRL_PORTIN & (1<<CC2420_PIN_FIFO))
#define SFD_IS_1	(CC2420_CTRL_PORTIN & (1<<CC2420_PIN_SFD))

// core definitions 
#define	 UART_BAUD_RATE	2400
u08 nh,nm,fh,fm,nap11,fap11;

// The data structure which is used to transmit packets
typedef struct 
{
    u16 destPanId;
    u16 destAddr;
    u08 length;
    u08 *pPayload;
    u08 ackRequest;
} BASIC_RF_TX_INFO;


// The receive struct:
typedef struct 
{
    u08 seqNumber;
    u16 srcAddr;
    u16 srcPanId;
    u08 length;
    u08 *pPayload;
    u08 ackRequest;
    u08 rssi;
} BASIC_RF_RX_INFO;

// The RF settings structure:
typedef struct 
{
    BASIC_RF_RX_INFO *pRxInfo;
    u08 txSeqNumber;
    volatile u08 ackReceived;
    u16 panId;
    u16 nodeId;
    u08 receiveOn;
} BASIC_RF_SETTINGS;
volatile BASIC_RF_SETTINGS rfSettings;

typedef struct  
{
    u08 direction;
    u16 srcaddr;
    u16 dstaddr;
    u08 nod; // Number of data
    u08 payload[111]; //128-11(MAC)-(1+2+2+1)
}nwklayer;
nwklayer packet;

BASIC_RF_RX_INFO rfRxInfo;
BASIC_RF_TX_INFO rfTxInfo;

u08 pTxBuffer[128];
u08 pRxBuffer[128];


#endif
