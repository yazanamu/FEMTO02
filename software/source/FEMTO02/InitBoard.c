#include "Define.h"
#include "ioport_128.c"
//#include "I2C.h"
#include "dot_matrix_func.c"
#include "main_func.c"
#include "remocon.c"
#include "uart.h" // added by jang 2017.9.15
#include "initboard.h"  // added by jang 2017.9.17
#include "i2c_master.h" // added by jang 2017.9.17
#include "ak4118a.h" // added by jang 2017.9.17

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
void _system_init(void){
U8 i;
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
    DDRA=0xff;      DDRB=0xbd;      DDRC=0xff;    DDRD=0x83;      DDRE=0x0A;      DDRF=0x0f;      DDRG=0x1f;
//DDRA=0xff;      DDRB=0x3d;      DDRC=0xff;    DDRD=0x83;      DDRE=0x08;      DDRF=0x0f;      DDRG=0x1f;

// PORT Setting   
    PORTA= 0x00;    PORTB=0x7f;     PORTC=0xff;      PORTD=0xff;     PORTE=0xff;     PORTF=0xf0;     PORTG=0x1e;
//PORTA= 0x00;    PORTB=0xff;     PORTC=0xff;      PORTD=0xff;     PORTE=0xff;     PORTF=0xf0;     PORTG=0x1e;
    
///////////////////////////////////////////////////////////////////////////////

    // I2C Setting
    send_string("I2C initialize..\r\n");
    //TWSR = 0;
    //TWBR = 0x0C;
    //TWCR = (0x01<<TWEN);
    i2c_init();
   
    //Timer Setting
    send_string("Timer setting.\r\n");
    TCCR0 = (0<<FOC0) | (0<<WGM00) | (0<<COM01) | (0<<COM00) |(0<<WGM01) | (0<< CS02)| (1<< CS01)| (1<< CS00);	// 32prescaling
    TCCR1B = (1<< CS12)| (0<< CS11)| (1<< CS10);	// 1024precaling
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
    TIMSK = (1<<TOIE2) | (1<<TOIE1) | (1<<TOIE0);
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
    
    //INT7~INT2 : SelMCLK(7),	Lock-Lch(6),	REMOCON(5),	USB_Lock(4),	Inkey2(3),	CS8415-AUDIO(2)
    EIMSK = 0xFC;
    
    //  falling edge : Inkey2(3),	rising edge : CS8415-AUDIO(2), 	Reserved : (INT1), (INT0)
    EICRA = (1<<ISC31) | (0<<ISC30) | (1<<ISC21) | (0<<ISC20) | (0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);
    
    // Any logical change :  SelMCLK(7),	Lock-Lch(6),	REMOCON(5),	USB_Lock(4)
    EICRB =  (0<<ISC41) | (1<<ISC40) | (0<<ISC51) | (1<<ISC50) | (0<<ISC61) | (1<<ISC60) | (0<<ISC71) | (1<<ISC70);
    
    //INT7~INT2
    EIFR =  (1<<INTF2) | (1<<INTF3) | (1<<INTF4) | (1<<INTF5) | (1<<INTF6) | (1<<INTF7);
    
    send_string("[I2C] AK4118A Searching...\r\n");
    if (Is_there_AK4118A()) send_string("[I2C] AK4118A Found.\r\n");
    else send_string("[I2C] AK4118A not found.\r\n");
 
    send_string("[I2C] ES9038PRO Searching...\r\n");
    i=Is_there_ES9038();
    if (i==0) send_string("[I2C] ES9038PRO x2 not found.\r\n");
    else if(i==3) send_string("[I2C] ES9038PRO x2 found.\r\n");
    else if(i==1) send_string("[I2C] ES9038PRO olny left found.\r\n");
    else if(i==2) send_string("[I2C] ES9038PRO olny right found.\r\n");
    
    send_string("[I2C] CS8416 Input7 to GND.\r\n");
    //I2C_Write(0x20, 0x04, 0xb8);              // CS8416 input7 = gnd
    send_string("[I2C] ES9038 U22 Left Mute.\r\n");
    ES9038_ADDR0_MUTE;		//mute
    if(es9038_read_register(ES9038_ADDR0,ES9038_REG_FILTERBW_MUTE)==0x41) send_string("[I2C] ES9038 U22 Left Mute OK.\r\n");
    else send_string("[I2C] ES9038 U22 Left Mute NG.\r\n");
    
    send_string("[I2C] ES9038 U52 Right Mute.\r\n");
    ES9038_ADDR1_MUTE;		//mute
    if(es9038_read_register(ES9038_ADDR1,ES9038_REG_FILTERBW_MUTE)==0x41) send_string("[I2C] ES9038 U52 Right Mute OK.\r\n");
    else send_string("[I2C] ES9038 U52 Right Mute NG.\r\n");
	


    //Dot matrix clear
    send_string("[SPI] DOT Matrix Clear.\r\n");
    dot_matrix_clear();

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
  
   for(i=0; i<16; i++) dot_string[i]=' ';


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

void rom_check_sum_error(){
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
    
  DelayTime_ms(10);  //5msec
  
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
      
    DelayTime_ms(10);  //5msec
  }
  
}


