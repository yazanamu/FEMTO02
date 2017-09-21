#include <iom128.h>
#include <inavr.h>
#include <string.h>
#include "define.h"
#include "uart.h" // added by jang 2017.9.15
#include "es9038.h"     // added by jang 2017.9.16
#include "initboard.h"  // added by jang 2017.9.18
#include "main_func.h"  // added by jang 2017.9.19
#include "dot_matrix_func.h" // added by jang 2017.9.19
#include "ak4118a.h" // added by jang 2017.9.19

unsigned char flag_usb_audio=1,         flag_usb_audio_before=1;
unsigned char flag_dsd128=1,            flag_dsd128_before=1;
unsigned char flag_dsd_on=1,            flag_dsd_on_before=1;
unsigned char flag_usb_detect=0,        flag_usb_detect_before=0;
unsigned char flag_key_int=0;
unsigned char flag_mute=0,              flag_mute_before=0;
enum front_key { KEY_LEFT=1, KEY_VOLUP, KEY_MUTE, KEY_RIGHT, KEY_VOLDOWN, KEY_INVERSE, KEY_FILTER };

extern unsigned char AK4118A_read_register(unsigned char devaddr, unsigned char regaddr);
extern void send_string(char *p);
extern int Init_UART0(unsigned long baud);
extern void send_integer(unsigned char ch);
extern void send_int2hex(unsigned char ch);
extern void es9038_set_volume(unsigned char devaddr,unsigned char volume_db);

U16 rom_add_pt=0;
U32 rom_cnt=0;
U8 rom_cnt_high=0;
U8 rom_cnt_mid=0;
U8 rom_cnt_low=0;
U8 rom_func_data=0;
U8 rom_add_pt_high=0;
U8 rom_add_pt_low=0;
U8 rom_device_high=0;
U8 rom_device_low=0;
U8 rom_add_check_sum=0;


U8 phase_data=1;

U8 interrupt_eicra=0xB5;
U8 init_setting_check=0;    //include remocon interrupt,
//U32 test_yu=0;

U8 ess_automute=0;
U8 ess_lock_ck=0;
U8 old_ess_automute=0;
U8 old_ess_lock_ck=0;
U8 exMute=0;
U8 old_exMute=0;
U16 key_tmr=0;
U8 tmr_osc_ck=0;
//U8 tmr_osc_ck2=0;
U8 key_int_flag=0;
U16 rom_tmr=0;
U8 rom_save_flag=1;
U8 rom_save_flag2=1;

//U8 key_enable=1;
//U8 remocon_enable=1;


//timer
U8 Time_500us=0;
U16 Time_50ms=0;

//display
U8 dot_light_reg=1;
U8 ch_led_data=0;		//ch0(coax1)
U8 ch_led_enable=1;
U8 sr_led_data=6;                 //sr = 0
U8 display_num=100;
//U32 audio_level=0x7fffffff;
//U8 led_start_flag=0;


//volume
U8 ess_lch_master_trim=0;
U8 ess_rch_master_trim=0;
unsigned char vol_dB=ES9038_MAX_VOLUME, vol_dB_before=ES9038_MAX_VOLUME;
//U8 ch0_vol_dB=0;
//U8 ch1_vol_dB=0;
//U8 ch2_vol_dB=0;
//U8 ch3_vol_dB=0;
//U8 ch4_vol_dB=0;
//U8 ch5_vol_dB=0;
//U8 ch6_vol_dB=0;
//U8 ch7_vol_dB=0;

U8 i2s_flag=1;
U8 usb_sr=48; 		//USB 48=48kHz affiliation,  44=44.1kHz affiliation

U8 button_data=0;
U8 filter_flag=0;
U8 key_func=0;
U16 key_func_tmr=0;
U8 key_condition=0;


//es9018
U8 es9018_reg10=0xce;		//0xcf = mute, 0xce = unmute
U8 es9018_lock=0;
U8 es9018_lock_flag=0;
U16 dac_audio_timer=0;
U16 sample_rate=0;
U16 sample_rate2=0;

U16 min44=0x8CE;
U16 max44=0x937;

U16 min48=0x9A0;
U16 max48=0xA09;

U16 min88=0x11D1;
U16 max88=0x123A;

U16 min96=0x1374;
U16 max96=0x13DD;

U16 min176=0x23D7;
U16 max176=0x243F;

U16 min192=0x271D;
U16 max192=0x2786;

U8 tmr_osc=0;

//CS8416
U8 interface_ch=0x80;
U16 cs_audio_timer=0;

