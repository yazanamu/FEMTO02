#define BUSY 1
#define IDLE 0

#define CLK 8000000
#define PRESCALE 8
#define TIME_US 100
#define TIMER0_CNT ( CLK * TIME_US / (8 * 1000000) )


enum PORT16 {PPCA=1,PPCB,PPCC,PPCD};
#define LCD_Display_Clear 0x01
#define Cursor_Home 0x02

#define nCS       PORTD_Bit5
#define SDA_Out   PORTD_Bit6
#define SCL       PORTD_Bit7
#define test_in   PINC_Bit1
#define Encoder_A PIND_Bit2
#define Encoder_B PIND_Bit4
#define CH_MODE   PIND_Bit3

#define HIGH 1
#define LOW 0
#define OFF 1
#define ON 0

#define	U8	unsigned char
#define	U16	unsigned int
#define	U32	unsigned long

#define RELAY_C PORTB_Bit3
#define RELAY_B PORTD_Bit1
#define RELAY_A PORTD_Bit0
#define RELAY_D PORTC_Bit6
//#define RELAY_E PORTD_Bit4

#define FND_DB0 PORTA_Bit0
#define FND_DB1 PORTA_Bit1
#define FND_DB2 PORTA_Bit2
#define FND_DB3 PORTA_Bit3
#define FND_DB4 PORTA_Bit4
#define FND_DB5 PORTA_Bit5
#define FND_DB6 PORTA_Bit6
#define FND_DB7 PORTA_Bit7


U8 VolReg=0xb0; //-8dB
U8 KeyReady = HIGH, KeyEnable = HIGH;
U16 Remoc_data_addr[4];

U8 Remoc_data[10],frontRc=0,rearRc=0;
U8 USBReceived,LCD_Display=0;
U8 LCD_Debug_Mode=0,LCD_Debug_Mode_temp=0;

//extern void string_out(U8 , U8 *);
//extern void string_center_out(U8 ,U8 *);
//extern void char_out(U8);
//extern void instruction_out(U8 );
//extern void init_LCD();
//extern void LCD_Clear();
//extern void modulator_status(U8 );
//extern void LCD_Func(U8 ,U8 ,U8 );
//extern void _outbit(U8 ,U8 ,U8 );

U8 Check_Remoc=0,Count_LCD=0,LCD_STT=0,LCD_Mute=1;
U8 LCD_char=0,Remoc_enter=0;

unsigned int ADC_result=0x0000,earphone_Count=0;
unsigned char earphone=0,earphone_temp=0;

//#define ENABLE  (PORTB_Bit0=1)
//#define DISABLE (PORTB_Bit0=0)

U8 Check_Channel=0;
U8 Encoder_Status=0;

U8 Vol_decade=0;
U8 Vol_num=0;
U8 FND_flg=0;
U8 FND_TIMER_flg=0;
//U8 special_timer_flg=0;
U8 special_flg_check=0;
U8 flg_TCNT1=0;
U8 flg_TCNT1_check=0;

U8 remo_da1=0;
U8 remo_da2=0;
U8 remo_da3=0;
U8 remo_da4=0;
