#ifndef _AVR_DEFINE_H_
#define _AVR_DEFINE_H_

//unsigned char  test_eeprom[0x100];
//unsigned char  test_eeprom[0x20];

//test IR
//unsigned int test_IR_tmr[0x250];
//unsigned int test_IR_flag=0x05;
///////

#define SCL 		PORTD_Bit0
#define SDA_Out 		PORTD_Bit1
#define SDA_ST 		DDRD_Bit1
#define SDA_In 		PIND_Bit1

#define Low 0
#define High 1

#define BUSY 1
#define IDLE 0

#define CLK 16000000UL
#define PRESCALE 8
#define TIME_US 100
#define TIMER0_CNT ( CLK * TIME_US / (8 * 1000000) )

enum PORT128 {PPCA=1,PPCB,PPCC,PPCD,PPCE,PPCF,PPCG};

#define HIGH 1
#define LOW 0
#define OFF 1
#define ON 0

#define	U8	unsigned char
#define	U16	unsigned int
#define	U32	unsigned long
#define S32  long
#define uint8_t unsigned char
#define uint16_t unsigned int
#define uint32_t unsigned long

#define EEPROM_ADDR_VERSION 0x0
#define EEPROM_ADDR_VOLUME 0x10
#define EEPROM_ADDR_MODE 0x20
#define EEPROM_ADDR_MUTE 0x30

//EEPROM
//rom_add_pt : 주소데이터의 시작 주소(1Byte)
//rom_cycle : 주소데이터의 반복 횟수(2Byte)
//rom_address : 데이터의 주소(2Bytes)
//rom_data_cnt : 데이터의 카운트(1Byte)
//rom_data : 데이터(총 2Bytes: 볼륨 8bits, 채널 4bits, 필터 3bits, 위상 1bit)

__eeprom const unsigned char version[16] @ EEPROM_ADDR_VERSION;
__eeprom const unsigned char volume @ EEPROM_ADDR_VOLUME;
__eeprom const unsigned char mode @ EEPROM_ADDR_MODE;
__eeprom const unsigned char mute @ EEPROM_ADDR_MUTE;


#define DOT_DATA	PORTA

//#define POWER_SW  	PORTB_Bit6
//#define DAC_POWER_SW  	PORTB_Bit7

#define M2_DOT_ENABLE  	PORTC_Bit7

#define DOT_CLR_FLAG	PORTD_Bit7
#define KEY_DATA2       PIND_Bit6
#define KEY_DATA1       PIND_Bit5
#define KEY_DATA0       PIND_Bit4
#define KEY_FLAG        PIND_Bit3

#define AUTOMUTE        PINE_Bit7
#define ESS_LOCK        PINE_Bit6
#define I2S_SEL		PORTE_Bit3

#define DOT_ADDR0	PORTF_Bit0
#define DOT_ADDR1	PORTF_Bit1
#define DOT_ADDR2	PORTF_Bit2
#define DOT_ADDR3	PORTF_Bit3

#define DOT_WR		PORTG_Bit2
#define M1_DOT_ENABLE	PORTG_Bit1
#define DOT_ADDR4	PORTG_Bit0



#endif