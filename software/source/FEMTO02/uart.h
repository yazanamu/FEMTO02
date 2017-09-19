#ifndef _AVR_UART_H
#define _AVR_UART_H

#ifndef CLK
#define CLK 16000000UL
#endif

#define UART0_DDR DDRE
#define UART0_PORT PORTE
#define UART0_TXD_PIN (0x1 << 1)
#define UART0_RXD_PIN (0x1 << 0)

#endif