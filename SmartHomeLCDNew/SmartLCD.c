/****************************************************************



this  file contains subroutines for handling lcd 
*****************************************************************/


//Array containing the room names
char rmnm[16][6]={"HALL","STORE","KTCHN","BEDRM","DININ","ENTNC"};

char switc[20][7]={"TV","FRDGE","WMCHN","AC","TUBE1","FAN1","TUBE2","TUBE3","FAN2","FAN3"};

//Start up screen


#define T_1 6 //No of Elements in Menu page
#define T_S 4 //No of Elements in Sensors page
#define RNO 6




u08 ri[30],pos=0,ln=0;

//Array for sensors
char CON[][10]={"TEMP","30","LIGHT","50","PRESSURE","40","GLASS","60"};

u08 val[4]={20,30,90,40};

//Array for Menus
char set[][15]={"VIEW STATUS","CTRL PANEL","SCHEDULER","TIME SET","ROOM SET","SWITCH SET"};

//Array for switch names for a particular room
char app[][10]={"FLT","FAN","BLB","TV ","AC ","HTR"};


u08 rd[24];
u08 sch[20][4]={ {0xaf,0xff,0xff,0xfe},{0xa3,0xf5,0x41,0x83},{0x41,0x2f,0xff,0xfe},{0x22,0x55,0xff,0x55} };



char time[]="10:10 A",date[]="10-10-2007";
//To display schedule page
void schd(u08 sno,u08 but)
{
    u08 i;
    char dt[8];
    u08 te[9];
    char wk[][2]={"S","M","T","W","T","F","S"};

    te[0]=((sch[sno][0] & 0xf0)>>4)/10;   //ON time hour 1st digit
    te[1]=((sch[sno][0] & 0xf0)>>4)%10;   //             2nd digit
    i=(((sch[sno][0] & 0x0f)<<2) + ((sch[sno][1] & 0xc0) >> 6));
    te[2]=i/10;                           //ON time seconds 1st digit
    te[3]=i%10;                         //                2nd digit

    te[4]=((sch[sno][2] & 0xf0)>>4)/10;    //off time
    te[5]=((sch[sno][2] & 0xf0)>>4)%10;
    i=(((sch[sno][2] & 0x0f)<<2) + ((sch[sno][3] & 0xc0) >> 6));
    te[6]=i/10;
    te[7]=i%10;

    te[8]=(sch[sno][1] <<4) + ((sch[sno][3]&0x0e));  // Weekly schedule

    dt[0]=te[0]+'0';
    dt[1]=te[1]+'0';
    dt[2]=':';
    dt[3]=te[2]+'0';
    dt[4]=te[3]+'0';
    dt[5]=sch[sno][1] & 0x20 ?'P' : 'A';   // ON time AM/PM
    dt[6]='\0';

    
    LCDBRD();
    LCDSTR("ON : ",1,1,1);
    LCDSTR(dt,7,1,1);


    dt[1]=te[5]+'0';
    dt[0]=te[4]+'0';
    dt[2]=':';
    dt[3]=te[6]+'0';
    dt[4]=te[7]+'0';
    dt[5]=sch[sno][3] & 0x20 ?'P' : 'A';  //OFF time AM/PM
    dt[6]='\0';

    LCDSTR("OFF: ",1,2,1);
    LCDSTR(dt,7,2,1);
    for(i=0;i<7;i++)
        LCDSTR(wk[i],i*2+1,3,0);
    LCDSTR(dt,7,2,1);
    for(i=0;i<7;i++)
        LCDNF(((te[8] << i) & 0x80) !=0 , i*2+1,4,0);
}

