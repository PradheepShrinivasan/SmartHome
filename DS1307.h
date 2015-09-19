#ifndef _DS1307_H
#define _DS1307_H


void initUart(void);
void i2cMasterTransferNI(u08 deviceAddr, u08 sendlength, u08* senddata, u08 receivelength, u08* receivedata);
void DS1307_settime(unsigned char hh, unsigned char mm, unsigned char ss);
void DS1307_setdate(unsigned char dd, unsigned char mm, unsigned char yy);
unsigned char DS1307_get(unsigned char addr);
void initi2c(void);
u08 ds1307_check(void);
void initI2c(void);

#endif
 
