/////////////////////////////////////////////////
//	 Title		: ioport_128.c
//	 Designer	: Seung-Myung Yang
//	 Date		: 2004.09.07
//	 Copyright	: PULSUS Technology.
//	 Revision	: 
//	 	2004.09.07
//	 		First coded.
//	 	2004.09.09
//	 		bit in/out control is enabled.
/////////////////////////////////////////////////

#include <stdio.h>
#include <inavr.h>


enum PORT16 {PPCA=1,PPCB,PPCC,PPCD};
unsigned char _porta,_portb,_portc,_portd;


/////////////////////////////////////////////////
// 	 This control the ports.
/////////////////////////////////////////////////
void _outport(unsigned char port, char data){
   switch (port){
      case PPCB:
      _portb=data;   PORTB=_portb;   break;
	}

}

/////////////////////////////////////////////////
//	 This read the port and return it.
/////////////////////////////////////////////////
unsigned char _inport(unsigned char port){
unsigned char dd;
   switch (port){
      case PPCB:
      dd=PINB;   break;
   }
   return dd;
}

/////////////////////////////////////////////////
//	 This input the bit-value and reture it.
/////////////////////////////////////////////////
char _inbit(unsigned char port,unsigned char no){
char dd;
dd=0;
   switch(port){
      case PPCB:
         if(PINB&(0x01<<no)) dd=1;
         else dd=0;
      break;
	}//end of port
	
	return dd;
}

/////////////////////////////////////////////////
//	 This output the bit-value and return it.
/////////////////////////////////////////////////
void _outbit(unsigned char port,unsigned char no,char on){
   switch(port){
      case PPCB:
         if(on) {_portb|=(0x01<<no);}
         else   {_portb&=~(0x01<<no);}
                  PORTB=_portb;   break;
	}//end of switch
}

/////////////////////////////////////////////////
// This initialize the ports.
/////////////////////////////////////////////////
void _port_init(unsigned char port,unsigned char data){
// initializing whole ports.
   switch(port){
      case PPCB:   DDRB=data;	break;
	}//end of switch
}

///////////////////////////////////////////////////////////////////////////////
//	This set the bit-support
///////////////////////////////////////////////////////////////////////////////
void _bit_init(unsigned char port,unsigned char no, char on){
   switch(port){
      case PPCB:
         if(on) {DDRB|=(0x01<<no);}
         else   {DDRB&=~(0x01<<no);}
      break;
	}//end of switch
}
