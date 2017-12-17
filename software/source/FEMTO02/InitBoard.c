#include <iom128.h>
#include <inavr.h>
#include "define.h"
#include "ioport_128.h"
#include "initboard.h"  // added by jang 2017.9.17
#include "dot_matrix_func.h"
#include "main_func.h"
#include "remocon.h"
#include "uart.h" // added by jang 2017.9.15
#include "i2c_master.h" // added by jang 2017.9.17
#include "ak4118a.h" // added by jang 2017.9.17
#include "es9038.h"// added by jang 2017.9.19
#include "at24c16.h"            // added by jang 2017.10.22

#define AK4118A_IC_ADDR AK4118A_I2C_ADDRESS(0)

extern char *dot_strings;
extern unsigned char ess_lch_master_trim;
extern unsigned char ess_rch_master_trim;

extern unsigned char tmr_osc;
extern unsigned char rom_add_check_sum;
extern unsigned int rom_add_pt;
extern unsigned long rom_cnt;
extern unsigned char ch_led_data;
extern unsigned char filter_flag;
extern unsigned char phase_data;
extern unsigned char vol_dB;
extern unsigned char vol_dB_HP;
extern unsigned char ch_led_data;
extern unsigned char dot_light_reg;

extern unsigned char AK4118A_read_register(unsigned char devaddr, unsigned char regaddr);
extern void send_string(char *p);
extern unsigned char Is_there_AK4118A(unsigned char ic_addr);

extern void es9038_soft_reset(unsigned char devaddr);
extern void es9038_system_mute(unsigned char devaddr, unsigned char onoff);
extern unsigned char es9038_read_register(unsigned char devaddr, unsigned char regaddr);
extern unsigned char Is_there_ES9038(void);
extern void es9038_audio_auto_select(unsigned char devaddr, unsigned char select);
extern void es9038_dac_channel_mapping(unsigned char devaddr,unsigned char dac_ch,unsigned char input_ch);
extern void es9038_automute_level(unsigned char devaddr, unsigned char level);
extern unsigned char es9038_set_volume(unsigned char devaddr,unsigned char volume_db);
extern void es9038_audio_input_select(unsigned char devaddr, unsigned char select);
extern void es9038_set_dpll_bw_dsd(unsigned char devaddr,unsigned char bandwidth);
extern void es9038_set_volume_rate(unsigned char devaddr, unsigned char volume_rate);
extern void es9038_set_soft_start_time(unsigned char devaddr, unsigned char time);
extern void es9038_set_gpio1(unsigned char devaddr, unsigned char gpio_cfg);
extern void es9038_18db_channel_gain_control(unsigned char devaddr,unsigned char ch, unsigned char onoff);

extern unsigned int AK4118A_read_status(unsigned char devaddr);
extern void AK4118A_reset(unsigned char devaddr);
extern void AK4118A_power_down(unsigned char devaddr);
extern void AK4118A_TX0_disable(unsigned char devaddr);
extern void AK4118A_TX1_disable(unsigned char devaddr);
extern void AK4118A_set_audio_format(unsigned char devaddr, unsigned char mode);

extern unsigned char flag_input_mode;
extern unsigned char vol_dB;
extern unsigned char vol_dB_HP;
extern unsigned char flag_mute;
extern unsigned char flag_filter;
extern unsigned char flag_headphone_output;
extern unsigned char eeprom_read_identity_error(void);
extern unsigned char eeprom_write_identity(char *str);
extern unsigned char eeprom_write_version (char *str);
extern unsigned char eeprom_write_revision(char *str);
extern unsigned char eeprom_write_mode(unsigned char mode);
extern unsigned char eeprom_write_line_volume(unsigned char line_volume);
extern unsigned char eeprom_write_headphone_volume(unsigned char headphone_volume);
extern unsigned char eeprom_write_mute(unsigned char mute);
extern unsigned char eeprom_write_filter(unsigned char filter);
extern unsigned char eeprom_write_output_headphone(unsigned char output_headphone);
extern unsigned char eeprom_save(unsigned char mode,\
                          unsigned char line_vol, unsigned char headphone_vol,\
                          unsigned char mute, unsigned char filter, \
                          unsigned char output_headphone);