//To display all the switches of a particular room
void swi(u08 max,u08 but,u08 ind)
{
    u08 i;
    u08 lmt=4;
    if (max<lmt) 
        lmt=max;

    switch(but)
    {
        case KEY_DOWN:
        if(pos<lmt-1)
            pos++;
        else
        {
            if(ri[lmt-1]==max-1)
                for(i=0,pos=0;i<max;i++)
                    ri[i]=i; 
            else
                for(i=0;i<max;i++)
                    ri[i]=(ri[i]+1)%max;
        }         
        break;  

        case KEY_UP:
            if(pos>0 )  
                pos--;
            else
            {
                if(ri[0]==0)
                    for(i=0,pos=lmt-1;i<max;i++)
                        ri[i]=(max-lmt+i)%max; 
                else
                    for(i=0;i<max;i++)
                    ri[i]=(ri[i]+max-1)%max;
            }
            break;
    }
    LCDCLR();
    for(i=0;i<lmt;i++)
    {
        if(pos==i)
        {
            LCDMEN(switc[ri[i]] ,1,i+1);
            if(ind==1)  
                LCDNF( (sch[ri[i]][3]) & 0x01 ,15,i+1,1);
            else if(ind==0) LCDNF( switc[ri[i]][6] & 1 ,15,i+1,1);

        }  
        else
        {
            LCDSTR(switc[ri[i]],1,i+1,1);
            if(ind==1)
                LCDNF((sch[ri[i]][3]) & 0x01,15,i+1,0);
            else         LCDNF(switc[ri[i]][6] & 1,15,i+1,0);
        }
    }
}

//For displaying many pages
void rmdisp(u08 max,u08 but,u08 pg)
{
    u08 i;
    u08 lmt=4;
    if (max<lmt) lmt=max;

    // PORTB=pos;
     if(but==KEY_DOWN)     //down
     {
        if(pg==2) pos=lmt-1;
         if(pos<lmt-1)     // if highlite pos less than MAX-1 or < 3 rd
            pos++;
        else
        {
           if(ri[lmt-1]==max-1)  // if hi-lite pos is at last
            for(i=0,pos=0;i<max;i++)
                ri[i]=i; 
            else
             for(i=0;i<max;i++) // hi-lite pos is at 4th ( MAX > 4 )
             {
                ri[i]=(ri[i]+1)%max;
                if(pg==2) ri[i]=(ri[i]+1)%max;
            }
        }
    }  
     if(but==KEY_UP)   ///up
     {
        if(pg==2) pos=0;
        if(pos>0 )  
            pos--;
        else
        {
            if(ri[0]==0)
                for(i=0,pos=lmt-1;i<max;i++)
                    ri[i]=(max-lmt+i)%max; 
                else
                    for(i=0;i<max;i++)
                    {
                        ri[i]=(ri[i]+max-1)%max;
                        if(pg==2) ri[i]=(ri[i]+max-1)%max;
                    }
                }
            }  


            LCDCLR();
            for(i=0;i<lmt;i++)
            {
                switch(pg)
                {
                    case 0:  
            if(pos==i)      //Disp hi-lited
                LCDMEN(set[ri[pos]],1,i+1);           
            else            //Disp others
                LCDSTR(set[ri[i]],1,i+1,1);
            break;

            case 1: 
            if(pos==i) 
                LCDMEN(rmnm[ri[i]],1,i+1);
            else
                LCDSTR(rmnm[ri[i]],1,i+1,1);
            break;

            case 2:
            if(ri[i]%2==0)   // Disp names at even place ("SMOKE","TEMP",etc)
                LCDSTR(CON[ri[i]],1,i+1,1);
            else            // DIsp values at odd place ("20","30",etc)
                LCDSTR(CON[ri[i]],10,i+1,1);
            break;

            case 4:
            if(pos==i)
            {
                if(ri[i]!=max-1) 
                    LCDMEN(switc[ri[i]],1,i+1);
                else
                    LCDSTR(switc[ri[i]],1,i+1,1);
                break;
            }
        }
    }

    if(pg==2)
        scrol(max/2,ri[pos]/2);
    else
        scrol(max,ri[pos]);


}

//For displaying the scroll bar and scrolling
void scrol(u08 tot,u08 id)
{
    u08 ch[4]={0,0,0,0};
    int i,sp,x=32/tot;
    sp=3;

    x=32-tot*sp;


    for(i=0;i<32;i++)
    {
        if(i<sp*id || i>=x+sp*id)
            ch[i/8]=ch[i/8]>>1;
        else
            ch[i/8]=(ch[i/8]>>1)+0x80;
    }

    for(i=0;i<4;i++)
    {

        LCDGOTOXY(16,i+1);
        spi_write(0x00);
        spi_write(ch[i]);
        spi_write(0xff^ch[i]);
        spi_write(ch[i]);
     }

 }


