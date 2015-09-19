#ifndef _DS1624_H
#define _DS1624_H


void ds1624_config(void);
void ds1624_startconvert(void);
u16 ds1624_readtemp(void);
void readtemp(u08, u08,u08, u08*);
u08 ds1624_check(void);

#endif