//XMOS
U16 usb_audio_timer=0;


U8 KeyReady=1;

U8 I2C_error=0;

U8 ch_name[8][6] = {
  "COAX1 ",
  "COAX2 ",
  " AES1 ",
  " AES2 ",
  " BNC  ",
  " OPT1 ",
  " OPT2 ",
  " USB  "
};
U8 sr_name[7][6] = {
  " 44.1 ",
  " 48.0 ",
  " 88.2 ",
  " 96.0 ",
  "176.4 ",
  "192.0 ",
  "      "
};
U8 filter_name[3][8] = {
  "FILTER1 ",
  "FILTER2 ",
  "FILTER3 "
};
U8 phase_name[8]={" INVERSE"};
U8 normal_name[8]={"  NORMAL"};

U8 ch_data;
U8 sr_data;
U8 filter_data;
U8 inverse_data;

U8 dot_string[16];
U8 ess_volume[4];


//sr_minphase_stage1 & sr_minphase_stage2,  Slow Roll-off
S32 f1_coeff_st1[64]={
0
,1656381
,9659303
,34409768
,93398781
,209942799
,407444422
,699022478
,1074874807
,1492352005
,1874902824
,2124488166
,2147483647
,1887806213
,1355685389
,639107586
,-110755555
,-719588677
,-1044031603
,-1020728165
,-690754479
,-187854371
,307259942
,630274318
,690105478
,497041076
,151573293
,-200494787
,-427342979
,-460267547
,-313535895
,-69239327
,163487939
,295508341
,290781514
,173357845
,8625355
,-129830628
,-191689217
,-166581017
,-81244643
,18370414
,89760574
,110702807
,84163673
,31575802
,-20180401
,-51101617
,-54545855
,-36486473
,-9671202
,13456521
,25681054
,26272207
,19136308
,9640102
,1930723
,-2245673
,-3249107
,-2468805
,-1263225
,-407167
,0
,0
};

S32 f1_coeff_st2[16]={
1475681183
,1475681183
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
};          

//sr_linphase_stage1 & sr_linphase_stage2,  Slow Roll-off
S32 f2_coeff_st1[64]={
0
,-1801695
,-4008643
,-5121013
,-2214480
,5996574
,16606227
,22319443
,15460455
,-5254559
,-30080590
,-41104893
,-23628756
,20097715
,65638314
,77510493
,33703685
,-51685218
,-126749606
,-129504345
,-32841829
,122779073
,237746372
,208859258
,4955037
,-285795146
,-476804265
,-374025116
,107471796
,869797120
,1652694820
,2147483647
,2147483647
,1652694820
,869797120
,107471796
,-374025116
,-476804265
,-285795146
,4955037
,208859258
,237746372
,122779073
,-32841829
,-129504345
,-126749606
,-51685218
,33703685
,77510493
,65638314
,20097715
,-23628756
,-41104893
,-30080590
,-5254559
,15460455
,22319443
,16606227
,5996574
,-2214480
,-5121013
,-4008643
,-1801695
,0
};

S32 f2_coeff_st2[16]={
2147400132
,2147400132
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
};


void key_scan(void);
void port_scan(void);
void flag_scan(void);
void DelayTime(unsigned int time_end) { while(time_end--); }

void DelayTime_ms(unsigned int time_end){  //msec
  unsigned int i, k;
  for(i=0; i<=time_end+1; i++) { k=16000;while(k--); } //1msec
}