//To monitor push button
 u08 getdKey()
 {
    u08 g;
    while( (PIND & 0x1C)==0x1c);
    g=PIND & 0x1C;
   //PORTB=0x00;
    while((PIND & 0x1C)!=0x1c);
    delayms(100);
   //PORTB=0xff;
    sbi(PIND,2);
    sbi(PIND,3);
    sbi(PIND,4);
    if(!(g&0x10)) return(0);
    else if(!(g&0x08)) return(1);
    else if(!(g&0x04)) return(2);
    else return(-1);
}


//To monitor push button
u08 getKey()
{
    u08 g,r,c;


    sbi(DDRC,0);  
    sbi(DDRC,1);
    sbi(DDRC,2);  
    sbi(DDRC,3);

    cbi(DDRC,4);  
    cbi(DDRC,5);
    cbi(DDRC,6);  
    cbi(DDRC,7);  

   while(1)
   {

        sbi(PORTC,3);           //  make all rows = 1
        sbi(PORTC,2);
        sbi(PORTC,1);   
        cbi(PORTC,0);           //test row 1
        _delay_ms(50);
        if ((PINC & 0x10) == 0)         // key 1 is presed
        {
            _delay_ms(50);          //depounce
            rprintf("0");
            while ((PINC & 0x10)==0);       //wait until release the key
            rprintf("0");
            return KEY_3;           //
        }
        if ((PINC & 0x20) == 0)         //key 2 is pressed
        {
            _delay_ms(50);              //depounce
            while ((PINC & 0x20)==0);       //wait until release the key
            rprintf("1");
            return KEY_HASH;
        }
        if ((PINC & 0x40) == 0)         //key 3 is pressed
        {
            _delay_ms(50);              //depounce
            while ((PINC & 0x40)==0);
            rprintf("2");
            return KEY_9;
        }
        if ((PINC & 0x80) == 0)     //key 4 is pressed
        {
            _delay_ms(50);              //depounce
            while ((PINC & 0x80)==0);
            rprintf("3");
            return KEY_6;
        }

        sbi(PORTC,3);
        sbi(PORTC,2);
        sbi(PORTC,0);
        cbi(PORTC,1);               //test row 2
        _delay_ms(50);
        if ((PINC & 0x10) == 0)         // key 1 is presed
        {
            _delay_ms(50);          //depounce
            rprintf("4");
            while ((PINC & 0x10)==0);       //wait until release the key
            rprintf("4");
            return KEY_BACK;            //
        }
        if ((PINC & 0x20) == 0)         //key 2 is pressed
        {
            _delay_ms(50);              //depounce
            while ((PINC & 0x20)==0);       //wait until release the key
            rprintf("5");
            return KEY_OK;
        }
        if ((PINC & 0x40) == 0)         //key 3 is pressed
        {
            _delay_ms(50);              //depounce
            while ((PINC & 0x40)==0);
            rprintf("6");
            return KEY_DOWN;
        }

        if ((PINC & 0x80) == 0)     //key 4 is pressed
        {
            _delay_ms(50);              //depounce
            while ((PINC & 0x80)==0);
            rprintf("7");
            return KEY_UP;
        }

        sbi(PORTC,3);
        sbi(PORTC,0);
        sbi(PORTC,1);
        cbi(PORTC,2);               //test row 3
        _delay_ms(50);
        if ((PINC & 0x10) == 0)         // key 1 is presed
        {
            _delay_ms(50);          //depounce
            while ((PINC & 0x10)==0);       //wait until release the key
            rprintf("8");       
            return KEY_2;           //
        }
        if ((PINC & 0x20) == 0)         //key 2 is pressed
        {
            _delay_ms(50);              //depounce
            while ((PINC & 0x20)==0);       //wait until release the key
            rprintf("9");
            return KEY_0;
        }
        if ((PINC & 0x40) == 0)         //key 3 is pressed
        {
            _delay_ms(50);              //depounce
            while ((PINC & 0x40)==0);
            rprintf("10");
            return KEY_8;
        }

        if ((PINC & 0x80) == 0)     //key 4 is pressed
        {
            _delay_ms(50);              //depounce
            while ((PINC & 0x80)==0);
            rprintf("11");
            return KEY_5;
        }

        sbi(PORTC,2);
        sbi(PORTC,0);
        sbi(PORTC,1);
        cbi(PORTC,3);               //test row 3
        _delay_ms(50);

        if ((PINC & 0x10) == 0)         // key 1 is presed
        {
            _delay_ms(10);          //depounce
            while ((PINC & 0x10)==0);       //wait until release the key
            rprintf("12");
            return KEY_1;           //
        }
        if ((PINC & 0x20) == 0)         //key 2 is pressed
        {
            _delay_ms(50);              //depounce
            while ((PINC & 0x20)==0);       //wait until release the key
            rprintf("13");
            return KEY_STAR;
        }
        if ((PINC & 0x40) == 0)         //key 3 is pressed
        {
            _delay_ms(50);              //depounce
            while ((PINC & 0x40)==0);
            rprintf("14");  
            return KEY_7;
        }

        if ((PINC & 0x80) == 0)     //key 4 is pressed
        {
            _delay_ms(50);              //depounce
            while ((PINC & 0x80)==0);
            rprintf("15");
            return KEY_4;
        }
    }// while (1)
}



