/****************************************************************************
 **             - iom128.h -
 **
 **     This file declares the internal register addresses for ATmega128.
 **
 **     Used with iccAVR and aAVR.
 **
 **     Copyright IAR Systems 2001. All rights reserved.
 **
 **     $Name: V3_10B V3_10A_EV V3_10A_PRO V3_10A $
 **
 ***************************************************************************/

#include "iomacro.h"

#if TID_GUARD(3)
#error This file should only be compiled with iccavr or aavr whith processor option -v3
#endif /* TID_GUARD(3) */

/* Include the SFR part if this file has not been included before,
 * OR this file is included by the assembler (SFRs must be defined in
 * each assembler module). */
#if !defined(__IOM128_H) || defined(__IAR_SYSTEMS_ASM__)

#pragma language=extended

/*==========================*/
/* Predefined SFR Addresses */
/*==========================*/

/****************************************************************************
 * An example showing the SFR_B() macro call, 
 * the expanded result and usage of this result:
 *
 * SFR_B(AVR,   0x1F) Expands to:
 * __io union {
 *             unsigned char AVR;                 // The sfrb as 1 byte
 *             struct {                           // The sfrb as 8 bits
 *                     unsigned char AVR_Bit0:1,
 *                                   AVR_Bit1:1,
 *                                   AVR_Bit2:1,
 *                                   AVR_Bit3:1,
 *                                   AVR_Bit4:1,
 *                                   AVR_Bit5:1,
 *                                   AVR_Bit6:1,
 *                                   AVR_Bit7:1;
 *                    };
 *            } @ 0x1F;
 * Examples of how to use the expanded result:
 * AVR |= (1<<5);
 * or like this:
 * AVR_Bit5 = 1;
 ***************************************************************************/