///////////////////////////////////////////////////////////////////////////////
void sample_rate_cal(){
  //U8 temp;
  
  //if ( (old_ess_lock_ck!=ess_lock_ck) || (old_ess_automute!=ess_automute) ) {
    
    //if (old_ess_lock_ck!=ess_lock_ck) old_ess_lock_ck=ess_lock_ck;
    //if (old_ess_automute!=ess_automute) old_ess_automute=ess_automute;
  
  if( ( (ch_led_data!=7) && ess_lock_ck) || (ess_lock_ck && !ess_automute) ) {
    send_string("[I2C] Read Sampling rate.\r\n"); 
    //sample_rate=I2C_Read(0x90, 31);
    sample_rate<<=8;
    //sample_rate+=I2C_Read(0x90, 30);
    //sample_rate2=I2C_Read(0x92, 31);
    sample_rate2<<=8;
    //sample_rate2+=I2C_Read(0x92, 30);
    send_string("[I2C] Complete reading Sampling rate.\r\n"); 
        //0=44.1kHz,    1=48kHz,    2=88.2kHz,    3=96kHz,    4=176.4kHz,   5=196kHz
  
        //192kHz
    if( (sample_rate>min192 && sample_rate<max192) || \
        (sample_rate2>min192 && sample_rate2<max192) )	sr_led_data=5;
    
        //176.4kHz
    else if( (sample_rate>min176 && sample_rate<max176) || \
             (sample_rate2>min176 && sample_rate2<max176) )	sr_led_data=4;
        
        //96kHz
    else if( (sample_rate>min96 && sample_rate<max96) || \
             (sample_rate2>min96 && sample_rate2<max96) )	sr_led_data=3;
    
        //88.2kHz
    else if( (sample_rate>min88 && sample_rate<max88) || (sample_rate2>min88 && sample_rate2<max88) )	sr_led_data=2;
    
        //48kHz
    else if( (sample_rate>min48 && sample_rate<max48) || (sample_rate2>min48 && sample_rate2<max48) )	sr_led_data=1;
    
        //44.1kHz
    else if( (sample_rate>min44 && sample_rate<max44) || (sample_rate2>min44 && sample_rate2<max44) )	sr_led_data=0;
    
    else sr_led_data=6;	//sr led off
  }
  else sr_led_data=6;	//sr led off
  //}
  
  
  if(sr_led_data==6){
    exMute=1;
  }
  else exMute=0;
 
  if(exMute!=old_exMute) {
    old_exMute=exMute;
   
    //if(!old_exMute) temp=vol_dB;
    //else temp=224;    //-114dB
      
    //for(i=0; i<8; i++){
    //  I2C_Write(0x90,i,temp);	//Lch volume of DAC0
    //  I2C_Write(0x92,i,temp);	//Rch volume of DAC0
    //}
    //es9038_set_volume(ES9038_ADDR0,temp);
    //es9038_set_volume(ES9038_ADDR1,temp);
  }
  
}



//ES9018 Automute check
//Any logical change
//active L : unmute, sample rate on
//active H : mute,  sample rate off
#pragma vector = INT7_vect
__interrupt void INT7_Handler(void)
{
 if(tmr_osc_ck){
  if(AUTOMUTE) ess_automute=1; 			//mute,  sample rate off
  else ess_automute=0;            	                            //unmute, sample rate on
  
  sample_rate_cal();
 }
}


//ESS LOCK-Lch
//Any logical change
//high : lock,		low : unlock
#pragma vector = INT6_vect
__interrupt void INT6_Handler(void)
{
 if(tmr_osc_ck){
  if(ESS_LOCK) ess_lock_ck=1;
  else ess_lock_ck=0;
  
  sample_rate_cal();
 }
}
/*
U8 non_audio_check=0;
//U8 old_non_audio=0;
U8 non_audio_tmr=0;
//Non_audio check
//Any logical change
//high : non_audio -> sdout: muted,		low : audio -> sdout : not muted
#pragma vector = INT4_vect
__interrupt void INT4_Handler(void)
{
  U8 temp;
  non_audio_check=1;
  non_audio_tmr=0;
  
  temp=I2C_Read(0x20, 0x0b);
  if((temp&0x20)==0x20) I2C_Write(0x20, 0x01, 0xc4);    //muted
  else I2C_Write(0x20, 0x01, 0x84);    //not muted
  //if(PINE_Bit4) old_non_audio=1;
  //else I2C_Write(0x20, 0x01, 0x84);    //not muted
}
*/

//Button Key
// falling edge
#pragma vector = INT3_vect
__interrupt void INT3_Handler(void)     // falling edge
{
  flag_key_int=1;
}
/*
//if(PINB_Bit6==0) PORTB_Bit7=0;       //Analog Power Disable
 #pragma vector = INT2_vect
__interrupt void INT2_Handler(void)
{
  PORTB_Bit7=0;       //Analog Power Disable
}
*/
///////////////////////////////////////////////////////////////////////////////

U16 led_tmr=0;
U8 init_sr_led=1;