void TimeSet()
{
    char th[]="12",tm[]="00",tap=0,i;
    char dd[]="01",dm[]="01",dy[]="2000";
    unsigned char g;
    char ps=-1;
    u08 mt,dt,hr,mn;
    int yr;


    packet.payload[0]=DATE;
    packet.payload[1]=TIME;
       if(nwkpacketsend(UP,0x1235,0x1245,0x02,0x1232))// the time is to be got from co
        rprintf("SUCCESS");
    else
        rprintf("Immediate Destination Error");
    for(i=0;i<10;i++)
        _delay_ms(100);

    if(flag==1)
    {
        rprintf("Interrupted");
        nwkpacketrec();
    }
  //sprintf(date,"%02d-%02d-%02d",packet.payload[0],packet.payload[1],packet.payload[2]);
   // sprintf(time,"%d:%02d %c",packet.payload[4],packet.payload[3],packet.payload[5]?'P':'A');
    sprintf(th,"%02x",packet.payload[4]);
    sprintf(tm,"%02x",packet.payload[3]);
    tap=packet.payload[5]?1:0;

    sprintf(dd,"%02x",packet.payload[0]);
    sprintf(dm,"%02x",packet.payload[1]);
    sprintf(dy,"20%02x",packet.payload[2]);

    LCDCLR();
    LCDBRD();

    LCDSTR("TIME",1,1,1);

    hour1:
    LCDSTR(th,3,2,1);
    LCDSTR(":",5,2,1);

    min1:
    LCDSTR(tm,6,2,1);
    LCDSTR((tap==1)?"P":"A",9,2,1);



    LCDSTR("DATE",1,3,1);
    date1:
    LCDSTR(dd,3,4,1);
    LCDSTR(":",5,4,1);

    month1:
    LCDSTR(dm,6,4,1);
    LCDSTR(":",8,4,1);

    year1:
    LCDSTR(dy,9,4,1);

 // PORTB=ps;
    KeyKey:
    g=getKey();
    if (g==KEY_OK) goto chk;
    else if(g==KEY_BACK) return;
    else if(g==KEY_UP)   { ps=(ps-1+13)%13; goto KeyKey; }
    else if(g==KEY_DOWN) { ps=(ps+1)%13;    goto KeyKey; }
    else if(g<=9)
    {
        ps = (ps + 1) % 13;

        switch(ps)
        {
            case 0:
            th[0] = g + '0';
            goto hour1;
            break;
            case 1:
            th[1] = g + '0';
        //if ( th[0]=='1') th[1]= (g % 3)+'0';
            goto hour1;
            break;
            case 2:
            tm[0] = (g % 6)+ '0';
            goto min1;
            break;
            case 3:
            tm[1] = g+'0';
            goto min1;
            break;
            case 4:
            tap = (tap + 1) % 2;
            goto min1;
            break;
            case 5:
            dd[0] = (g) +'0';
            goto date1;
            break;
            case 6:
            dd[1] = g + '0' ;
            goto date1;
            break;
            case 7:
            dm[0] = g + '0' ;
            goto month1;
            break;
            case 8:
            dm[1] = g + '0' ;
            goto month1;
            break;
            case 9:
            dy[0] = g + '0' ;
            goto year1;
            break;
            case 10:
            dy[1] = g + '0' ;
            goto year1;
            break;
            case 11:
            dy[2] = g + '0' ;
            goto year1;
            break;
            case 12:
            dy[3] = g + '0' ;
            goto year1;
            break;


        }
    }
    else
        goto KeyKey;
    chk:  
    yr=atoi(dy);
    mt=atoi(dm);
    dt=atoi(dd);

    hr=atoi(th);
    mn=atoi(tm);

    if((hr>12) || (mn>60))
    {
        LCDCLR();
        LCDSTR("WRONG TIME",2,2,1);
        getKey();
        TimeSet();
    }
    else if( (  mt==2 && (yr % 4)==0 && dt>29 )  || (  mt==2 && (yr % 4)!=0 && dt>28 )  || 
        (dt>31) || (dt==31 && (mt==4 || mt==6 || mt==9 || mt==11))  ||  (mt>12))
    {
        LCDCLR();
        LCDSTR("WRONG DATE",2,2,1);
        getKey();
        TimeSet();
    }

    yr=atoi(dy);
    mt=atoi(dm);
    dt=atoi(dd);

    hr=atoi(th);
    mn=atoi(tm);


    packet.payload[1]=((dt/10)<<4) | ( dt%10);
    packet.payload[2]=((mt/10)<<4) | ( mt%10);
    packet.payload[3]=(((yr%100)/10)<<4) | ( yr%10);
    packet.payload[5]=((mn/10)<<4) | (mn%10);
    packet.payload[6]=((hr/10)<<4) | (hr%10);
    packet.payload[7]=tap;

    packet.payload[0]=DATECHG;
    packet.payload[4]=TIMECHG;
       if(nwkpacketsend(UP,0x1235,0x1245,0x08,0x1232))// the time is to be got from co
        rprintf("--SUCCESS");
    else
        rprintf("Immediate Destination Error");
    for(i=0;i<10;i++)
        _delay_ms(100);

    if(flag==1)
    {
        rprintf("Interrupted");
        nwkpacketrec();
    }
}

