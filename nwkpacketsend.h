#ifndef NWKPACKETSEND_H
#define NWKPACKETSEND_H

#define UP 0x01
#define DOWN 0x02
#define NOTWORKING 0xFF


u08 nwkpacketsend(u08 direction,u16 srcaddr,u16 dstaddr, u08 nod, u16 macdst);

#endif