SFR_B(PINF,   0x00)     /* Input Pins, Port F */
SFR_B(PINE,   0x01)     /* Input Pins, Port E */
SFR_B(DDRE,   0x02)     /* Data Direction Register, Port E */
SFR_B(PORTE,  0x03)     /* Data Register, Port E */
SFR_W(ADC,    0x04)     /* ADC Data register */
SFR_B(ADCSR,  0x06)     /* ADC Control and Status Register */
SFR_B(ADMUX,  0x07)     /* ADC Multiplexer Selection Register */
SFR_B(ACSR,   0x08)     /* Analog Comparator Control and Status Register */
SFR_B(UBRR0L, 0x09)     /* USART0 Baud Rate Register Low*/
SFR_B(UCSR0B, 0x0A)     /* USART0 Control and Status Register B */
SFR_B(UCSR0A, 0x0B)     /* USART0 Control and Status Register A */
SFR_B(UDR0,   0x0C)     /* USART0 I/O Data Register */
SFR_B(SPCR,   0x0D)     /* SPI Control Register */
SFR_B(SPSR,   0x0E)     /* SPI Status Register */
SFR_B(SPDR,   0x0F)     /* SPI I/O Data Register */
SFR_B(PIND,   0x10)     /* Input Pins, Port D */
SFR_B(DDRD,   0x11)     /* Data Direction Register, Port D */
SFR_B(PORTD,  0x12)     /* Data Register, Port D */
SFR_B(PINC,   0x13)     /* Input Pins, Port C */
SFR_B(DDRC,   0x14)     /* Data Direction Register, Port C */
SFR_B(PORTC,  0x15)     /* Data Register, Port C */
SFR_B(PINB,   0x16)     /* Input Pins, Port B */
SFR_B(DDRB,   0x17)     /* Data Direction Register, Port B */
SFR_B(PORTB,  0x18)     /* Data Register, Port B */
SFR_B(PINA,   0x19)     /* Input Pins, Port A */
SFR_B(DDRA,   0x1A)     /* Data Direction Register, Port A */
SFR_B(PORTA,  0x1B)     /* Data Register, Port A */
SFR_B(EECR,   0x1C)     /* EEPROM Control Register */
SFR_B(EEDR,   0x1D)     /* EEPROM Data Register */
SFR_W(EEAR,   0x1E)     /* EEPROM Address Register */
SFR_B(SFIOR,  0x20)     /* Special Function I/O Register */
SFR_B(WDTCR,  0x21)     /* Watchdog Timer Control Register */
SFR_B(OCDR,   0x22)     /* On-Chip Debug Register */
SFR_B(OCR2,   0x23)     /* Timer/Counter 2 Output Compare Register */
SFR_B(TCNT2,  0x24)     /* Timer/Counter 2 */
SFR_B(TCCR2,  0x25)     /* Timer/Counter 2 Control Register */
SFR_W(ICR1,   0x26)     /* Timer/Counter 1 Input Capture Register */
SFR_W(OCR1B,  0x28)     /* Timer/Counter 1 Output Compare Register B */
SFR_W(OCR1A,  0x2A)     /* Timer/Counter 1 Output Compare Register A */
SFR_W(TCNT1,  0x2C)     /* Timer/Counter 1 Register */
SFR_B(TCCR1B, 0x2E)     /* Timer/Counter 1 Control Register B */
SFR_B(TCCR1A, 0x2F)     /* Timer/Counter 1 Control Register A */
SFR_B(ASSR,   0x30)     /* Asynchronous mode Status Register */
SFR_B(OCR0,   0x31)     /* Timer/Counter 0 Output Compare Register */
SFR_B(TCNT0,  0x32)     /* Timer/Counter 0 */
SFR_B(TCCR0,  0x33)     /* Timer/Counter 0 Control Register */
SFR_B(MCUCSR, 0x34)     /* MCU general Control and Status Register */
SFR_B(MCUCR,  0x35)     /* MCU general Control Register */
SFR_B(TIFR,   0x36)     /* Timer/Counter Interrupt Flag Register */
SFR_B(TIMSK,  0x37)     /* Timer/Counter Interrupt Mask Register */
SFR_B(EIFR,   0x38)     /* External Interrupt Flag Register */
SFR_B(EIMSK,  0x39)     /* External Interrupt Mask Register */
SFR_B(EICRB,  0x3A)     /* External Interrupt Control Register B */
SFR_B(RAMPZ,  0x3B)     /* RAM Page Z Select Register */
SFR_B(XDIV,   0x3C)     /* XTAL Divide Control Register */
SFR_W(SP,     0x3D)     /* Stack Pointer */
SFR_B(SREG,   0x3F)     /* Status Register */

/* Extended I/O space */
SFR_B(DDRF,   0x61)     /* Data Direction Register, Port F */
SFR_B(PORTF,  0x62)     /* Data Register, Port F */
SFR_B(PING,   0x63)     /* Input Pins, Port G */
SFR_B(DDRG,   0x64)     /* Data Direction Register, Port G */
SFR_B(PORTG,  0x65)     /* Data Register, Port G */
SFR_B(SPMCSR, 0x68)     /* Store Program Memory Control and Status Register */
SFR_B(EICRA,  0x6A)     /* External Interrupt Control Register A */
SFR_B(XMCRB,  0x6C)     /* External Memory Control Register B */
SFR_B(XMCRA,  0x6D)     /* External Memory Control Register A */
SFR_B(OSCCAL, 0x6F)     /* Oscillator Calibration Register */
SFR_B(TWBR,   0x70)     /* TWI Bit Rate Register */
SFR_B(TWSR,   0X71)     /* TWI Status Register */
SFR_B(TWAR,   0x72)     /* TWI Address Register */
SFR_B(TWDR,   0x73)     /* TWI Data Register */
SFR_B(TWCR,   0x74)     /* TWI Control Register */
SFR_W(OCR1C,  0x78)     /* Timer/Counter 1 Output Compare Register C Low */
SFR_B(TCCR1C, 0x7A)     /* Timer/Counter 1 Control Register C */
SFR_B(ETIFR,  0x7C)     /* Extended Timer/Counter Interrupt Flag Register */
SFR_B(ETIMSK, 0x7D)     /* Extended Timer/Counter Interrupt Mask Register */
SFR_W(ICR3,   0x80)     /* Timer/Counter 3 Input Capture Register Low */
SFR_W(OCR3C,  0x82)     /* Timer/Counter 3 Output Compare Register C Low */
SFR_W(OCR3B,  0x84)     /* Timer/Counter 3 Output Compare Register B Low */
SFR_W(OCR3A,  0x86)     /* Timer/Counter 3 Output Compare Register A Low */
SFR_W(TCNT3, 0x88)      /* Timer/Counter 3 Register Low */
SFR_B(TCCR3B, 0x8A)     /* Timer/Counter 3 Control Register B */
SFR_B(TCCR3A, 0x8B)     /* Timer/Counter 3 Control Register A */
SFR_B(TCCR3C, 0x8C)     /* Timer/Counter 3 Control Register C */