void SchTimeSet(u08 bt,u08 sno)//bt --- button and sno switch number
{
    u08 nh[2],nm[2],nap=0;
    u08 fh[2],fm[2],fap=0;
    u08 t,t1,t2;

    nh[0]=( (sch[sno][0] & 0xf0) >> 4 ) / 10 ;
    nh[1]=( (sch[sno][0] & 0xf0 ) >> 4 ) % 10;

    t=( (sch[sno][0] & 0x0f) << 2 ) | ((sch[sno][1] & 0xc0) >> 6);
    nm[0]=t / 10;
    nm[1]=t % 10;

    nap=(sch[sno][1] & 0x20) ? 1 : 0;

    fh[0]=( (sch[sno][2] & 0xf0) >> 4 ) / 10 ;
    fh[1]=( (sch[sno][2] & 0xf0 ) >> 4 ) % 10;

    t=( (sch[sno][2] & 0x0f) << 2 ) | ((sch[sno][3] & 0xc0) >> 6);
    fm[0]=t / 10;
    fm[1]=t % 10;

    fap=(sch[sno][3] & 0x20) ? 1 : 0;

    t1=sch[sno][1] & 0x0f;
    t2=sch[sno][3] & 0x0f;

  switch(pos)// pos - position of 
  {
    case 0:
    if(bt>1) { pos--; break; }
    case 1:
    nh[pos]=bt;   
    break;
    case 2:
    case 3:
    nm[pos-2]=bt;     
    break;
    case 4:
    nap=bt % 2;
    break;

    case 5:
    if(bt>1) { pos--; break; }
    case 6:
    fh[pos-5]=bt;     
    break;
    case 7:
    case 8:
    fm[pos-7]=bt;     
    break;
    case 9:
    fap=bt % 2;
    break;
    case 10:
    case 11:
    case 12:
    case 13:
    if(bt==KEY_STAR) t1=t1  ^ (0x08 >> (pos-10));
    break;
    case 14:
    case 15:
    case 16:
    if(bt==KEY_STAR)  t2=t2  ^ (0x08 >> (pos-14));
    break;



}

sch[sno][0]= ((( nh[0] * 10 + nh[1] ) << 4) & 0xf0 ) | ( ((nm[0]*10 + nm[1]) >> 2) & 0x0f);
sch[sno][1]= ((( nm[0] * 10 + nm[1] ) << 6 ) & 0xc0) | (nap * 0x20) | t1;

sch[sno][2]= ((( fh[0] * 10 + fh[1] ) << 4 ) & 0xf0) | (((fm[0]*10 + fm[1]) >> 2) & 0x0f);
sch[sno][3]= ((( fm[0] * 10 + fm[1] ) << 6 ) & 0xc0) | (fap * 0x20) | t2;
    // pos++;
}

