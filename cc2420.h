#ifndef CC2420_H
	#define CC2420_H
#define CC2420_PIN_CS		PB4


#define CC2420_CTRL_PORT	PORTD
#define CC2420_CTRL_DDR		DDRD
#define CC2420_CTRL_PORTIN	PIND


#define CC2420_PIN_FIFO		PD4 //18 
#define CC2420_PIN_FIFOP	PD3 //17 	
#define CC2420_PIN_CCA		PD5 //19
#define CC2420_PIN_SFD		PD6 //20 

#define VREG				PD2 //16	pulled down


#define CCRST				PB3 //4		Pulled up 

// CC2420 command/register defines
#define CC2420_SNOP             0x00
#define CC2420_SXOSCON          0x01
#define CC2420_STXCAL           0x02
#define CC2420_SRXON            0x03
#define CC2420_STXON            0x04
#define CC2420_STXONCCA         0x05
#define CC2420_SRFOFF           0x06
#define CC2420_SXOSCOFF         0x07
#define CC2420_SFLUSHRX         0x08
#define CC2420_SFLUSHTX         0x09
#define CC2420_SACK             0x0A
#define CC2420_SACKPEND         0x0B
#define CC2420_SRXDEC           0x0C
#define CC2420_STXENC           0x0D
#define CC2420_SAES             0x0E

#define CC2420_MAIN             0x10
#define CC2420_MDMCTRL0         0x11
#define CC2420_MDMCTRL1         0x12
#define CC2420_RSSI             0x13
#define CC2420_SYNCWORD         0x14
#define CC2420_TXCTRL           0x15
#define CC2420_RXCTRL0          0x16
#define CC2420_RXCTRL1          0x17
#define CC2420_FSCTRL           0x18
#define CC2420_SECCTRL0         0x19
#define CC2420_SECCTRL1         0x1A
#define CC2420_BATTMON          0x1B
#define CC2420_IOCFG0           0x1C
#define CC2420_IOCFG1           0x1D
#define CC2420_MANFIDL          0x1E
#define CC2420_MANFIDH          0x1F
#define CC2420_FSMTC            0x20
#define CC2420_MANAND           0x21
#define CC2420_MANOR            0x22
#define CC2420_AGCCTRL          0x23
#define CC2420_AGCTST0          0x24
#define CC2420_AGCTST1          0x25
#define CC2420_AGCTST2          0x26
#define CC2420_FSTST0           0x27
#define CC2420_FSTST1           0x28
#define CC2420_FSTST2           0x29
#define CC2420_FSTST3           0x2A
#define CC2420_RXBPFTST         0x2B
#define CC2420_FSMSTATE         0x2C
#define CC2420_ADCTST           0x2D
#define CC2420_DACTST           0x2E
#define CC2420_TOPTST           0x2F
#define CC2420_RESERVED         0x30

#define CC2420_TXFIFO           0x3E
#define CC2420_RXFIFO           0x3F
//-------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------
// Memory

// Sizes
#define CC2420_RAM_SIZE			368
#define CC2420_FIFO_SIZE		128

// Addresses
#define CC2420RAM_TXFIFO		0x000
#define CC2420RAM_RXFIFO		0x080
#define CC2420RAM_KEY0			0x100
#define CC2420RAM_RXNONCE		0x110
#define CC2420RAM_SABUF			0x120
#define CC2420RAM_KEY1			0x130
#define CC2420RAM_TXNONCE		0x140
#define CC2420RAM_CBCSTATE		0x150
#define CC2420RAM_IEEEADDR		0x160
#define CC2420RAM_PANID			0x168
#define CC2420RAM_SHORTADDR		0x16A
//-------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------
// Status byte
#define CC2420_XOSC16M_STABLE	6
#define CC2420_TX_UNDERFLOW		5
#define CC2420_ENC_BUSY			4
#define CC2420_TX_ACTIVE		3
#define CC2420_LOCK				2
#define CC2420_RSSI_VALID		1
//-------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------
// SECCTRL0
#define CC2420_SECCTRL0_NO_SECURITY         0x0000
#define CC2420_SECCTRL0_CBC_MAC             0x0001
#define CC2420_SECCTRL0_CTR                 0x0002
#define CC2420_SECCTRL0_CCM                 0x0003

#define CC2420_SECCTRL0_SEC_M_IDX           2

#define CC2420_SECCTRL0_RXKEYSEL0           0x0000
#define CC2420_SECCTRL0_RXKEYSEL1           0x0020

#define CC2420_SECCTRL0_TXKEYSEL0           0x0000
#define CC2420_SECCTRL0_TXKEYSEL1           0x0040

#define CC2420_SECCTRL0_SEC_CBC_HEAD        0x0100
#define CC2420_SECCTRL0_RXFIFO_PROTECTION   0x0200


// Constants concerned with the Basic RF packet format

// Packet overhead ((frame control field, sequence number, PAN ID, destination and source) + (footer))
// Note that the length byte itself is not included included in the packet length
#define BASIC_RF_PACKET_OVERHEAD_SIZE   ((2 + 1 + 2 + 2 + 2) + (2))
#define BASIC_RF_MAX_PAYLOAD_SIZE		(127 - BASIC_RF_PACKET_OVERHEAD_SIZE)
#define BASIC_RF_ACK_PACKET_SIZE		5

// The time it takes for the acknowledgment packet to be received after the data packet has been
// transmitted
#define BASIC_RF_ACK_DURATION			(0.5 * 32 * 2 * ((4 + 1) + (1) + (2 + 1) + (2)))
#define BASIC_RF_SYMBOL_DURATION	    (32 * 0.5)

// The length byte
#define BASIC_RF_LENGTH_MASK            0x7F

// Frame control field
#define BASIC_RF_FCF_NOACK              0x8841
#define BASIC_RF_FCF_ACK                0x8861
#define BASIC_RF_FCF_ACK_BM             0x0020
#define BASIC_RF_FCF_BM                 (~BASIC_RF_FCF_ACK_BM)
#define BASIC_RF_ACK_FCF		        0x0002

// Footer
#define BASIC_RF_CRC_OK_BM              0x80

void cc2420ReadFifo(u08* data, u16 nBytes);
void cc2420DumpFifo(u16 nBytes);
void cc2420WriteReg(u08 addr, u16 data);
void cc2420WriteRam(u16 addr, u08* data, u16 nBytes);
void cc2420WriteFifo(u08* data, u16 nBytes);
void cc2420SetChannel(u08 channel);
void cc2420Command(u08 cmd);
u08 cc2420GetStatus(void);
#include "cc2420.c"
#endif