void _system_init_se(void){
  U16 i;
  
  //I2C_Write(0x20, 0x04, 0xb8);              // CS8416 input7 = gnd
  send_string("[I2C] CS8416 Input7 to GND.\r\n");
  //I2C_Write(0x20, 0x04, 0xb8);              // CS8416 input7 = gnd
  send_string("[I2C] ES9038 U22 Right Mute.\r\n");
  es9038_system_mute(ES9038_ADDR0,1);
  send_string("[I2C] ES9038 U52 Left Mute.\r\n");
  es9038_system_mute(ES9038_ADDR1,1);

  
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
    
    DelayTime_ms(5);  //5msec
    
    //I2C_START();
    //I2C_SHOUT(0xa0);        //device address
    //I2C_SHOUT(0x10);        //Sub address   
    //I2C_SHOUT(0x00);       //
    //I2C_SHOUT(0x00);       //
    //I2C_SHOUT(0x00);       //
    //I2C_SHOUT(0xff);       //vol(-999.999dB)
    //I2C_SHOUT(0x01);       //ch(4bits)+filter(3bits)+inverse(1bit)
    //I2C_STOP();
    
    DelayTime_ms(5);  //5msec
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
    
      DelayTime_ms(5);  //5msec
    }
    else{
      I2C_START();
      I2C_SHOUT(0xa0);        //device address
      I2C_SHOUT(0x00);        //Sub address   
      I2C_SHOUT((test_check>>8)&0xff);       
      I2C_SHOUT(test_check&0xff);       
      I2C_STOP();
    
      DelayTime_ms(10);  //5msec
      
      I2C_START();
      I2C_SHOUT((0xa0 + ((test_check>>7) & 0x000e)));        //device address
      I2C_SHOUT(test_check & 0x00ff);        //Sub address   
      I2C_SHOUT(0x00);       //
      I2C_SHOUT(0x00);       //
      I2C_SHOUT(0x00);       //
      I2C_SHOUT(0xff);       //vol(-999.999dB)
      I2C_SHOUT(0x01);       //ch(4bits)+filter(3bits)+inverse(1bit)
      I2C_STOP();
    
    DelayTime_ms(10);  //5msec
      
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
        ch_led_data=0x00;
        filter_flag=0x00;
        phase_data=0x01;
    }
  }
  else {    
    vol_dB=0xff;
    ch_led_data=0x00;
    filter_flag=0x00;
    phase_data=0x01;
  }
    
  /*
    //////////write check
    rom_write_multi();
    DelayTime_ms(10);  //5msec
    
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
      DelayTime_ms(10);  //5msec
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
  
  volume_set();
  DelayTime_ms(10);  //5msec
  phase_ess();
  DelayTime_ms(10);  //5msec
  ess_filter();
  
//Dot-matrix & LED Display
//Dot-matrix
  DOT_CLR_FLAG = 1;		//remove CLR(dot-matrix)
  dot_light_reg=1;			//DOT_CLR_FLAG = 1 -> dot_light_reg=1

  //bright = (0 = 100%), (1 = 80%), (2 = 53%), (3 = 40%), (4 = 27%), (5 = 20%), (6 = 13%), (7 = 0%)
  
  DOT_ADDR4=1;
  dot_matrix_bright(3);		//Bright 27%  
  
  PORTB_Bit7=1;       //Analog Power Enable
/*  
 for(i=0; i<0x30; i++) reset_eeprom(i);
 for(i=0; i<0x20; i++)  reset_eeprom(i+0x7e0);

//test check
  for(i=0; i<0x80; i++)     test_eeprom[i]=rom_I2C_Read(i);
  for(i=0; i<0x20; i++)     test_eeprom[i+0x80]=rom_I2C_Read(i+0x7e0);
  */
  //for(i=0; i<0x20; i++)     test_eeprom[i]=rom_I2C_Read(i);
}