#pragma vector = TIMER0_OVF_vect
__interrupt void TIMER0_OVF_Handler(void)
{
  U8 i;
  
  TCNT0=255-250; //500usec 
  // 16MHz / 32prescaling = 500kHz
  // T= 1 / 500kHz = 2usec
  // 2usec X 250 = 500usec
  
  Time_500us++;  
 // if(mtime_flag) mtime_length++;
  
  if(tmr_osc_ck){
    if(key_func){   //filter or inverse f();  
      if(key_func_tmr>6000) { key_func=0; key_condition=0; }
      
      key_func_tmr++;
    }
    
    else {         // channel and sample rate, volume
      if(led_tmr==200){   //0.1sec      
        for(i=0; i<16; i++){
          if(i<6) dot_string[i]=ch_name[ch_led_data][i];
          else if(i<12) dot_string[i]=sr_name[sr_led_data][i-6];
          else  dot_string[i]=ess_volume[i-12];
        }
        dot_string_digit();
        //for(i=0; i<5; i++)        dot_matrix_digit ( ch_name[ch_led_data][i],i ); 
        //dot_vol_hextodeci(vol_dB);
      }
      
      else if(led_tmr==800){  //sample rate,  0.4sec
        old_ess_automute=ess_automute;
        old_ess_lock_ck=ess_lock_ck;
        
        if(AUTOMUTE) ess_automute=1; 			//mute,  sample rate off
        else ess_automute=0;            	                            //unmute, sample rate on
      
        if(ESS_LOCK) ess_lock_ck=1;
        else ess_lock_ck=0;
      
        if( (old_ess_automute!=ess_automute) || (old_ess_lock_ck!= ess_lock_ck))  sample_rate_cal();
        
        //for(i=0; i<2; i++)       dot_matrix_digit ( sr_name[sr_led_data][i],i+6 );
        //for(i=2; i<5; i++)       dot_matrix_digit2 ( sr_name[sr_led_data][i],i-2 );
      
        led_tmr=0;
      }
      led_tmr++;  
    }// end else 
   
    
  }
}
U8 tmr_flag_ck=1;
//Board Timer
#pragma vector = TIMER1_OVF_vect
__interrupt void TIMER1_OVF_Handler(void)
{
U8 data=0;
//U16 temp;
  //TCNT1=0xffff-1563; //0.1sec 
  TCNT1=0xffff-781; //0.05sec 
  // 16MHz / 1024precaling = 15.625kHz
  // T= 1 / 15.625Hz = 64usec
  // 64usec X 1563 = 0.1sec
  // 64usec X 781 = 50msec
  
  //test
  if(tmr_osc) {
    if(tmr_osc<31) tmr_osc++;
    if(tmr_osc==10) {   //0.5sec
      //_system_init_1();
      _system_init_se();
    }
    /*else if(tmr_osc==60) {    //3sec
      _system_init_se();
    }*/
    else if(tmr_osc==30) {    //1.5sec
      //_system_init_1();
      es9018_reg10=0xce;			
      //I2C_Write(0x90, 0x0a, es9018_reg10);
      //I2C_Write(0x92, 0x0a, es9018_reg10);
      init_vol_dn(vol_dB);
      DelayTime_ms(50);  //50msec
      //init_vol(vol_dB);
      dot_vol_hextodeci(vol_dB);
      tmr_osc_ck=1;
      init_setting_check=1;   //include remocon interrupt,
    }
    
    if(tmr_osc_ck){
      
      if(!KEY_FLAG){
        if(key_int_flag==1 && key_tmr>5) key_int_flag=2;
        else if(key_int_flag==2 && key_tmr>6){ //key_tmr=400 : 0.05sec
 
          data=KEY_DATA2;
          data<<=1;
          data+=KEY_DATA1;
          data<<=1;
          data+=KEY_DATA0;
          
          if(data==2) {
            audio_level_up();                            //Master Volume Up
          }
          else if(data==5){
            audio_level_down();                        //Master Volume Down
          }
          
        }//end else if
        tmr_flag_ck=0;
        key_tmr++;
      }
      else { 
        key_int_flag=0; key_tmr=0; 
        if(tmr_flag_ck==0) {
          tmr_flag_ck=1;
          rom_tmr=0;
        }
      }
      /*
      if(IR_data_flag ){
      //if(IR_data_flag && Time_50ms>1){
        //Time_50ms=0;
        //IR_data[IR_data_flag]=_remocon_data;

        if(IR_data[IR_data_flag]==0x80) {                             //Master Volume Up
          //if(new_repeat_flag) { for(test_i=0; test_i<3; test_i++) audio_level_up(); }
          //else audio_level_up();
          if(IR_data[IR_data_flag]==0x80 && IR_data[IR_data_flag+1]==0x80) {
            audio_level_sp_up();
            IR_data_flag--;
          }
          else audio_level_up();
        }
        else if(IR_data[IR_data_flag]==0xA8) {                         //Master Volume Down
          //if(new_repeat_flag) { for(test_i=0; test_i<3; test_i++) audio_level_down();}
          //else audio_level_down();
          if(IR_data[IR_data_flag]==0xA8 && IR_data[IR_data_flag+1]==0xA8) {
            audio_level_sp_down();
            IR_data_flag--;
          }
          else audio_level_down();
        }
          
        IR_data_flag--;
      }
    */  
      
      
    }
  }		
	
  Time_50ms++;
	
}