SFR_B(UBRR0H, 0x90)     /* USART0 Baud Rate Register High*/

SFR_B(UCSR0C, 0x95)     /* USART0 Control and Status Register C */
SFR_B(UBRR1H, 0x98)     /* USART1 Baud Rate Register High */
SFR_B(UBRR1L, 0x99)     /* USART1 Baud Rate Register Low */
SFR_B(UCSR1B, 0x9A)     /* USART1 Control and Status Register B */
SFR_B(UCSR1A, 0x9B)     /* USART1 Control and Status Register A */
SFR_B(UDR1,   0x9C)     /* USART1 I/O Data Register */
SFR_B(UCSR1C, 0x9D)     /* USART1 Control and Status Register C */

#ifndef __IOM128_H
#define __IOM128_H


/* SFRs are local in assembler modules (so this file may need to be */
/* included in more than one module in the same source file), */
/* but #defines must only be made once per source file. */

/*==============================*/
/* Interrupt Vector Definitions */
/*==============================*/

/* NB! vectors are specified as byte addresses */

#define    RESET_vect           (0x00)
#define    INT0_vect            (0x04)
#define    INT1_vect            (0x08)
#define    INT2_vect            (0x0C)
#define    INT3_vect            (0x10)
#define    INT4_vect            (0x14)
#define    INT5_vect            (0x18)
#define    INT6_vect            (0x1C)
#define    INT7_vect            (0x20)
#define    TIMER2_COMP_vect     (0x24)
#define    TIMER2_OVF_vect      (0x28)
#define    TIMER1_CAPT_vect     (0x2C)
#define    TIMER1_COMPA_vect    (0x30)
#define    TIMER1_COMPB_vect    (0x34)
#define    TIMER1_OVF_vect      (0x38)
#define    TIMER0_COMP_vect     (0x3C)
#define    TIMER0_OVF_vect      (0x40)
#define    SPI_STC_vect         (0x44)
#define    USART0_RXC_vect      (0x48)
#define    USART0_UDRE_vect     (0x4C)
#define    USART0_TXC_vect      (0x50)
#define    ADC_vect             (0x54)
#define    EE_RDY_vect          (0x58)
#define    ANA_COMP_vect        (0x5C)
#define    TIMER1_COMPC_vect    (0x60)
#define    TIMER3_CAPT_vect     (0x64)
#define    TIMER3_COMPA_vect    (0x68)
#define    TIMER3_COMPB_vect    (0x6C)
#define    TIMER3_COMPC_vect    (0x70)
#define    TIMER3_OVF_vect      (0x74)
#define    USART1_RXC_vect      (0x78)
#define    USART1_UDRE_vect     (0x7C)
#define    USART1_TXC_vect      (0x80)
#define    TWI_vect             (0x84)
#define    SPM_RDY_vect         (0x88)

#ifdef __IAR_SYSTEMS_ASM__   
#ifndef ENABLE_BIT_DEFINITIONS
#define  ENABLE_BIT_DEFINITIONS
#endif /* ENABLE_BIT_DEFINITIONS */
#endif /* __IAR_SYSTEMS_ASM__ */

#ifdef ENABLE_BIT_DEFINITIONS


/* Bit definitions for use with the IAR Assembler   
   The Register Bit names are represented by their bit number (0-7).
*/

