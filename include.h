#ifndef INCLUDE_H
#define INCLUDE_H


#include <avr/io.h>
#include <avr/interrupt.h>
#include  <util/delay.h>


#include "global.h"

#include "uart.h"
#include "uart.c"

#include "rprintf.h"
#include "rprintf.c"
 
#include "buffer.h"
#include "buffer.c"
 
#include "string.h"

#include "spi.h"
#include "spi.c"

#include "a2d.h"
#include "a2d.c"

#include "i2c.h"
#include "i2c.c"

#include "DS1624.c"
#include "DS1307.c"

#include "cc2420.c"


#include "defines.h"

#include "cc2420init.c"

#include "packetsend.c"
#include "packetrecieve.c"

#include "init.c"

#include "nwkpacketsend.c"
#include "nwkpacketrec.c"

#ifdef MOBILENODE
#include "smartlcd.h"
#include "lcd3310.h"
#include "LCD3310.c"
#include "smartlcd.c"
#include "util/delay.h"
#endif



#endif
