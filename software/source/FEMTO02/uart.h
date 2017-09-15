#ifndef _AVR_UART_H
#define _AVR_UART_H

#include <iom128.h>
#include <inavr.h>

#ifndef CLK
#define CLK 16000000
#endif

#define UART0_DDR DDRE
#define UART0_PORT PORTE
#define UART0_TXD_PIN (0x1 << 1)
#define UART0_RXD_PIN (0x1 << 0)

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
  //UCSR0B = 0x18;
  UCSR0B = (1<<RXEN0) | (1<<RXCIE0) | (1<<TXEN0) | (0<<UCSZ02);
  //UCSR0C = 0x06;
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

#endif