/* USART1 Control and Status Register C */
#define    UMSEL1   6
#define    UPM11    5
#define    UPM10    4
#define    USBS1    3
#define    UCSZ11   2
#define    UCSZ10   1
#define    UCPOL1   0

/* USART1 Control and Status Register A */
#define    RXC1     7
#define    TXC1     6
#define    UDRE1    5
#define    FE1      4
#define    DOR1     3
#define    UPE1     2
#define    U2X1     1
#define    MPCM1    0

/* USART1 Control and Status Register B */
#define    RXCIE1   7
#define    TXCIE1   6
#define    UDRIE1   5
#define    RXEN1    4
#define    TXEN1    3
#define    UCSZ12   2
#define    RXB81    1
#define    TXB81    0

/* USART0 Control and Status Register C */
#define    UMSEL0   6
#define    UPM01    5
#define    UPM00    4
#define    USBS0    3
#define    UCSZ01   2
#define    UCSZ00   1
#define    UCPOL0   0

/* USART0 Control and Status Register A */
#define    RXC0     7
#define    TXC0     6
#define    UDRE0    5
#define    FE0      4
#define    DOR0     3
#define    UPE0     2
#define    U2X0     1
#define    MPCM0    0

/* USART0 Control and Status Register B */
#define    RXCIE0   7
#define    TXCIE0   6
#define    UDRIE0   5
#define    RXEN0    4
#define    TXEN0    3
#define    UCSZ02   2
#define    RXB80    1
#define    TXB80    0

/* Timer/Counter 3 Control Register C */
#define    FOC3A    7
#define    FOC3B    6
#define    FOC3C    5

/* Timer/Counter 3 Control Register A */
#define    COM3A1   7
#define    COM3A0   6
#define    COM3B1   5
#define    COM3B0   4
#define    COM3C1   3
#define    COM3C0   2
#define    WGM31    1
#define    WGM30    0

/* Timer/Counter 3 Control Register B */
#define    ICNC3    7
#define    ICES3    6
#define    WGM33    4
#define    WGM32    3
#define    CS32     2
#define    CS31     1
#define    CS30     0

/* Extended Timer/Counter Interrupt Mask Register */
#define    TICIE3   5
#define    OCIE3A   4
#define    OCIE3B   3
#define    TOIE3    2
#define    OCIE3C   1
#define    OCIE1C   0

/* Extended Timer/Counter Interrupt Flag Register */
#define    ICF3     5
#define    OCF3A    4
#define    OCF3B    3
#define    TOV3     2
#define    OCF3C    1
#define    OCF1C    0

/* TWI Control Register */
#define    TWINT    7
#define    TWEA     6
#define    TWSTA    5
#define    TWSTO    4
#define    TWWC     3
#define    TWEN     2
#define    TWIE     0

/* TWI (slave) Address Register */
#define    TWA6     7
#define    TWA5     6
#define    TWA4     5
#define    TWA3     4
#define    TWA2     3
#define    TWA1     2
#define    TWA0     1
#define    TWGCE    0

/* TWI Status Register */
#define    TWS7     7
#define    TWS6     6
#define    TWS5     5
#define    TWS4     4
#define    TWS3     3
#define    TWPS1    1
#define    TWPS0    0

/* External Memory Control Register A */
#define    SRL2     6
#define    SRL1     5
#define    SRL0     4
#define    SRW01    3
#define    SRW00    2
#define    SRW11    1

/* External Memory Control Register B */
#define    XMBK     7 
#define    XMM2     2
#define    XMM1     1
#define    XMM0     0

/* External Interrupt Control Register A */
#define    ISC31    7
#define    ISC30    6
#define    ISC21    5
#define    ISC20    4
#define    ISC11    3
#define    ISC10    2
#define    ISC01    1
#define    ISC00    0

/* Store Program Memory Control and Status Register */
#define    SPMIE    7
#define    RWWSB    6
#define    RWWSRE   4
#define    BLBSET   3
#define    PGWRT    2
#define    PGERS    1
#define    SPMEN    0

/* Data Register, Port G */
#define    PG4      4
#define    PG3      3
#define    PG2      2
#define    PG1      1
#define    PG0      0

 /* Data Register, Port G */
