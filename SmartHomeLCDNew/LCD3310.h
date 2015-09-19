void delaym(int );

//millisec delay
void delayms(int );


//Fn for datawrite into display
void spi_write(u08 );


//Fn to print a string in location (x,y) 
void LCDSTR(char *,u08  ,u08 ,u08 );

//Fn to indicate ON/OFF , YES/NO , En/Dis
//fl --> invert color
void LCDNF(u08 ,u08 ,u08 ,u08 );


//Fn to print the integers directly
//st--> invert color
void LCDDEC(u08 ,u08 );

//Fn to print a u08 
void LCDDIR(u08 );


//Set the position of cursor
void LCDGOTOXY(u08 ,u08 );


//NO NEED
void LCDEN(char *,u08 ,u08 );


//Fn to display highlighted menu
void LCDMEN(char *,u08 ,u08 );


//Initialize LCD
void LCDINI();


//To print borders alone
void LCDBRD();

