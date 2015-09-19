#include <smartlcd.h>
#include <lcd3310.h>
#include <LCD3310.c>
#include <smartlcd.c>
#include "util/delay.h"
//#define F_CPU 1000000
#define CYCLES_PER_US ((F_CPU+500000)/1000000)

int main(void)
{
    u08 i,j,g,sel,rm;

    //Key 
    PORTD=0x00;
    DDRD=0xff;

    sbi(DDRD,5);
    cbi(DDRD,2);
    cbi(DDRD,3);
    cbi(DDRD,4);
    sbi(PORTD,6);
    cbi(PORTD,5);


    cbi(PORTA,RST);
    LCDINI();

    Page0:
    LCDGOTOXY(1,0);

    /// draws smart home picture
    for(i=0;i<=62;i++)
        for(j=0;j<=7;j++)
            spi_write(Welup[i][j]);

    for(i=0;i<10;i++)
        _delay_ms(200);

     cbi(PORTA,RST);

     Mainmenu:
    sbi(PORTA,RST);
    LCDBRD();/// draws border
    LCDCLR();// clear lcd


    //displays date and time
    LCDSTR(date,1,1,0);
    LCDSTR(time,8,2,0);
    LCDMEN("H O M E",4,4);

    //waiting for key
    Page:
    if(getKey()!=KEY_OK) goto Page;

//MENU Page 0 // menu list 
    Menulist:
    pos=0;             //pos=highlighted position //all  room names and total no of rooms
    LCDCLR();
    for(i=0;i<T_1;i++)
       ri[i]=i;   
    rmdisp(T_1,0,0);
   
    while(1)
    {
        g=getKey();
        if(g==KEY_DOWN || g==KEY_UP)
            rmdisp(T_1,g,0);
        else if (g==KEY_BACK)
        goto Mainmenu;
        else if(g==KEY_OK) 
        break;
    }   


sel=ri[pos];

   if ( sel == 3)// set time - time syncronize with central server(cordinator) date time()
   {
       TimeSet();
       goto Menulist;
   }
//   PORTB=sel;
/////////////////////////////////////////////////////////////////////////////////////////////
// 
//ROOMS Disp page 1 pressing of ok
   Roomlist:
   pos=0;
//Page3:
   LCDCLR();

   for(i=0;i<RNO;i++)
    ri[i]=i;   
rmdisp(RNO,0,1);
while(1)
{
    g=getKey();
    if(g==KEY_UP || g==KEY_DOWN)
      rmdisp(RNO,g,1);
  else if(g==KEY_BACK)
      goto Menulist;
  else if(g==KEY_OK) 
   break;
//   delayms(7000);
}
rm=ri[pos]+1;

// selection based values ie room choice->temp ,pressure,control etc
//SENSOR Status Page 2 

pos=0;
if(sel==0) rmdisp(8,0,2);
while(sel==0)
{
 g=getKey();
 if(g==KEY_DOWN || g==KEY_UP)
   rmdisp(10,g,2);
else if (g==KEY_BACK)
    goto Roomlist;
else if(g==KEY_OK) goto Roomlist;
 //      delayms(7000);
}



pos=0;   //////////////////////////////////////control page switch display
if(sel==1)
{

  j=20;
  for(i=0;i<j;i++)
     ri[i]=i;
 swi(j,0,0);
 while(1)
 {
     g=getKey();
     if(g==KEY_UP || g==KEY_DOWN)
       swi(j,g,0);
   else if(g==KEY_STAR) 
   { 
       switc[ri[pos]][6] = switc[ri[pos]][6] ^ 0xff; 
       swi(j,12,0);
   }
   else if(g==KEY_OK)  goto Roomlist;
}
goto Page0;
}
else if(sel==4)
{
   pos=0;
   NameSet(1,rm);
   goto Menulist;
}
else if(sel==5)
{
  j=0;
  for(i=0;i<20;i++)
  {
     rd[j++]=i;
 }

 for(i=0;i<=j;i++)
     ri[i]=i;
 rmdisp(j+1,0,4);
 while(1)
 {
     g=getKey();
     if(g==KEY_DOWN || g==KEY_UP)
       rmdisp(j+1,g,4);
   else if(g==KEY_OK) break;
}

pos=0;
NameSet(0,ri[pos]);
goto Menulist;

}
else if(sel==2)
{
  SchSwitch:

  for(i=0;i<10;i++)
     ri[i]=i;
 swi(10,0,1);
 while(1)
 {
     g=getKey();
     if(g==KEY_DOWN || g==KEY_UP)
       swi(10,g,1);
   else if (g==KEY_STAR)
   {
       sch[ri[pos]][3]  =sch[ri[pos]][3] ^ 0x01;
       swi(10,19,1);
   }
   else if (g==KEY_BACK)
    goto Roomlist;
else if(g==KEY_OK) break;
}
j=ri[pos];
SchDisp:
LCDCLR();
schd(j,0);
pos=0;
while(1)
{
   g=getKey();
   if(g<=KEY_9 || g==KEY_STAR) 
      SchTimeSet(g,j);
  else if(g==KEY_DOWN && pos <16) pos =(pos +1);
  else if(g==KEY_UP && pos>0)   pos = (pos-1); 
  schd(j,0);
  if (g==KEY_BACK)
      goto SchSwitch;
  else
   if(g==KEY_OK)
   {
     g=SchTimeChk(j);
     if(g==1) goto SchDisp;
     else    goto Mainmenu;
 }
}

}

cbi(PORTA,4);

while(1);
return 0;
}


