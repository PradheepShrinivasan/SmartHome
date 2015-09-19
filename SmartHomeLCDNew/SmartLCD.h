#ifndef SMARTLCD_H
 #define SMARTLCD_H


#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>

#include <string.h>
#include "global.h"
#include "timer.h"


//display the scheduling time
void schd(u08 ,u08);// (SwitchNo,ButtonNo)

//used to set the set the time for scheduling
void SchTimeSet(u08 , u08);//(ButtonNo,SwitchNo)

//validation of scheduler time
u08 SchTimeChk(u08);//(SwitchNo)

//Naming the switches and rooms
u08 NameSet(u08,u08);//(Category,SwitchNo)

// for displaying the switches with enable and disable options
void swi(u08 ,u08 ,u08 );//(MaxSwitches,ButtonNo,FlagSchedulerVsSwitch)

//display routine for rooms,settings and other such menus
void rmdisp(u08,u08 ,u08 );//(MaximumItemCnt,SwitchNo,PageNo)

//scroll bar 
void scrol(u08 ,u08 );//(MaximunCount,Position)

// to detect a key press
u08 getKey();
u08 getdKey();

// to set the date and time 
void TimeSet();
void pirmotion(void);
//to convert ascii to integer 
int atoi(char *);

//conversion of characters to string
void sprintf(const char *, ...);

//Key Macros
#define KEY_OK    15
#define KEY_BACK  14
#define KEY_UP    13
#define KEY_DOWN  12
#define KEY_STAR  11
#define KEY_HASH  10
#define KEY_9      9
#define KEY_8      8
#define KEY_7      7
#define KEY_6      6
#define KEY_5      5
#define KEY_4      4
#define KEY_3      3
#define KEY_2      2
#define KEY_1      1
#define KEY_0      0
 /*
#define KEY_OK    0
#define KEY_BACK  1
#define KEY_UP    2
#define KEY_DOWN  3
#define KEY_STAR  4
#define KEY_HASH  10
#define KEY_9      9
#define KEY_8      8
#define KEY_7      7
#define KEY_6      6
#define KEY_5      5
#define KEY_4      4
#define KEY_3      3
#define KEY_2      2
#define KEY_1      1
#define KEY_0      6
*/
#endif