#define    PORTG4   4
#define    PORTG3   3
#define    PORTG2   2
#define    PORTG1   1
#define    PORTG0   0

/* Data Direction Register, Port G */
#define    DDG4     4
#define    DDG3     3
#define    DDG2     2
#define    DDG1     1
#define    DDG0     0

/* Input Pins, Port G */
#define    PING4    4
#define    PING3    3
#define    PING2    2
#define    PING1    1
#define    PING0    0

/* Data Register, Port F */
#define    PF7      7
#define    PF6      6
#define    PF5      5
#define    PF4      4
#define    PF3      3
#define    PF2      2
#define    PF1      1
#define    PF0      0
 
/* Data Register, Port F */
#define    PORTF7   7
#define    PORTF6   6
#define    PORTF5   5
#define    PORTF4   4
#define    PORTF3   3
#define    PORTF2   2
#define    PORTF1   1
#define    PORTF0   0
 
/* Data Direction Register, Port F */
#define    DDF7     7
#define    DDF6     6
#define    DDF5     5
#define    DDF4     4
#define    DDF3     3
#define    DDF2     2
#define    DDF1     1
#define    DDF0     0
 
/* Input Pins, Port F */
#define    PINF7    7
#define    PINF6    6
#define    PINF5    5
#define    PINF4    4
#define    PINF3    3
#define    PINF2    2
#define    PINF1    1
#define    PINF0    0

/* Stack Pointer High */
#define    SP15     7
#define    SP14     6
#define    SP13     5
#define    SP12     4
#define    SP11     3
#define    SP10     2
#define    SP9      1
#define    SP8      0

/* Stack Pointer Low */
#define    SP7      7
#define    SP6      6
#define    SP5      5
#define    SP4      4
#define    SP3      3
#define    SP2      2
#define    SP1      1
#define    SP0      0

/* XTAL Divide Control Register */
#define    XDIVEN   7
#define    XDIV6    6
#define    XDIV5    5
#define    XDIV4    4
#define    XDIV3    3
#define    XDIV2    2
#define    XDIV1    1
#define    XDIV0    0

/* RAM Page Z Select Register */
#define    RAMPZ0   0

/* External Interrupt Control Register B */
#define    ISC71    7
#define    ISC70    6
#define    ISC61    5
#define    ISC60    4
#define    ISC51    3
#define    ISC50    2
#define    ISC41    1
#define    ISC40    0

/* External Interrupt Mask Register */
#define    INT7     7
#define    INT6     6
#define    INT5     5
#define    INT4     4
#define    INT3     3
#define    INT2     2
#define    INT1     1
#define    INT0     0

/* External Interrupt Flag Register */
#define    INTF7    7
#define    INTF6    6
#define    INTF5    5
#define    INTF4    4
#define    INTF3    3
#define    INTF2    2
#define    INTF1    1
#define    INTF0    0

/* Timer/Counter Interrupt Mask Register */
#define    OCIE2    7
#define    TOIE2    6
#define    TICIE1   5
#define    OCIE1A   4
#define    OCIE1B   3
#define    TOIE1    2
#define    OCIE0    1
#define    TOIE0    0

/* Timer/Counter Interrupt Flag Register */
#define    OCF2     7
#define    TOV2     6
#define    ICF1     5
#define    OCF1A    4
#define    OCF1B    3
#define    TOV1     2
#define    OCF0     1
#define    TOV0     0


/* MCU general Control Register */
#define    SRE      7
#define    SRW10    6
#define    SE       5
#define    SM1      4
#define    SM0      3
#define    SM2      2
#define    IVSEL    1
#define    IVCE     0

/* MCU general Control and Status Register */
#define    JTD      7
#define    JTRF     4
#define    WDRF     3
#define    BORF     2
#define    EXTRF    1
#define    PORF     0

/* Timer/Counter 0 Control Register */
#define    FOC0     7
#define    WGM00    6
#define    COM01    5
#define    COM00    4
#define    WGM01    3
#define    CS02     2
#define    CS01     1
#define    CS00     0