extern unsigned char eeprom_read_mode(char *mode);
extern unsigned char eeprom_read_line_volume(char *line_volume);
extern unsigned char eeprom_read_headphone_volume(char *headphone_volume);
extern unsigned char eeprom_read_mute(char *mute);
extern unsigned char eeprom_read_filter(char *filter);
extern unsigned char eeprom_read_output_headphone(char *output_headphone);


extern void send_integer(unsigned char ch);
extern void send_byte2hex(unsigned char ch);
//extern void phase_ess(void);



U16 mtime_length = 0;
U8 mtime_flag = 0;






///////////////////////////////////////////////////////////////////////////////
/*
void wait_mtime(unsigned int time){
  //timer : 0.1msec
  mtime_length=0;
  mtime_flag=1;
	
	while(mtime_length < time){
	}
	mtime_flag=0;
}
*/
void _system_init(void)
{
  U8 i;
  unsigned int status=0;

  send_string("Port init.\r\n");
//DDR=1, PORT=1 --> 출력 High
//DDR=1, PORT=0 --> 출력 Low
//DDR=0, PORT=1 --> 입력(내부 Pull Up)

//PORTA	: 7(DotD7)=Out,L      6(DotD6)=Out,L        5(DotD5)=Out,L       4(DotD4)=Out,L     3(DotD3)=Out,L    2(DotD2)=Out,L      1(DotD1)=Out,L      0(DotD0)=Out,L
//PORTB	: 7(APower_EN)=Out,L  6(Power_EN)=In,H     5(open)=Out,H        4(open)=Out,H      3(open)=Out,H     2(open)=Out,H       1(sck)=In,H         0(open)=Out,H
//PORTC	: 7(Dot_/CE1)=Out,H   6(open)=Out,H         5(open)=Out,H        4(open)=Out,H      3(open)=Out,H     2(open)=Out,H       1(open)=Out,H       0(open)=Out,H
//PORTD	: 7(Dot/CLR)=Out,H    6(Inkey1_A2)=In,H     5(Inkey1_A1)=In,H    4(Inkey1_A0)=In,H  3(Inkey1)=In,H    2(CS_ERR)=In,H      1(iSDA)=In,H       0(iSCL)=Out,H
//PORTE	: 7(AutoMute)=In,H    6(LOCK)=In,H          5(R_Trans)=In,H      4(CS_ERR1)=In,H    3(I2S_Sel)=Out,H  2(XMOS_check)=In,H  1(TXD)=Out,H         0(RXD)=In,H
//PORTF	: 7(TDI)=In,H         6(TDO)=In,H           5(TMS)=In,H          4(TCK)=In,H        3(Dot_A3)=Out,L   2(Dot_A2)=Out,L     1(Dot_A1)=Out,L     0(Dot_A0)=Out,L
//PORTG	:                                                                4(open)=Out,H      3(open)=Out,H     2(Dot_/CE1)=Out,H   1(Dot_/CE0)=Out,H   0(Dot_A4)=Out,L
	
// DDR Setting  
  DDRA=0xff;      DDRB=0xbd;      DDRC=0xff;    DDRD=0x83;      DDRE=0x4A;      DDRF=0x0f;      DDRG=0x1f;
//DDRA=0xff;      DDRB=0x3d;      DDRC=0xff;    DDRD=0x83;      DDRE=0x08;      DDRF=0x0f;      DDRG=0x1f;

// PORT Setting   
    PORTA= 0x00;    PORTB=0x7f;     PORTC=0xff;      PORTD=0xff;     PORTE=0xff;     PORTF=0xf0;     PORTG=0x1e;
//PORTA= 0x00;    PORTB=0xff;     PORTC=0xff;      PORTD=0xff;     PORTE=0xff;     PORTF=0xf0;     PORTG=0x1e;

  UA_EN_DDR_INIT;     UA_EN_PORT_INIT;
  DSD128_DDR_INIT;    DSD128_PORT_INIT;
  USB_DET_DDR_INIT;   USB_DET_PORT_INIT;
  HP_MUTE_DDR_INIT;   HP_MUTE_PORT_INIT;
  LINE_MUTE_DDR_INIT; LINE_MUTE_PORT_INIT;
  DAC_MUTE_DDR_INIT;  DAC_MUTE_PORT_INIT;
  AMLI_DDR_INIT;      AMLI_PORT_INIT;
  I2S_SEL_DDR_INIT;   I2S_SEL_PORT_INIT;
  DSD_ON_DDR_INIT;    DSD_ON_PORT_INIT;
  //AK_INT0_DDR_INIT; AK_INT0_PORT_INIT;
  //AK_INT1_DDR_INIT; AK_INT1_PORT_INIT;
  
///////////////////////////////////////////////////////////////////////////////

    // I2C Setting
    send_string("I2C initialize..\r\n");
    //TWSR = 0;
    //TWBR = 0x0C;
    //TWCR = (0x01<<TWEN);
    i2c_init();
   
    //Timer Setting
    send_string("Timer setting.\r\n");
    //Timer0
    TCCR0 = (0<<FOC0) | (0<<WGM00) | (0<<COM01) | (0<<COM00) |(0<<WGM01) | (0<< CS02)| (1<< CS01)| (1<< CS00);	// 32prescaling
    //Timer1
    //TCCR1B = (1<< CS12)| (0<< CS11)| (1<< CS10);	// 1024precaling
    TCCR1B = (1<< CS12)| (0<< CS11)| (0<< CS10);	// 256precaling
    
    //Timer2 - remocon
    TCCR2 = (0<<FOC2) | (0<<WGM20) | (0<<COM21) | (0<<COM20) |(0<<WGM21) | (0<< CS22)| (1<< CS21)| (0<< CS20);	// 8prescaling, remocon
    
    TCNT2 = 0xff - TIMER0_CNT;
    TCNT0=255-250; //0.5msec 
    TCNT1=0xffff-1563; //0.1sec 

    // Output Compare Register.
    OCR0 = 0x00;
    // OCIE0 : Timer / Counter0 Output Compare Match Interrupt Enable.
    // '1' : Timer / Counter0 is enabled.
    // '0' : Timer / Counter0 is disabled.
    // TOIE0 : Timer / Counter0 Overflow Interrupt Enable.
    // '1' : Timer / Counter0 Overflow Interrupt enabled.
    // '0' : Timer / Counter0 Overflow Interrupt disabled.
    TIMSK = (1<<TOIE2) | (1<<TOIE1) | (1<<TOIE0);       // Timer1 Interrupt Enable, Timer2 Interrupt Enable, Timer3 Interrupt Enable
    // Timer / Counter Interrupt Flag Register.
    // OCF0 : Output Compare Flag.
    // TOV0 : Timer / Overflow Flag.
    // '1' : When Timer / Counter0 cause the overflow.
    TIFR = (1<<TOV2) | (1<<TOV1) | (1<<TOV0);
    
///////////////////////////////////////////////////////////////////////////////
//interrupt
    send_string("Interrupt setting.\r\n");
    //EIMSK = (1<<INT7) | (1<<INT6) | (1<<INT5) | (1<<INT4) | (1<<INT3) | (1<<INT2) | (1<<INT1) | (1<<INT0);
    
    //EICRA = (1<<ISC31) | (0<<ISC30) | (0<<ISC21) | (0<<ISC20) | (0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);
    //ISCn1	ISCn0
    //	0				0		: The low level of INTn generates an interrupt request.
    //	0				1		: Reserved
    //	1				0		: The falling edge of INTn generates asynchronously an interrupt request.
    //	1				1		: The rising edge of INTn generates asynchronously an interrupt request.
    
    //EICRB = (0<<ISC41) | (1<<ISC40) | (0<<ISC51) | (1<<ISC50) | (1<<ISC61) | (0<<ISC60) | (0<<ISC71) | (1<<ISC70);
    //ISCn1	ISCn0
    //	0				0		: The low level of INTn generates an interrupt request.
    //	0				1		: Any logical change on INTn generates an interrupt request
    //	1				0		: The falling edge of INTn generates asynchronously an interrupt request.
    //	1				1		: The rising edge of INTn generates asynchronously an interrupt request.
    
    //EIFR =  (1<<INTF7) | (1<<INTF6) | (1<<INTF5) | (1<<INTF4) |  (1<<INTF3) | (1<<INTF2) | (1<<INTF1) | (1<<INTF0);

// #define INT_AK_INT1  INT2 // Rising edge  (group A)
// #define INT_INKEY1   INT3 // Rising edge  (group A)
// #define INT_AK_INT0  INT4 // Rising edge  (group B)
// #define INT_IR_TRANS INT5 // Logic  edge  (group B)
// #define INT_AMLI     INT6 // Rising edge  (group B) (X)
// #define INT_UA_EN    INT7 // Logic  edge  (group B)
    
    EIMSK = (1<<INT_UA_EN) | (1<<INT_AK_INT0) | (1<<INT_AK_INT1) | (1<<INT_IR_TRANS) |\
            (1<<INT_INKEY1);            // ALMI Removed. 17.11.10
    EICRA = (1<<ISC31) | (0<<ISC30) | (1<<ISC21) | (1<<ISC20);
    EICRB = (0<<ISC71) | (1<<ISC70) | (1<<ISC61) | (1<<ISC60) |\
            (0<<ISC51) | (1<<ISC50) | (1<<ISC41) | (1<<ISC40);
       
    //INT7~INT2
    EIFR =  (1<<INTF2) | (1<<INTF3) | (1<<INTF4) | (1<<INTF5) | (1<<INTF7); // AMLI Removed. 17.11.10
    
    
    ////////////Initial ES9038 /////////////////////////
    es9038_soft_reset(ES9038_ADDR0); es9038_soft_reset(ES9038_ADDR1);
    send_string("[I2C] ES9038PRO Soft Reset complete.\r\n");

    send_string("[I2C] ES9038 U22 Left Mute.\r\n");
    es9038_system_mute(ES9038_ADDR0,1);		//mute
    if(es9038_read_register(ES9038_ADDR0,ES9038_REG_FILTERBW_MUTE)==0x41) \
         send_string("[I2C] ES9038 U22 Left Mute OK.\r\n");
    else send_string("[I2C] ES9038 U22 Left Mute NG.\r\n");
    send_string("[I2C] ES9038 U52 Right Mute.\r\n");
    es9038_system_mute(ES9038_ADDR1,1);		//mute
    if(es9038_read_register(ES9038_ADDR1,ES9038_REG_FILTERBW_MUTE)==0x41) \
         send_string("[I2C] ES9038 U52 Right Mute OK.\r\n");
    else send_string("[I2C] ES9038 U52 Right Mute NG.\r\n");

    send_string("[I2C] ES9038 Searching...\r\n");
    i=Is_there_ES9038();
    if (i==0) send_string("[I2C] ES9038 x2 not found.\r\n");
    else if(i==3) send_string("[I2C] ES9038 x2 found.\r\n");
    else if(i==1) send_string("[I2C] ES9038 olny left found.\r\n");
    else if(i==2) send_string("[I2C] ES9038 olny right found.\r\n");

    es9038_audio_auto_select(ES9038_ADDR0,AUTO_SEL_DSD_I2S);
    es9038_audio_auto_select(ES9038_ADDR1,AUTO_SEL_DSD_I2S);
    send_string("[I2C] ES9038 Auto Select function DSD/I2S complete.\r\n");
    
    for(i=1;i<=8;i++) es9038_dac_channel_mapping(ES9038_ADDR0,i,INPUT_CH1);
    for(i=1;i<=8;i++) es9038_dac_channel_mapping(ES9038_ADDR1,i,INPUT_CH2);
    send_string("[I2C] ES9038 Channel mapping complete.\r\n");
    
    if (es9038_set_volume(ES9038_ADDR0,ES9038_MAX_VOLUME)) send_string("[I2C] ES9038 Left Volume Max complete.\r\n");
    if (es9038_set_volume(ES9038_ADDR1,ES9038_MAX_VOLUME)) send_string("[I2C] ES9038 Right Volume Max complete.\r\n");
    
    es9038_set_volume_rate(ES9038_ADDR0,7);
    es9038_set_volume_rate(ES9038_ADDR1,7);
    send_string("[I2C] ES9038 Volume Ramp Rate = 7.\r\n");
    
    es9038_set_soft_start_time(ES9038_ADDR0, REG_SOFT_START_TIME);
    es9038_set_soft_start_time(ES9038_ADDR1, REG_SOFT_START_TIME);
    send_string("[I2C] ES9038 Soft start time = 11.\r\n");
    
    //send_string("[I2C] CS8416 Input7 to GND.\r\n");
    //I2C_Write(0x20, 0x04, 0xb8);              // CS8416 input7 = gnd
    
    es9038_system_mute(ES9038_ADDR0,0); // disable mute
    es9038_system_mute(ES9038_ADDR1,0); // disable mute
    send_string("[I2C] ES9038 Mute Off.\r\n");
    
    es9038_set_dpll_bw_dsd(ES9038_ADDR0,DPLL_HIGHEST);
    es9038_set_dpll_bw_dsd(ES9038_ADDR1,DPLL_HIGHEST);
    send_string("[I2C] ES9038 DSD DPLL Highest.\r\n");
    
    //es9038_set_gpio1(ES9038_ADDR0, GPIO_AMLI);
    //es9038_set_gpio1(ES9038_ADDR1, GPIO_AMLI);
    //send_string("[I2C] ES9038 GPIO1 = AMLI Function.\r\n");
    
    
    
    
    //////////// AK4118A Initialize ///////////
    // AK4118 has Reset IC APX809 (200ms delay)
    //AK4118A_power_down(AK4118A_IC_ADDR); // AK4118A power down
    //send_string("[I2C] AK4118A power down.\r\n");
    delay_ms(200);
    send_string("[I2C] AK4118A Searching...\r\n");
    if (Is_there_AK4118A(AK4118A_IC_ADDR)) send_string("[I2C] AK4118A Found.\r\n");
    else send_string("[I2C] AK4118A not found.\r\n");
        
    status=AK4118A_read_status(AK4118A_IC_ADDR);
    if (status>>8) {
      send_string("[I2C] AK4118A Error found.\r\n Error code = ");
      send_integer(status>>8);
      send_string("\r\n");
    }
    AK4118A_TX0_disable(AK4118A_IC_ADDR);
    AK4118A_TX1_disable(AK4118A_IC_ADDR);
    AK4118A_set_audio_format(AK4118A_IC_ADDR, 5);
    
    //Dot matrix clear
    send_string("[SPI] DOT Matrix Clear.\r\n");
    dot_matrix_clear();
    
    // Reading EEPROM Settings
    if (eeprom_read_identity_error()) { // factory reset
      send_string("[I2C] Factory Reset....\r\n");
      eeprom_write_identity(EEPROM_STRING_IDENTITY);
      eeprom_write_version (EEPROM_STRING_VERSION);
      eeprom_write_revision(EEPROM_STRING_REVISION);
      eeprom_write_mode(EEPROM_MODE);
      flag_input_mode=EEPROM_MODE;
      eeprom_write_line_volume(EEPROM_LINE_VOLUME);
      vol_dB=EEPROM_LINE_VOLUME;
      eeprom_write_headphone_volume(EEPROM_HEADPHONE_VOLUME);
      vol_dB_HP=EEPROM_HEADPHONE_VOLUME;
      eeprom_write_mute(EEPROM_VOLUME_MUTE);
      flag_mute=EEPROM_VOLUME_MUTE;
      eeprom_write_filter(EEPROM_FILTER);
      flag_filter=EEPROM_FILTER;
      eeprom_write_output_headphone(EEPROM_OUTPUT_HEADPHONE);
      flag_headphone_output=EEPROM_OUTPUT_HEADPHONE;
      send_string("[I2C] Factory Reset Completed.\r\n");
    }
    else {
      send_string("[I2C] Reading from EEPROM....\r\n");
      eeprom_read_mode(&flag_input_mode);
      eeprom_read_line_volume(&vol_dB);
      eeprom_read_headphone_volume(&vol_dB_HP);
      eeprom_read_mute(&flag_mute);
      eeprom_read_filter(&flag_filter);
      eeprom_read_output_headphone(&flag_headphone_output);
      send_string("[I2C] Reading Complete from EEPROM.\r\n");
    }

///////////////////////////////////////////////////////////////////////////////
//I2C
    // f(scl) = ( f(cpu) ) / ( 16+2*TWBR*4^(TWPS) )
    // f(cpu) = 16MHz
    //TWBR = 72;	//100kHz
    //TWSR = 0;    //100kHz
    
    //TWBR = 152;	//50kHz
    //TWSR = 0;    //50kHz
    
      //interrupt enable
  __enable_interrupt(); 
  
   for(i=0; i<16; i++) dot_strings[i]=' ';


///////////////////////////////////////////////////////////////////////////////  
  //L-ch, R-ch gain setting
  //master_trim(#23) 
  //7f = -0.000dB,  //7b = -0.275dB,  //77 = -0.560dB,  //73 = -0.854dB,
  //7e = -0.068dB,  //7a = -0.345dB,  //76 = -0.633dB,  //72 = -0.930dB,
  //7d = -0.136dB,  //79 = -0.416dB,  //75 = -0.706dB,  //71 = -1.005dB,
  //7c = -0.206dB,  //78 = -0.488dB,  //74 = -0.780dB,  //70 = -1.082dB
  //ex, I2C_Write(0x90, 20, 0x7f-ess_lch_master_trim);
  ess_lch_master_trim=0;
  ess_rch_master_trim=0;

///////////////////////////////////////////////////////////////////////////////  



//test
tmr_osc=1;


}
/*
void _system_init_1(){
  //Power S/W : ON
  POWER_SW =1;
  DAC_POWER_SW =1; 
}
*/

