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
#include <iom128.h>
#include <inavr.h>

//#include "Define.c"

unsigned char _porta,_portb,_portc,_portd,_porte,_portf,_portg;


/////////////////////////////////////////////////
// 	 This control the ports.
/////////////////////////////////////////////////
void _outport(unsigned char port, char data){
   switch (port){
      case PPCA:
      _porta=data;   PORTA=_porta;   break;
      case PPCB:
      _portb=data;   PORTB=_portb;   break;
	  case PPCC:
      _portc=data;   PORTC=_portc;   break;
      case PPCD:
      _portd=data;   PORTD=_portd;   break;
      case PPCE:
      _porte=data;   PORTE=_porte;   break;
      case PPCF:
      _portf=data;   PORTF=_portf;   break;
      case PPCG:
      _portg=data;   PORTG=_portg;   break;
	}

}

/////////////////////////////////////////////////
//	 This read the port and return it.
/////////////////////////////////////////////////
unsigned char _inport(unsigned char port){
unsigned char dd;
   switch (port){
      case PPCA:
      dd=PINA;   break;
      case PPCB:
      dd=PINB;   break;
      case PPCC:
      dd=PINC;   break;
      case PPCD:
      dd=PIND;   break;
      case PPCE:
      dd=PINE;   break;
      case PPCF:
      dd=PINF;   break;
      case PPCG:
      dd=PING;   break;
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
      case PPCA:
         if(PINA&(0x01<<no)) dd=1;
         else dd=0;
      break;
      case PPCB:
         if(PINB&(0x01<<no)) dd=1;
         else dd=0;
      break;
      case PPCC:
         if(PINC&(0x01<<no)) dd=1;
         else dd=0;
      break;
      case PPCD:
         if(PIND&(0x01<<no)) dd=1;
         else dd=0;
      break;
      case PPCE:
         if(PINE&(0x01<<no)) dd=1;
         else dd=0;
      break;
      case PPCF:
         if(PINF&(0x01<<no)) dd=1;
         else dd=0;
      break;
      case PPCG:
         if(PING&(0x01<<no)) dd=1;
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
      case PPCA:
         if(on) {_porta|=(0x01<<no);} //HIGH
		 else   {_porta&=~(0x01<<no);}
                  PORTA=_porta;   break;
      case PPCB:
         if(on) {_portb|=(0x01<<no);}
         else   {_portb&=~(0x01<<no);}
                  PORTB=_portb;   break;
      case PPCC:
         if(on) {_portc|=(0x01<<no);}
         else   {_portc&=~(0x01<<no);}
                  PORTC=_portc;   break;
      case PPCD:
         if(on) {_portd|=(0x01<<no);}
         else   {_portd&=~(0x01<<no);}
			      PORTD=_portd;   break;
      case PPCE:
         if(on) {_porte|=(0x01<<no);}
         else   {_porte&=~(0x01<<no);}
                  PORTE=_porte;   break;
      case PPCF:
         if(on) {_portf|=(0x01<<no);}
         else   {_portf&=~(0x01<<no);}
                  PORTF=_portf;   break;
      case PPCG:
         if(on) {_portg|=(0x01<<no);}
         else   {_portg&=~(0x01<<no);}
                  PORTG=_portg;   break;
	}//end of switch
}

/////////////////////////////////////////////////
// This initialize the ports.
/////////////////////////////////////////////////
void _port_init(unsigned char port,unsigned char data){
// initializing whole ports.
   switch(port){
      case PPCA:   DDRA=data;   break;
      case PPCB:   DDRB=data;	break;
      case PPCC:   DDRC=data;	break;
      case PPCD:   DDRD=data;	break;
      case PPCE:   DDRE=data;	break;
      case PPCF:   DDRF=data;	break;
      case PPCG:   DDRG=data;	break;
	}//end of switch
}

///////////////////////////////////////////////////////////////////////////////
//	This set the bit-support
///////////////////////////////////////////////////////////////////////////////
void _bit_init(unsigned char port,unsigned char no, char on){
   switch(port){
      case PPCA:
         if(on) {DDRA|=(0x01<<no);} //HIGH
         else   {DDRA&=~(0x01<<no);} //LOW
      break;
      case PPCB:
         if(on) {DDRB|=(0x01<<no);}
         else   {DDRB&=~(0x01<<no);}
      break;
      case PPCC:
         if(on) {DDRC|=(0x01<<no);}
         else   {DDRC&=~(0x01<<no);}
      break;
      case PPCD:
         if(on) {DDRD|=(0x01<<no);}
         else   {DDRD&=~(0x01<<no);}
      break;
      case PPCE:
         if(on) {DDRE|=(0x01<<no);}
         else   {DDRE&=~(0x01<<no);}
      break;
      case PPCF:
         if(on) {DDRF|=(0x01<<no);}
         else   {DDRF&=~(0x01<<no);}
	  break;
      case PPCG:
         if(on) {DDRG|=(0x01<<no);}
         else   {DDRG&=~(0x01<<no);}
      break;
	}//end of switch
}
