#include <iom128.h>
#include <inavr.h>
#include "uart.h"

int Init_UART0(unsigned long baud)
{
  unsigned int baudrate;
  
  if ((baud%1200)!=0) return -1;
  
  UART0_DDR |=  UART0_TXD_PIN;
  UART0_PORT |= UART0_TXD_PIN;
  UART0_DDR &= ~UART0_RXD_PIN;
  UART0_PORT |= UART0_RXD_PIN;
  
  baudrate = (unsigned int) (CLK / baud / 16.0) - 1;
  
  UBRR0H = baudrate >> 8;
  UBRR0L = baudrate ; 
  UCSR0A = 0;
  UCSR0B = (1<<RXEN0) | (1<<RXCIE0) | (1<<TXEN0) | (0<<UCSZ02);
  UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
  return 0;
}

int putchar(int ich)
{
   while(!(UCSR0A & (1<<UDRE0)));
   UDR0 = (unsigned char) ich;
   return 0;
}

void send_string(char *p)
{
    char    rd;
    while(rd = *p++) putchar((int) rd);
}

void send_integer(unsigned char ch)
{
  char *message = "000";
  
  message[0] = '0'+ (ch/100);
  ch -= (ch/100)*100;
  message[1] = '0'+ (ch/10);
  ch -= (ch/10)*10;
  message[2] = '0'+ ch;
  
  send_string(message);
}

void send_byte2hex(unsigned char value)
{
  char *message = "0x00 ";
  unsigned char i;
  
  for (i=3;i>1;i--) {
    message[i] = '0'+ (value&0x0F);
    if ((message[i])>('0'+9)) message[i]+=7;
    value>>=4;
  }
  
  send_string(message);
}

void send_int2hex(unsigned int value)
{
  char *message = "0x0000 ";
  unsigned char i;

  for (i=5;i>3;i--) {
    message[i] = '0'+ (value&0x0F);
    if ((message[i])>('0'+9)) message[i]+=7;
    value>>=4;
  }
  //message[2] = '0'+ (value>>4);
  //if ((value>>4)>9) message[2]+=7;
  //message[3] = '0'+ (value&0x0F);
  //if ((value&0x0F)>9) message[3]+=7;
  
  send_string(message);
}

void send_long2hex(unsigned long value)
{
  char *message = "0x00000000 ";
  unsigned char i;

  for (i=9;i>3;i--) {
    message[i] = '0'+ (value&0x0F);
    if ((message[i])>('0'+9)) message[i]+=7;
    value>>=4;
  }
  //message[2] = '0'+ (value>>4);
  //if ((value>>4)>9) message[2]+=7;
  //message[3] = '0'+ (value&0x0F);
  //if ((value&0x0F)>9) message[3]+=7;
  
  send_string(message);
}