U16 test_check=0;
U16 test_check2=0;
U8 init_rom_data0=0;
U8 init_rom_data1=0;
U8 init_rom_data2=0;
U8 init_rom_add0=0;
U8 init_rom_add1=0;
U8 init_rom_add2=0;
U8 init_rom_address_data=0;

void rom_check_sum_error(void){
  U16 i=0x0a;
  
  while(i<=0x7fa){
    i+=5;
    
    if((i&0x000f) == 0x000f) i++;
      
    //init_rom_address_data=rom_I2C_Read(i);
    
    if( init_rom_address_data  == 0xff) break;
  }
  
  test_check=i;
  
  init_rom_add0=(test_check>>8)&0xff;
  init_rom_add1=test_check&0xff;
  init_rom_add2=init_rom_add0+init_rom_add1;
  
  //I2C_START();
  //I2C_SHOUT(0xa0);        //device address
  //I2C_SHOUT(0x00);        //Sub address   
  //I2C_SHOUT(init_rom_add0);       
  //I2C_SHOUT(init_rom_add1);       
  //I2C_SHOUT(init_rom_add2); 
  //I2C_STOP();
    
  delay_ms(10);  //5msec
  
  if(test_check<0x7ff) {      
    //I2C_START();
    //I2C_SHOUT((0xa0 + ((test_check>>7) & 0x000e)));        //device address
    //I2C_SHOUT(test_check & 0x00ff);        //Sub address   
    //I2C_SHOUT(0x00);       //
    //I2C_SHOUT(0x00);       //
    //I2C_SHOUT(0x00);       //
    //I2C_SHOUT(0xff);       //vol(-999.999dB)
    //I2C_SHOUT(0x01);       //ch(4bits)+filter(3bits)+inverse(1bit)
    //I2C_STOP();
      
    delay_ms(10);  //5msec
  }
  
}