u08 SchTimeChk(u08 sno)
{
    u08 nh[2],nm[2],nap=0;
    u08 fh[2],fm[2],fap=0;
  u08 t;                                            //ON TIME

  nh[0]=( (sch[sno][0] & 0xf0) >> 4 ) / 10 ;   //hour ist digit
  nh[1]=( (sch[sno][0] & 0xf0 ) >> 4 ) % 10;   //hour 2nd digit 

  t=( (sch[sno][0] & 0x0f) << 2 ) | ((sch[sno][1] & 0xc0) >> 6);
  nm[0]=t / 10;                                //minute 1st digit
  nm[1]=t % 10;                                //minute 2nd digit

  nap=(sch[sno][1] & 0x20) ? 1 : 0;             //am=1 pm=0

                                                 //OFF TIME

  fh[0]=( (sch[sno][2] & 0xf0) >> 4 ) / 10 ;     
  fh[1]=( (sch[sno][2] & 0xf0 ) >> 4 ) % 10;

  t=( (sch[sno][2] & 0x0f) << 2 ) | ((sch[sno][3] & 0xc0) >> 6);
  fm[0]=t / 10;
  fm[1]=t % 10;

  fap=(sch[sno][3] & 0x20) ? 1 : 0;

  if( ( (nh[0] * 10 + nh[1])>12 ) || (nh[0] * 10 + nh[1]==0) || ((nm[0] * 10 + nm[1])>59) 
    || ((fh[0] * 10 + fh[1])>12) || ((fh[0] * 10 + fh[1])==0) ||((fm[0] * 10 + fm[1])>59) )
  {
    LCDCLR();
    LCDSTR("WRONG TIME!!",2,2,1);
    getKey();
    return 1;    
}
return 0;

}

u08 NameSet(u08 category,u08 num)
{
    char *a="NAME0";
    u08 caa[5][2],k,j,i;
    u08 carr[10][4]={ {' ','-','_','\0'},
    {'A','B','C','1'},
    {'D','E','F','2'},
    {'G','H','I','3'},
    {'J','K','L','4'},
    {'M','N','O','5'},
    {'P','Q','R','6'},
    {'S','T','U','7'},
    {'V','W','X','8'},
    {'Y','Z','9','0'} };
    
    
    
    
    LCDCLR();

    if(category==0) a=switc[num];
    else a=rmnm[num-1];

    for(i=0;i<5;i++)
        for(j=0;j<10;j++)
            for(k=0;k<4;k++)
                if(a[i]==carr[j][k])
                {
                    caa[i][0]=j;
                    caa[i][1]=k;
                }
                while(1)
                {
                    LCDSTR(a,2,2,1);
                  //strcpy(switc[pos],a);  
                    k=getKey();
                    // PORTB=pos;
                    if(k>KEY_0 && k<=KEY_9)
                    {
                        if(caa[pos][0]==k)
                            caa[pos][1]=( caa[pos][1] + 1 ) % 4;
                        else 
                            caa[pos][1]=0;
                        caa[pos][0]=k;
                    }
                     //else
                    else if(k==KEY_DOWN && pos<4) pos++;
                    else if(k==KEY_UP && pos>0) pos--;
                    else return 0;
                    //Display the Name
                    for(i=0;i<5;i++)
                        a[i]=carr[ caa[i][0] ][ caa[i][1] ];
                    a[i]='\0'; 
                }

            }

            void pirmotion()
            {
                LCDCLR();
                LCDSTR("THIRUDAN",2,1,1);
                LCDSTR("VIVEK!!!",2,2,1);
                LCDSTR("DETECTED",2,3,1);
//  getkey();

            }
