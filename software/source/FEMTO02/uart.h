#ifndef _AVR_UART_H
#define _AVR_UART_H

// 16MHz operation ///////
#define B9600   103
#define B19200  51
#define B38400  25
#define B57600  16

void Init_UART0(unsigned char buad)
{
  UBRR0H = 0;
  UBRR0L = buad; 
  UCSR0A = 0;
  UCSR0C = 0x06;
  UCSR0B = 0x18;
}

int putchar(int ich)     // Use printf
{
   while(!(UCSR0A & 0x20));
   UDR0 = (unsigned char) ich;
   return 0;
}

#endif