void _system_init_se(void){
  U16 i=0;
  
  //I2C_Write(0x20, 0x04, 0xb8);              // CS8416 input7 = gnd
  //send_string("[I2C] CS8416 Input7 to GND.\r\n");
  //I2C_Write(0x20, 0x04, 0xb8);              // CS8416 input7 = gnd
  //send_string("[I2C] ES9038 U22 Right Mute.\r\n");
  //es9038_system_mute(ES9038_ADDR0,1);
  //send_string("[I2C] ES9038 U52 Left Mute.\r\n");
  //es9038_system_mute(ES9038_ADDR1,1);

  
  //PORTB_Bit6=1;   //Digital Power Enable

///////////////////////////////////////////////////////////////////////////////
//EEPROM

 /*
 //test eeprom
test_set_eeprom(0x1a, 0x0c);
test_set_eeprom(0x1b, 0x34);
test_set_eeprom(0x1c, 0xaf);
test_set_eeprom(0x1d, 0x35);
test_set_eeprom(0x1e, 0x01);
 
 
 test_set_eeprom(0, 0x00);
 test_set_eeprom(1, 0x1a);
*/

  //i=rom_I2C_Read(0);
  ////////처음 한번만
  if(i==0xff){
    //I2C_START();
    //I2C_SHOUT(0xa0);        //device address
    //I2C_SHOUT(0x00);        //Sub address   
    //I2C_SHOUT(0x00);       //
    //I2C_SHOUT(0x10);       //
    //I2C_SHOUT(0x10);       //check_sum
    //I2C_STOP();
    
    delay_ms(5);  //5msec
    
    //I2C_START();
    //I2C_SHOUT(0xa0);        //device address
    //I2C_SHOUT(0x10);        //Sub address   
    //I2C_SHOUT(0x00);       //
    //I2C_SHOUT(0x00);       //
    //I2C_SHOUT(0x00);       //
    //I2C_SHOUT(0xff);       //vol(-999.999dB)
    //I2C_SHOUT(0x01);       //ch(4bits)+filter(3bits)+inverse(1bit)
    //I2C_STOP();
    
    delay_ms(5);  //5msec
  }
  
  //init_rom_data0=rom_I2C_Read(0);
  //init_rom_data1=rom_I2C_Read(1);
  //init_rom_data2=rom_I2C_Read(2);
  rom_add_check_sum=init_rom_data0+init_rom_data1;
  
  if(rom_add_check_sum!=init_rom_data2) { //error
    //rom_check_sum_error();
  }
  else {
    test_check=init_rom_data0;
    test_check<<=8;
    test_check+=init_rom_data1;
  }
/*
  ///////////////////////
  //에러 체크 및 보정
  //test_check-=0x10;
  test_check2=test_check;
  test_check2=test_check&0x000f;
  if( test_check2 %5 ){
    if (test_check2<5) test_check2=5;
    else if (test_check2<10) test_check2=10;
    else test_check2=0x10;
    test_check=(test_check&0xfff0) + test_check2;

    if(test_check>0x7fe) {      
      I2C_START();
      I2C_SHOUT(0xa0);        //device address
      I2C_SHOUT(0x00);        //Sub address   
      I2C_SHOUT(0x08);       
      I2C_SHOUT(0x00);       
      I2C_STOP();
      
      vol_dB=0xff;
      ch_led_data=0x00;
      filter_flag=0x00;
      phase_data=0x01;
    
      delay_ms(5);  //5msec
    }
    else{
      I2C_START();
      I2C_SHOUT(0xa0);        //device address
      I2C_SHOUT(0x00);        //Sub address   
      I2C_SHOUT((test_check>>8)&0xff);       
      I2C_SHOUT(test_check&0xff);       
      I2C_STOP();
    
      delay_ms(10);  //5msec
      
      I2C_START();
      I2C_SHOUT((0xa0 + ((test_check>>7) & 0x000e)));        //device address
      I2C_SHOUT(test_check & 0x00ff);        //Sub address   
      I2C_SHOUT(0x00);       //
      I2C_SHOUT(0x00);       //
      I2C_SHOUT(0x00);       //
      I2C_SHOUT(0xff);       //vol(-999.999dB)
      I2C_SHOUT(0x01);       //ch(4bits)+filter(3bits)+inverse(1bit)
      I2C_STOP();
    
    delay_ms(10);  //5msec
      
    }
  }
 /////////////////
*/

  rom_add_pt=test_check;
  
  if(rom_add_pt<0x07ff){
    //rom_cnt=rom_I2C_Read(rom_add_pt);
    rom_cnt=rom_cnt<<8;
    //rom_cnt+=rom_I2C_Read(rom_add_pt+1);
    rom_cnt=rom_cnt<<8;
    //rom_cnt+=rom_I2C_Read(rom_add_pt+2);
    
    //vol_dB=rom_I2C_Read(rom_add_pt+3);
    //i=rom_I2C_Read(rom_add_pt+4);
    
    
    if(i<0x80) {    //channel 4bits, filter 3bits, inverse 1bit
      ch_led_data=((i>>4)&0x0f);
      filter_flag=((i>>1)&0x07);
      phase_data=(i&0x01);
    }
    else{
        vol_dB=0xff;
        vol_dB_HP=0xFF;
        ch_led_data=0x00;
        filter_flag=0x00;
        phase_data=0x01;
    }
  }
  else {    
    vol_dB=0xff;
    vol_dB_HP=0xFF;
    ch_led_data=0x00;
    filter_flag=0x00;
    phase_data=0x01;
  }
    
  /*
    //////////write check
    rom_write_multi();
    delay_ms(10);  //5msec
    
    rom_cnt=rom_I2C_Read(rom_add_pt);
    rom_cnt=rom_cnt<<8;
    rom_cnt+=rom_I2C_Read(rom_add_pt+1);
    rom_cnt=rom_cnt<<8;
    rom_cnt+=rom_I2C_Read(rom_add_pt+2);
    
    vol_dB=rom_I2C_Read(rom_add_pt+3);
    i=rom_I2C_Read(rom_add_pt+4);
    test_set_eeprom(0x1c,0xff)
    
    if(old_rom_cnt!=rom_cnt){
      rom_add_pt=(((rom_add_pt/5)+1)*5);
      vol_dB=0xff;
      ch_led_data=0x00;
      filter_flag=0x00;
      phase_data=0x01;
      rom_write_multi();
      delay_ms(10);  //5msec
    }
    //////////
    */
/////////////////////////////////////////////////////////////////////////////// 

  //initialize CS8416
  //I2C_Write(0x20, 0x00, 0x00);
  //I2C_Write(0x20, 0x01, 0x84);  
  //I2C_Write(0x20, 0x02, 0x09);              //GPO0 : Non-audio check
  //I2C_Write(0x20, 0x03, 0x50);              //GPO1 : RERR check,
  //I2C_Write(0x20, 0x04, 0xb8);              // CS8416 input7 = gnd
  //I2C_Write(0x20, 0x04, 0x80);              // test CS8416 input0
  channel_change();
  //I2C_Write(0x20, 0x05, 0x85);
  //I2C_Write(0x20, 0x06, 0x17);
  
  
  //I2C_Write(0x20, 0x01, 0x82);  
  //I2C_Write(0x20, 0x01, 0x80);
  //I2C_Write(0x20, 0x05, 0x8d);
  
  
  
  

  //initialize ES9038
  
  //I2C_Write(0x90, 0x0b, 0x85);
  //I2C_Write(0x90, 0x0b, 0x95); //DPLL_BANDWIDTH : Med-High
  //I2C_Write(0x90, 0x0b, 0x9d); //DPLL_BANDWIDTH : Highest
  //es9038_set_dpll_bw(0x0F,0x0F); //DPLL_BANDWIDTH : Highest
  //I2C_Write(0x90, 0x0c, 0x20);
  //I2C_Write(0x90, 0x0f, 0x00);
  //I2C_Write(0x90, 0x11, 0x1d);
  //I2C_Write(0x90, 0x19, 0x01);
  //es9038_serial_data_config_automute(AUTOMUTE_RAMPNGND,I2S_MODE);
  
  //I2C_Write(0x92, 0x0b, 0x85);
  //I2C_Write(0x92, 0x0b, 0x95); //DPLL_BANDWIDTH : Med-High
  //I2C_Write(0x92, 0x0b, 0x9d); //DPLL_BANDWIDTH : Highest
  //I2C_Write(0x92, 0x0c, 0x20);
  //I2C_Write(0x92, 0x0f, 0x00);
  //I2C_Write(0x92, 0x10, 0x08);
  //I2C_Write(0x92, 0x11, 0x9d);
  //I2C_Write(0x92, 0x19, 0x01);
  
  // set automute level -120dB
  es9038_automute_level(ES9038_ADDR0,120);
  es9038_automute_level(ES9038_ADDR1,120);
  
  if (flag_headphone_output) volume_set(vol_dB_HP);   // set es9038
  else                       volume_set(vol_dB);
  delay_ms(10);  //5msec
  //phase_ess();
  delay_ms(10);  //5msec
  ess_filter();
  
//Dot-matrix & LED Display
//Dot-matrix
  DOT_CLR_FLAG = 1;		//remove CLR(dot-matrix)
  dot_light_reg=1;			//DOT_CLR_FLAG = 1 -> dot_light_reg=1

  //bright = (0 = 100%), (1 = 80%), (2 = 53%), (3 = 40%), (4 = 27%), (5 = 20%), (6 = 13%), (7 = 0%)
  
  DOT_ADDR4=1;
  dot_matrix_bright(3);		//Bright 27%  
  
  //PORTB_Bit7=1;       //Analog Power Enable
/*  
 for(i=0; i<0x30; i++) reset_eeprom(i);
 for(i=0; i<0x20; i++)  reset_eeprom(i+0x7e0);

//test check
  for(i=0; i<0x80; i++)     test_eeprom[i]=rom_I2C_Read(i);
  for(i=0; i<0x20; i++)     test_eeprom[i+0x80]=rom_I2C_Read(i+0x7e0);
  */
  //for(i=0; i<0x20; i++)     test_eeprom[i]=rom_I2C_Read(i);
}

void delay_ms(unsigned int ms) {
  unsigned int i, k;
  
  for(i=0; i<=ms+1; i++) { k=16000;while(k--); } //1msec
}