///////////////////////////////////////////////////////////////////////////////
//U8 non_audio_flag=0;6


void main(void){
  
  __enable_interrupt();
  Init_UART0(57600);    // added by jang 2017.9.15
  send_string("\r\nSystem started.\r\n");
  send_string("System initializing......\r\n");
  _system_init();
  //_system_init_1();
  send_string("System init Completed.\r\n");

while(1){
    //if(PINB_Bit6==0) PORTB_Bit7=0;       //Analog Power Disable
    port_scan();
    flag_scan();
    if (flag_key_int) {
      flag_key_int=0;
      key_scan();
    }
  }//end while
}

void key_scan(void)
{
  unsigned char key_data=0;

  key_data = (INKEY1_READ & INKEY1_READ_MASK) >> INKEY1_READ_LOC;
  send_string("Key code = ");
  send_int2hex(key_data);
  send_string("\r\n");
  if(tmr_osc_ck)
    switch(key_data) {
      case KEY_LEFT:
        channel_down();
        break;
      case KEY_VOLUP:
        audio_level_up();
        break;
      case KEY_MUTE:
        ess_mute();
        break;
      case KEY_RIGHT:
        channel_up();
        break;
      case KEY_VOLDOWN:
        audio_level_down();
        break;
      case KEY_INVERSE:
        if(!key_condition) key_func=1; else key_func=3;
        femto_function();
        break;
      case KEY_FILTER:
        if(!key_condition) key_func=2; else key_func=4;
        femto_function();
    } // end of switch
}

void flag_scan(void)
{
  if(flag_mute != flag_mute_before) {
    flag_mute_before=flag_mute;
    if (flag_mute) send_string("[ES9038] Mute on.\r\n");
    else send_string("[ES9038] Mute off.\r\n");
  }
  if(vol_dB!=vol_dB_before) {
    if (vol_dB>vol_dB_before) send_string("[ES9038] Volume decresed. - ");
    else send_string("[ES9038] Volume incresed. - ");
    send_integer(vol_dB); send_string("\r\n");
    vol_dB_before=vol_dB;
    es9038_set_volume(ES9038_ADDR0,vol_dB);
    es9038_set_volume(ES9038_ADDR1,vol_dB);
  }
}

void port_scan(void)
{
  //detect USB Audio
  if(UA_EN_READ & UA_EN_PIN) flag_usb_audio=1; else flag_usb_audio=0;
  if(flag_usb_audio & !flag_usb_audio_before) {         // edge detect
    flag_usb_audio_before=1;
    send_string("[SA9127] USB Audio Activated.\r\n");
  }
  if(!flag_usb_audio & flag_usb_audio_before) {         // edge detect
    flag_usb_audio_before=0;
    send_string("[SA9127] USB Audio Deactivated.\r\n");
  }
  // detect dsd on
  if(DSD_ON_READ & DSD_ON_PIN) flag_dsd_on=1; else flag_dsd_on=0;
  if(flag_dsd_on & !flag_dsd_on_before) {         // edge detect
    flag_dsd_on_before=1;
    send_string("[SA9127] DSD mode detected.\r\n");
  }
  if(!flag_dsd_on & flag_dsd_on_before) {         // edge detect
    flag_dsd_on_before=0;
    send_string("[SA9127] PCM mode detected.\r\n");
  }
  // dsd128 detect
  if(DSD128_READ & DSD128_PIN) flag_dsd128=1; else flag_dsd128=0;
  if(flag_dsd128 & !flag_dsd128_before) {         // edge detect
    flag_dsd128_before=1;
    send_string("[SA9127] DSD 128 detected.\r\n");
  }
  if(!flag_dsd128 & flag_dsd128_before) {         // edge detect
    flag_dsd128_before=0;
    send_string("[SA9127] DSD 64 detected.\r\n");
  }
  // detect usb connected
  if(USB_DET_PORT & USB_DET_PIN) flag_usb_detect=1; else flag_usb_detect=0;
  if(flag_usb_detect & !flag_usb_detect_before) {         // edge detect
    flag_usb_detect_before=1;
    send_string("[SA9127] USB is in normal mode.\r\n");
  }
  if(!flag_usb_detect & flag_usb_detect_before) {         // edge detect
    flag_usb_detect_before=0;
    send_string("[SA9127] USB is in suspended.\r\n");
  } 
}