/* Asynchronous mode Status Register */
#define    AS0      3
#define    TCN0UB   2
#define    OCR0UB   1
#define    TCR0UB   0

/* Timer/Counter 1 Control Register C */
#define    FOC1A    7
#define    FOC1B    6
#define    FOC1C    5

/* Timer/Counter 1 Control Register A */
#define    COM1A1   7
#define    COM1A0   6
#define    COM1B1   5
#define    COM1B0   4
#define    COM1C1   3
#define    COM1C0   2
#define    WGM11    1
#define    WGM10    0

/* Timer/Counter 1 Control Register B */
#define    ICNC1    7
#define    ICES1    6
#define    WGM13    4
#define    WGM12    3
#define    CS12     2
#define    CS11     1
#define    CS10     0

/* Timer/Counter 2 Control Register */
#define    FOC2     7
#define    WGM20    6
#define    COM21    5
#define    COM20    4
#define    WGM21    3
#define    CS22     2
#define    CS21     1
#define    CS20     0

/* On-Chip Debug Register */
#define    IDRD     7
#define    OCDR6    6
#define    OCDR5    5
#define    OCDR4    4
#define    OCDR3    3
#define    OCDR2    2
#define    OCDR1    1
#define    OCDR0    0

/* Watchdog Timer Control Register */
#define    WDCE     4
#define    WDE      3
#define    WDP2     2
#define    WDP1     1
#define    WDP0     0

/* Special Function I/O Register */
#define    TSM      7
#define    ADHSM    4
#define    ACME     3
#define    PUD      2
#define    PSR0     1
#define    PSR321   0

/* EEPROM Control Register */
#define    EERIE    3
#define    EEMWE    2
#define    EEWE     1
#define    EERE     0

/* Data Register, Port A */
#define    PA7      7
#define    PA6      6
#define    PA5      5
#define    PA4      4
#define    PA3      3
#define    PA2      2
#define    PA1      1
#define    PA0      0
 
/* Data Register, Port A */
#define    PORTA7   7
#define    PORTA6   6
#define    PORTA5   5
#define    PORTA4   4
#define    PORTA3   3
#define    PORTA2   2
#define    PORTA1   1
#define    PORTA0   0
 
/* Data Direction Register, Port A */
#define    DDA7     7
#define    DDA6     6
#define    DDA5     5
#define    DDA4     4
#define    DDA3     3
#define    DDA2     2
#define    DDA1     1
#define    DDA0     0
 
/* Input Pins, Port A */
#define    PINA7    7
#define    PINA6    6
#define    PINA5    5
#define    PINA4    4
#define    PINA3    3
#define    PINA2    2
#define    PINA1    1
#define    PINA0    0

/* Data Register, Port B */
#define    PB7      7
#define    PB6      6
#define    PB5      5
#define    PB4      4
#define    PB3      3
#define    PB2      2
#define    PB1      1
#define    PB0      0
 
/* Data Register, Port B */
#define    PORTB7   7
#define    PORTB6   6
#define    PORTB5   5
#define    PORTB4   4
#define    PORTB3   3
#define    PORTB2   2
#define    PORTB1   1
#define    PORTB0   0
 
/* Data Direction Register, Port B */
#define    DDB7     7
#define    DDB6     6
#define    DDB5     5
#define    DDB4     4
#define    DDB3     3
#define    DDB2     2
#define    DDB1     1
#define    DDB0     0
 
/* Input Pins, Port B */
#define    PINB7    7
#define    PINB6    6
#define    PINB5    5
#define    PINB4    4
#define    PINB3    3
#define    PINB2    2
#define    PINB1    1
#define    PINB0    0

/* Data Register, Port C */
#define    PC7      7
#define    PC6      6
#define    PC5      5
#define    PC4      4
#define    PC3      3
#define    PC2      2
#define    PC1      1
#define    PC0      0
 
/* Data Register, Port C */
#define    PORTC7   7
#define    PORTC6   6
#define    PORTC5   5
#define    PORTC4   4
#define    PORTC3   3
#define    PORTC2   2
#define    PORTC1   1
#define    PORTC0   0
 
/* Data Direction Register, Port C */
#define    DDC7     7
#define    DDC6     6
#define    DDC5     5
#define    DDC4     4
#define    DDC3     3
#define    DDC2     2
#define    DDC1     1
#define    DDC0     0
 
/* Input Pins, Port C */
#define    PINC7    7
#define    PINC6    6
#define    PINC5    5
#define    PINC4    4
#define    PINC3    3
#define    PINC2    2
#define    PINC1    1
#define    PINC0    0

/* Data Register, Port D */
#define    PD7      7
#define    PD6      6
#define    PD5      5
#define    PD4      4
#define    PD3      3
#define    PD2      2
#define    PD1      1
#define    PD0      0
 
/* Data Register, Port D */
#define    PORTD7   7
#define    PORTD6   6
#define    PORTD5   5
#define    PORTD4   4
#define    PORTD3   3
#define    PORTD2   2
#define    PORTD1   1
#define    PORTD0   0
 
/* Data Direction Register, Port D */
#define    DDD7     7
#define    DDD6     6
#define    DDD5     5
#define    DDD4     4
#define    DDD3     3
#define    DDD2     2
#define    DDD1     1
#define    DDD0     0
 
/* Input Pins, Port D */
#define    PIND7    7
#define    PIND6    6
#define    PIND5    5
#define    PIND4    4
#define    PIND3    3
#define    PIND2    2
#define    PIND1    1
#define    PIND0    0

/* SPI Status Register */
#define    SPIF     7
#define    WCOL     6
#define    SPI2X    0

/* SPI Control Register */
#define    SPIE     7
#define    SPE      6
#define    DORD     5
#define    MSTR     4
#define    CPOL     3
#define    CPHA     2
#define    SPR1     1
#define    SPR0     0

/* Analog Comparator Control and Status Register */
#define    ACD      7
#define    ACBG     6
#define    ACO      5
#define    ACI      4
#define    ACIE     3
#define    ACIC     2
#define    ACIS1    1
#define    ACIS0    0

/* ADC Multiplexer Selection Register */
#define    REFS1    7
#define    REFS0    6
#define    ADLAR    5
#define    MUX4     4
#define    MUX3     3
#define    MUX2     2
#define    MUX1     1
#define    MUX0     0

/* ADC Control and Status Register */
#define    ADEN     7
#define    ADSC     6
#define    ADFR     5
#define    ADIF     4
#define    ADIE     3
#define    ADPS2    2
#define    ADPS1    1
#define    ADPS0    0

/* Data Register, Port E */
#define    PE7      7
#define    PE6      6
#define    PE5      5
#define    PE4      4
#define    PE3      3
#define    PE2      2
#define    PE1      1
#define    PE0      0
 
/* Data Register, Port E */
#define    PORTE7   7
#define    PORTE6   6
#define    PORTE5   5
#define    PORTE4   4
#define    PORTE3   3
#define    PORTE2   2
#define    PORTE1   1
#define    PORTE0   0
 
/* Data Direction Register, Port E */
#define    DDE7     7
#define    DDE6     6
#define    DDE5     5
#define    DDE4     4
#define    DDE3     3
#define    DDE2     2
#define    DDE1     1
#define    DDE0     0
 
/* Input Pins, Port E */
#define    PINE7    7
#define    PINE6    6
#define    PINE5    5
#define    PINE4    4
#define    PINE3    3
#define    PINE2    2
#define    PINE1    1
#define    PINE0    0


/* Pointer definition */
#define    XL     r26
#define    XH     r27
#define    YL     r28
#define    YH     r29
#define    ZL     r30
#define    ZH     r31

/* Contants */
#define    RAMEND   0x10FF    /*Last On-Chip SRAM location*/
#define    XRAMEND  0xFFFF
#define    E2END    0x0FFF
#define    FLASHEND 0x1FFFF

#endif /* ENABLE_BIT_DEFINITIONS */ 
#endif /* __IOM128_H (define part) */
#endif /* __IOM128_H (SFR part) */
