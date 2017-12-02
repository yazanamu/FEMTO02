#include <iom128.h>
#include <inavr.h>
#include <string.h>
#include "define.h"
#include "uart.h"               // added by jang 2017.9.15
#include "es9038.h"             // added by jang 2017.9.16
#include "initboard.h"          // added by jang 2017.9.18
#include "main_func.h"          // added by jang 2017.9.19
#include "dot_matrix_func.h"    // added by jang 2017.9.19
#include "ak4118a.h"            // added by jang 2017.9.19
#include "at24c16.h"            // added by jang 2017.10.22

#define AK4118A_I2C_ADDR AK4118A_I2C_ADDRESS(0)

unsigned long system_clock=0;
unsigned char flag_usb_audio=1,         flag_usb_audio_before=1;
unsigned char flag_dsd128=1,            flag_dsd128_before=1;
unsigned char flag_dsd_on=1,            flag_dsd_on_before=1;
unsigned char flag_usb_detect=1,        flag_usb_detect_before=1;
unsigned char flag_ak4118a_int0=0;
unsigned char flag_ak4118a_int1=0;
unsigned char flag_key_int=0;
unsigned int  flag_longkey_count=0;
unsigned int  flag_longkey=0;
unsigned char flag_voluplongkey=0;
unsigned char flag_voldownlongkey=0;
unsigned char flag_mute=0,              flag_mute_before=0xFF;
unsigned char flag_line_relay_on=0, flag_line_relay_on_before=1;
unsigned char flag_hp_relay_on=0, flag_hp_relay_on_before=1;
enum front_key { KEY_LEFT=1, KEY_VOLUP, KEY_MUTE, KEY_RIGHT, KEY_VOLDOWN, \
                 KEY_INVERSE, KEY_FILTER };
unsigned char flag_need_display_update=1;
unsigned char flag_need_save_eeprom=1;
unsigned char flag_input_mode=0, flag_input_mode_before=0xFF;
unsigned char flag_filter=0, flag_filter_before=0xFF; // Filter mode F1~F7 
                    // F1:Fast Roll-Off, Minimum Phase Filter
                    // F2: Apdizing, Fast Roll-Off, Linear Phase Filter
                    // F3: Fast Roll-Off, Linear Phase Filter
                    // F4: Slow Roll-Off, Linear Phase Filter
                    // F5: Slow Roll-Off, Minimum Phase Filter
                    // F6: Hybrid, Fast Roll-Off, Minimum Phase Filter
                    // F7: Brickwall Filter
unsigned char flag_headphone_output=0, flag_headphone_output_before=0xFF; // H=Headphone out, L=Line out
unsigned char flag_timer_read_sampling_rate=0;
unsigned int flag_sampling_rate=0, flag_sampling_rate_before=0xFF;
unsigned int flag_first_display=60000;    // delay initial display
unsigned char flag_first_display_complete=0;
unsigned char flag_amli=0,flag_amli_before=1;
unsigned char flag_es9038_left_locked=0, flag_es9038_right_locked=0;


enum input_mode { MODE_COAX1, MODE_COAX2, MODE_AES1, MODE_OPT1, MODE_OPT2, \
                  MODE_OPT3, MODE_OPT4, MODE_USB };

unsigned char flag_refresh_display=0;

unsigned char flag_timer_100ms=0;
unsigned char determine_sampling_rate(void);

extern unsigned char AK4118A_read_register(unsigned char devaddr, unsigned char regaddr);
extern void send_string(char *p);
extern void send_int2hex(unsigned int value);
extern int Init_UART0(unsigned long baud);
extern void send_integer(unsigned char ch);
extern void send_byte2hex(unsigned char ch);
extern void send_long2hex(unsigned long value);
extern void es9038_set_volume(unsigned char devaddr,unsigned char volume_db);
extern unsigned long es9038_read_dpll_number(unsigned char devaddr);
extern void es9038_set_filter_shape(unsigned char devaddr, unsigned char filter);
extern void es9038_soft_reset(unsigned char devaddr);
extern void es9038_dac_mute_off(void);
extern void es9038_dac_mute_on(void);
extern unsigned char es9038_read_lock_status(unsigned char devaddr);
extern unsigned char es9038_read_volume(unsigned char devaddr);
extern void es9038_set_soft_start_time(unsigned char devaddr, unsigned char time);

extern void AK4118A_power_down(unsigned char devaddr);
extern unsigned char AK4118A_input_select(unsigned char devaddr, unsigned char channel);
extern unsigned char AK4118A_read_current_channel(unsigned char devaddr);
extern void AK4118A_BCU_enable(unsigned char devaddr);
extern void AK4118A_BCU_disable(unsigned char devaddr);
extern void display_dot_matrix(unsigned char ch, unsigned int sr,unsigned char volume,\
                        unsigned char filter,unsigned char headphone,\
                        unsigned char first_display,\
                        unsigned char mute, unsigned char dsdon);

extern unsigned char write_identity(void);
extern unsigned char eeprom_save(unsigned char mode,\
                          unsigned char line_vol, unsigned char headphone_vol,\
                          unsigned char mute, unsigned char filter, \
                          unsigned char output_headphone);

unsigned int read_sampling_rate(unsigned char mode, unsigned char dsd_on, unsigned char amli);

extern unsigned char need_display_update(char *str1, char *str2);
extern unsigned int message_delay;
extern char display_default_strings[17];
extern char display_message_strings[17];
extern char *dot_strings;
extern char *display_ess_volume;
extern char *ch_name[8];
extern char *sr_name[7];

extern unsigned char IR_data_flag;
extern unsigned char IR_data[20];
extern int _remocon_data;
extern unsigned char new_repeat_flag;


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
U8 sr_led_data=0, sr_led_data_before=0;                 //sr = 0
U8 display_num=100;
//U32 audio_level=0x7fffffff;
//U8 led_start_flag=0;


//volume
U8 ess_lch_master_trim=0;
U8 ess_rch_master_trim=0;
unsigned char vol_dB=ES9038_MAX_VOLUME, vol_dB_before=0;
unsigned char vol_dB_HP=ES9038_MAX_VOLUME, vol_dB_HP_before=0;

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


U8 ch_data;
U8 sr_data;
U8 filter_data;
U8 inverse_data;



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
void es9038_usb_start_playback(void);
void es9038_usb_stop_playback(void);
void DelayTime(unsigned int time_end) { while(time_end--); }

void DelayTime_ms(unsigned int time_end){  //msec
  unsigned int i, k;
  for(i=0; i<=time_end+1; i++) { k=16000;while(k--); } //1msec
}


///////////////////////////////////////////////////////////////////////////////
void sample_rate_cal(){
/*
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
*/
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
unsigned char key_code=0;
#pragma vector = INT3_vect
__interrupt void INT3_Handler(void)     // inkey1 falling edge
{
  if (INKEY1_READ & INKEY1_PIN) {       // if key pressed (low Active)
    flag_longkey_count=0;
  }
  else {
    flag_key_int=1;
    flag_longkey_count=1;
  }
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

/*
//AMLI Function
// AMLI -> PE6
#pragma vector = INT6_vect
__interrupt void INT6_Handler(void)     // logic edge
{
  if (AMLI_READ & AMLI_PIN) flag_amli=1;
  else flag_amli=0;
}
*/


//Urgent Mute Function
// UA_EN -> PE7
#pragma vector = INT7_vect
__interrupt void INT7_Handler(void)     // logic edge
{
  /*
  if (flag_input_mode==MODE_USB) {
    if ((UA_EN_READ & UA_EN_PIN)==0) { // Low edge
      HP_MUTE_DDR_INIT;   HP_MUTE_ON;
      //flag_hp_relay_on = 0; flag_hp_relay_on_before =1;       // refresh
      LINE_MUTE_DDR_INIT; LINE_MUTE_ON;
      //flag_line_relay_on = 0; flag_line_relay_on_before =1;   // refresh
    }
    
    else {                             // High edge
      if (flag_headphone_output) {      // headphone
        flag_hp_relay_on = 1; flag_hp_relay_on_before =0;     // refresh
        HP_MUTE_DDR_INIT;   HP_MUTE_OFF;
        DAC_MUTE_OFF;
        es9038_dac_mute_off();
      } 
      else {                          // line-out
        flag_line_relay_on = 1; flag_line_relay_on_before =0; // refresh
        LINE_MUTE_DDR_INIT; LINE_MUTE_OFF;
        DAC_MUTE_OFF;
        es9038_dac_mute_off();
      }
    }
  }
*/  
}

U16 led_tmr=0;
U8 init_sr_led=1;

#pragma vector = TIMER0_OVF_vect
__interrupt void TIMER0_OVF_Handler(void)
{
  //U8 i;
  
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
        //for(i=0; i<16; i++){
        //  if(i<6) dot_string[i]=ch_name[flag_input_mode][i];
        //  else if(i<12) dot_string[i]=sr_name[sr_led_data][i-6];
        //  else  dot_string[i]=display_ess_volume[i-12];
        //}
        flag_timer_100ms=1;
        
        //dot_string_digit();
        //for(i=0; i<5; i++)        dot_matrix_digit ( ch_name[ch_led_data][i],i ); 
        //dot_vol_hextodeci(vol_dB);
      }
      
      else if(led_tmr==800){  //sample rate,  0.4sec
        led_tmr=0;
        //old_ess_automute=ess_automute;
        //old_ess_lock_ck=ess_lock_ck;
        //if(AUTOMUTE) ess_automute=1; 			//mute,  sample rate off
        //else ess_automute=0;            	                            //unmute, sample rate on
        //if(ESS_LOCK) ess_lock_ck=1;
        //else ess_lock_ck=0;
        //if( (old_ess_automute!=ess_automute) || (old_ess_lock_ck!= ess_lock_ck))  sample_rate_cal();
        
        if (!flag_mute) flag_timer_read_sampling_rate=1;        // 2017.10.22
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
  unsigned char test_i;
//U8 data=0;
//U16 temp;
  //TCNT1=0xffff-1563; //0.1sec 
  //TCNT1=0xffff-781; //0.05sec 
  // 16MHz / 1024prescaling = 15.625kHz T = 1/15.625kHz = 64us 64usec X 1563 = 0.1sec 64usec X 781 = 50msec
  // 16MHz / 256 prescaling = 62.5kHz T = 1/62.5kHz = 16 us n=10000/16=625
  TCNT1=0xffff-625;  //10ms
  if ((INKEY1_READ & INKEY1_PIN)==0) { // key holding
    flag_refresh_display=1; // refresh display
    if (flag_longkey_count) {
      flag_longkey_count++;
      if ((flag_longkey_count%25)==0) flag_longkey=1; // every 0.25s
      //if ((flag_longkey_count>150)&&(flag_longkey_count%5)==0) flag_longkey=1; // every 0.1s
      if ((flag_longkey_count>50)&&(flag_longkey_count%5)==0) flag_longkey=1; // every 0.1s
    }
  }
  else flag_longkey_count=0;
  
  if(tmr_osc) {
    /*
    if(tmr_osc<31) tmr_osc++;
    if(tmr_osc==10) {   //0.5sec
      //_system_init_1();
      _system_init_se();
    }
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
    */
    
    
    
    if(tmr_osc_ck){
      /*
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
      */
      ///////////////////////////////////////////////////////////////////
      if(IR_data_flag){
        if(IR_data_flag && Time_50ms>1){
          Time_50ms=0;
          IR_data[IR_data_flag]=_remocon_data;
          if(IR_data[IR_data_flag]==0x80) {   //Master Volume Up
            if(new_repeat_flag) { 
              for(test_i=0; test_i<3; test_i++) {
                //audio_level_up();
                send_string("[IR] Remocom Volume up.1\r\n");
              } 
            }
            else {
              //audio_level_up();
              send_string("[IR] Remocom Volume up.2\r\n");
            }
            if(IR_data[IR_data_flag+1]==0x80) {
              //audio_level_sp_up();
              send_string("[IR] Remocom Volume up.3\r\n");
              IR_data_flag--;
            }
            else {
              //audio_level_up();
              send_string("[IR] Remocom Volume up.4\r\n");
            }
          }
          else if(IR_data[IR_data_flag]==0xA8) {   //Master Volume Down
            if(new_repeat_flag) { 
              for(test_i=0; test_i<3; test_i++) {
                //audio_level_down();
                send_string("[IR] Remocom Volume down.1\r\n");
              }
            }
            else {
              //audio_level_down();
              send_string("[IR] Remocom Volume down.2\r\n");
            }
            if(IR_data[IR_data_flag+1]==0xA8) {
              //audio_level_sp_down();
              send_string("[IR] Remocom Volume down.3\r\n");
              IR_data_flag--;
            }
            else {
              //audio_level_down();
              send_string("[IR] Remocom Volume down.4\r\n");
            }
          }
          IR_data_flag--;
        }
      }
    }
    Time_50ms++;
  }
}
///////////////////////////////////////////////////////////////////////////////
//U8 non_audio_flag=0;6


void main(void)
{
  __enable_interrupt();
  Init_UART0(57600);    // added by jang 2017.9.15
  send_string("\r\nSystem started.\r\n");
  send_string("System initializing......\r\n");
  _system_init();
  //_system_init_1();
  send_string("System init Completed.\r\n");
  _system_init_se();
  send_string("System init se Completed.\r\n");
  
  tmr_osc_ck=1;
  init_setting_check=1;   //include remocon interrupt,
  if (flag_headphone_output) {
    flag_headphone_output_before=0;
  }
  else {
    flag_headphone_output_before=1;
  }
  //if (flag_input_mode==MODE_USB) {
  //  I2S_SEL_ON;
  //  send_string("USB Mode.\r\n");
  //}
  //else I2S_SEL_OFF;
  
  while(1){
    if (flag_first_display) flag_first_display--;
    if (flag_first_display_complete) { flag_need_display_update=1; flag_first_display_complete=0; }
    if (flag_first_display==1) flag_first_display_complete=1;
    
    //if(PINB_Bit6==0) PORTB_Bit7=0;       //Analog Power Disable
    port_scan();
    flag_scan();
  }//end while
}// end of MAIN

void key_scan(void)
{
  unsigned char key_data=0;
  //unsigned char i;

  key_data = (INKEY1_READ & INKEY1_READ_MASK) >> INKEY1_READ_LOC;
  send_string("Key code = ");
  send_byte2hex(key_data);
  send_string("\r\n");
    switch(key_data) {
      case KEY_LEFT:
        flag_need_display_update =1;
        flag_need_save_eeprom =1;
        flag_input_mode--;
        flag_input_mode&=0x07;
        channel_change();
        break;
        
      case KEY_VOLUP:
        flag_need_display_update =1;
        flag_need_save_eeprom =1;
        flag_voluplongkey = 1;
        if(flag_mute) ess_mute();
        if (flag_headphone_output) {
          if(vol_dB_HP>0) vol_dB_HP--;
          if(vol_dB_HP>199) vol_dB_HP=199;
          es9038_set_volume(ES9038_ADDR0,vol_dB_HP);   // set es9038
          es9038_set_volume(ES9038_ADDR1,vol_dB_HP);
        }
        else {
          if(vol_dB>0) vol_dB--;
          if(vol_dB>199) vol_dB=199;
          es9038_set_volume(ES9038_ADDR0,vol_dB);
          es9038_set_volume(ES9038_ADDR1,vol_dB);
        }
        dot_vol_hextodeci(vol_dB_HP);
        dot_vol_hextodeci(vol_dB);
        
      break;
        
      case KEY_MUTE:
        flag_need_display_update =1;
        flag_need_save_eeprom =1;
        ess_mute();
      break;
        
      case KEY_RIGHT:
        flag_need_display_update =1;
        flag_need_save_eeprom =1;
        flag_input_mode++;
        flag_input_mode&=0x07;
        channel_change();
      break;

      case KEY_VOLDOWN:
        flag_need_display_update =1;
        flag_need_save_eeprom =1;
        flag_voldownlongkey = 1;
        if(flag_mute) ess_mute();
        if (flag_headphone_output) {
          if(vol_dB_HP<199) vol_dB_HP++; else vol_dB_HP=0xFF;
          es9038_set_volume(ES9038_ADDR0,vol_dB_HP);   // set es9038
          es9038_set_volume(ES9038_ADDR1,vol_dB_HP);
        }
        else {
          if(vol_dB<199) vol_dB++; else vol_dB=0xFF;
          es9038_set_volume(ES9038_ADDR0,vol_dB_HP);
          es9038_set_volume(ES9038_ADDR1,vol_dB_HP);
        }
        dot_vol_hextodeci(vol_dB_HP);
        dot_vol_hextodeci(vol_dB);
      break;
        
      case KEY_FILTER:
        flag_need_display_update =1;
        flag_need_save_eeprom =1;
        flag_filter++;
        if (flag_filter>6) flag_filter=0;
        if      (flag_filter==0) {   // Filter No.1
          es9038_set_filter_shape(ES9038_ADDR0,FAST_ROLL_OFF_MIN_PHASE_FILTER);
          es9038_set_filter_shape(ES9038_ADDR1,FAST_ROLL_OFF_MIN_PHASE_FILTER);
          send_string("[I2C] ES9038 Filter 1 \r\n");
        }
        else if (flag_filter==1) {   // Filter No.2
          es9038_set_filter_shape(ES9038_ADDR0,APODIZING_FAST_ROLL_OFF_LINEAR_PHASE_FILTER);
          es9038_set_filter_shape(ES9038_ADDR1,APODIZING_FAST_ROLL_OFF_LINEAR_PHASE_FILTER);
          send_string("[I2C] ES9038 Filter 2 \r\n");
        }
        else if (flag_filter==2) {   // Filter No.3
          es9038_set_filter_shape(ES9038_ADDR0,FAST_ROLL_OFF_LINEAR_PHASE_FILTER);
          es9038_set_filter_shape(ES9038_ADDR1,FAST_ROLL_OFF_LINEAR_PHASE_FILTER);
          send_string("[I2C] ES9038 Filter 3 \r\n");
        }
        else if (flag_filter==3) {   // Filter No.4
          es9038_set_filter_shape(ES9038_ADDR0,SLOW_ROLL_OFF_LINEAR_PHASE_FILTER);
          es9038_set_filter_shape(ES9038_ADDR1,SLOW_ROLL_OFF_LINEAR_PHASE_FILTER);
          send_string("[I2C] ES9038 Filter 4 \r\n");
        }
        else if (flag_filter==4) {   // Filter No.5
          es9038_set_filter_shape(ES9038_ADDR0,SLOW_ROLL_OFF_MIN_PHASE_FILTER);
          es9038_set_filter_shape(ES9038_ADDR1,SLOW_ROLL_OFF_MIN_PHASE_FILTER);
          send_string("[I2C] ES9038 Filter 5 \r\n");
        }
        else if (flag_filter==5) {   // Filter No.6
          es9038_set_filter_shape(ES9038_ADDR0,HYBRID_FAST_ROLL_OFF_MIN_PHASE_FILTER);
          es9038_set_filter_shape(ES9038_ADDR1,HYBRID_FAST_ROLL_OFF_MIN_PHASE_FILTER);
          send_string("[I2C] ES9038 Filter 6 \r\n");
        }
        else if (flag_filter==6) {   // Filter No.7
          es9038_set_filter_shape(ES9038_ADDR0,BRICKWALL_FILTER);
          es9038_set_filter_shape(ES9038_ADDR1,BRICKWALL_FILTER);
          send_string("[I2C] ES9038 Filter 7 \r\n");
        }
        
        //send_string("AK4118A Resigter =\r\n");
        //for(i=0;i<=0x28;i++) {
        //  send_byte2hex(AK4118A_read_register(AK4118A_I2C_ADDR,i));
        //  if (((i+1)%8)==0) send_string("\r\n"); else send_string(" ");
        //}
        //send_int2hex(read_sampling_rate());
        send_long2hex(es9038_read_dpll_number(ES9038_ADDR0)); send_string("\r\n");
        break;
        
    case KEY_INVERSE:
      
        flag_need_display_update =1;
        flag_need_save_eeprom =1;
        if (flag_headphone_output) {
          flag_headphone_output=0;
          send_string("[MCU] Line out selected.\r\n");
        }
        else {
          flag_headphone_output=1;
          send_string("[MCU] Headphone out selected.\r\n");
        }
        //send_string("Current Volume = "); send_integer(es9038_read_volume(ES9038_ADDR0));
        //send_string("\r\n");
        
        break;
    
    } // end of switch
}

void flag_scan(void)
{
  unsigned char error;
  
  /////////////// flag_headphone_output or line_output
  if (flag_headphone_output!=flag_headphone_output_before) { //edge
    flag_headphone_output_before = flag_headphone_output;
    flag_need_display_update=1;
    flag_need_save_eeprom=1;
    if (flag_headphone_output) {        // headphone out
      es9038_set_volume(ES9038_ADDR0,vol_dB_HP);   // set es9038
      es9038_set_volume(ES9038_ADDR1,vol_dB_HP);
      dot_vol_hextodeci(vol_dB_HP);
      send_string("Headphone out Mode loaded.\r\n");
      flag_hp_relay_on=1; // set relay
      flag_line_relay_on=0; // set relay
      send_string("[MCU] Headphone out Relay selected.\r\n");
    }
    else {                              // line out
      es9038_set_volume(ES9038_ADDR0,vol_dB);
      es9038_set_volume(ES9038_ADDR1,vol_dB);
      dot_vol_hextodeci(vol_dB);
      send_string("Line out Mode loaded.\r\n");
      flag_line_relay_on=1; // set relay
      flag_hp_relay_on=0; // set relay
      send_string("[MCU] Line out Relay selected.\r\n");
    }
  }
  
  if (flag_amli!=flag_amli_before) {    // edge
    flag_amli_before=flag_amli;
    if (es9038_read_lock_status(ES9038_ADDR0)) {
      flag_es9038_left_locked=1;
      send_string("ES9038_LEFT_Locked.\r\n");
    }
    else flag_es9038_left_locked=0;
    if (es9038_read_lock_status(ES9038_ADDR1)) {
      flag_es9038_right_locked=1;
      send_string("ES9038_RIGHT_Locked.\r\n");
    }
    else flag_es9038_right_locked=0;
  }
   
  if (flag_key_int) {
    flag_key_int=0;
    send_string("Key interrupted.\r\n");
    if ((INKEY1_READ & INKEY1_PIN)==0) key_scan();
  }
  if (flag_longkey) {
    flag_longkey=0;
    send_string("Long Key interrupted.\r\n");
    if ((INKEY1_READ & INKEY1_PIN)==0) key_scan();
  }
  if (flag_refresh_display) {
    flag_refresh_display=0;
    //dot_string_digit();
  }
  if(flag_timer_100ms) {
    flag_timer_100ms=0;
    //flag_display_update=1;

  }
  if(flag_mute != flag_mute_before) {
    flag_mute_before=flag_mute;
    flag_need_display_update =1;
    flag_need_save_eeprom =1;
    if (flag_mute) send_string("[ES9038] Mute on.\r\n");
    else send_string("[ES9038] Mute off.\r\n");
  }
  if(vol_dB!=vol_dB_before) {
    flag_need_display_update =1;
    flag_need_save_eeprom =1;
    if (vol_dB>vol_dB_before) send_string("[ES9038] Volume decresed. - ");
    else send_string("[ES9038] Line ouput Volume incresed. - "); 
    send_integer(vol_dB); send_string("\r\n");
    vol_dB_before=vol_dB;
    es9038_set_volume(ES9038_ADDR0,vol_dB);
    es9038_set_volume(ES9038_ADDR1,vol_dB);
  }
  if(vol_dB_HP!=vol_dB_HP_before) {
    flag_need_display_update =1;
    flag_need_save_eeprom =1;
    if (vol_dB_HP>vol_dB_HP_before) send_string("[ES9038] Volume decresed. - ");
    else send_string("[ES9038] Headphone Volume incresed. - "); 
    send_integer(vol_dB_HP); send_string("\r\n");
    vol_dB_HP_before=vol_dB_HP;
    es9038_set_volume(ES9038_ADDR0,vol_dB_HP);
    es9038_set_volume(ES9038_ADDR1,vol_dB_HP);
  }
  
  if(flag_input_mode!=flag_input_mode_before) {
    flag_input_mode_before=flag_input_mode;
    flag_need_display_update =1;
    flag_need_save_eeprom =1;
 
    switch(flag_input_mode) {
      case MODE_COAX1:
        send_string("[I2C] AK4118A Wake up.\r\n");
        if (AK4118A_input_select(AK4118A_I2C_ADDR, MODE_COAX1)) \
          send_string("[I2C] COAX1 Selected.\r\n");
        else send_string("Comm error.\r\n");
        AK4118A_BCU_enable(AK4118A_I2C_ADDR);
        send_string("BCU enable.\r\n");
        SELECT_AK4118A;
        send_string("[MCU] I2S_SEL LOW.\r\n");
        flag_sampling_rate =0;
        break;
        
      case MODE_COAX2:
        if (AK4118A_input_select(AK4118A_I2C_ADDR, MODE_COAX2)) \
          send_string("[I2C] COAX2 Selected.\r\n");
        else send_string("Comm error.\r\n");
        SELECT_AK4118A;
        flag_sampling_rate =0;
        break;
        
      case MODE_AES1:
        if (AK4118A_input_select(AK4118A_I2C_ADDR, MODE_AES1)) \
          send_string("[I2C] AES1 Selected.\r\n");
        else send_string("Comm error.\r\n");
        SELECT_AK4118A;
        flag_sampling_rate =0;
        break;
        
      case MODE_OPT1:
        if (AK4118A_input_select(AK4118A_I2C_ADDR, MODE_OPT1)) \
          send_string("[I2C] OPT1 Selected.\r\n");
        else send_string("Comm error.\r\n");
        SELECT_AK4118A;
        flag_sampling_rate =0;
        break;
        
      case MODE_OPT2:
        if (AK4118A_input_select(AK4118A_I2C_ADDR, MODE_OPT2)) \
          send_string("[I2C] OPT2 Selected.\r\n");
        else send_string("Comm error.\r\n");
        SELECT_AK4118A;
        flag_sampling_rate =0;
        break;
        
      case MODE_OPT3:
        if (AK4118A_input_select(AK4118A_I2C_ADDR, MODE_OPT3)) \
          send_string("[I2C] OPT3 Selected.\r\n");
        else send_string("Comm error.\r\n");
        SELECT_AK4118A;
        flag_sampling_rate =0;
        break;
        
      case MODE_OPT4:
        send_string("[I2C] AK4118A Wake up.\r\n");
        if (AK4118A_input_select(AK4118A_I2C_ADDR, MODE_OPT4)) \
          send_string("[I2C] OPT4 Selected.\r\n");
        else send_string("Comm error.\r\n");
        AK4118A_BCU_enable(AK4118A_I2C_ADDR);
        send_string("BCU enable.\r\n");
        SELECT_AK4118A;
        send_string("[MCU] I2S_SEL LOW.\r\n");
        flag_sampling_rate =0;
      break;
        
      case MODE_USB:
        send_string("[I2C] AK4118A Power down.\r\n");
        AK4118A_input_select(AK4118A_I2C_ADDR, MODE_USB);       // AK4118A Off
        SELECT_USB;
        DAC_MUTE_OFF;
        AK4118A_BCU_disable(AK4118A_I2C_ADDR);
        send_string("BCU disable.\r\n");
        if (flag_headphone_output) flag_hp_relay_on=1; else flag_line_relay_on=1;
      break;
        
      default:
        
      break;
    }
  }

  if (flag_sampling_rate!=flag_sampling_rate_before) {
    flag_sampling_rate_before=flag_sampling_rate;
    flag_need_display_update=1;
  }

  if (flag_hp_relay_on!=flag_hp_relay_on_before){
    if (flag_input_mode==MODE_USB) {
      if (UA_EN_READ&UA_EN_PIN) {       // if UA_EN = ON
        flag_hp_relay_on_before = flag_hp_relay_on;
        if(flag_hp_relay_on) { 
          HP_MUTE_DDR_INIT; HP_MUTE_OFF;
          //LINE_MUTE_DDR_INIT; LINE_MUTE_ON;
          send_string("[MCU] Headphone out Relay changed at USB mode.\r\n");
        }
      }
    }
    else {                              // ak4118
      flag_hp_relay_on_before = flag_hp_relay_on;
      if(flag_hp_relay_on) { 
        HP_MUTE_DDR_INIT; HP_MUTE_OFF;
        //LINE_MUTE_DDR_INIT; LINE_MUTE_ON;
        send_string("[MCU] Headphone out Relay changed at AK4118 mode.\r\n");
      }
      
    }
  }

  if (flag_line_relay_on!=flag_line_relay_on_before){
    if (flag_input_mode==MODE_USB) {
      if (UA_EN_READ&UA_EN_PIN) {
        flag_line_relay_on_before = flag_line_relay_on;
        if(flag_line_relay_on) {
          HP_MUTE_DDR_INIT; HP_MUTE_ON;
          LINE_MUTE_DDR_INIT; LINE_MUTE_OFF;
          send_string("[MCU] Line out Relay changed at USB Mode.\r\n");
        }
        
      }
    }
    else {                              // ak4118
      flag_line_relay_on_before = flag_line_relay_on;
      if(flag_line_relay_on) { 
        HP_MUTE_DDR_INIT; HP_MUTE_ON;
        LINE_MUTE_DDR_INIT; LINE_MUTE_OFF; 
        send_string("[MCU] Line out Relay changed at AK4118 mode.\r\n");
      }
    }
  }

  if (flag_timer_read_sampling_rate==1) {
      flag_timer_read_sampling_rate=0;
      flag_sampling_rate = read_sampling_rate(flag_input_mode, flag_dsd_on, flag_amli);
      //send_string("[ES9038] Reading Sampling Rate.....\r\n");
  }

  if (flag_need_save_eeprom) {
    flag_need_save_eeprom=0;
    send_string("EEPROM Saving.......\r\n");
    error=eeprom_save(flag_input_mode,vol_dB,vol_dB_HP,flag_mute,flag_filter,flag_headphone_output);
    if (error!=SUCCESS) {
      send_string("EEPROM Save Error.......\r\nerror =");
      send_byte2hex(error); send_string("\r\n");
    }
    else send_string("EEPROM Save Complete.\r\n");
    flag_need_display_update=1;
  }
  
  if (flag_need_display_update) {
    flag_need_display_update=0;
    if (flag_headphone_output) \
      display_dot_matrix(flag_input_mode,flag_sampling_rate,vol_dB_HP,flag_filter,\
                         flag_headphone_output,flag_first_display,\
                         flag_mute, flag_dsd_on);
    else  \
      display_dot_matrix(flag_input_mode,flag_sampling_rate,vol_dB,flag_filter,\
                         flag_headphone_output,flag_first_display,\
                         flag_mute, flag_dsd_on);
  }

  
}

void port_scan(void)
{
  
  ///////////////////////////////////////
  ///////////detect UA_EN
  
  if(UA_EN_READ & UA_EN_PIN) flag_usb_audio=1; else flag_usb_audio=0;
  if(flag_usb_audio!=flag_usb_audio_before) {         // edge detect
    flag_usb_audio_before=flag_usb_audio;
    if(flag_usb_audio) {        // USB Audio Enable High
      ///*
      if (flag_input_mode==MODE_USB) {
        es9038_usb_start_playback();
        //if (flag_headphone_output) flag_hp_relay_on=1; else flag_line_relay_on=1;
        if (flag_headphone_output) HP_MUTE_OFF; else LINE_MUTE_OFF;
        /*
        if (flag_headphone_output) {
          flag_headphone_output_before=0;
          if(UA_EN_READ & UA_EN_PIN) HP_MUTE_OFF;
        }
        else {
          flag_headphone_output_before=1;  // refresh
          if(UA_EN_READ & UA_EN_PIN) LINE_MUTE_OFF;
        }
        */
      }
      else {
        es9038_usb_stop_playback();
      }
      send_string("[SA9127] USB Audio Activated.\r\n");
    }
    else {   // usb audio Enable = Low
      if (flag_input_mode==MODE_USB) {
        SELECT_USB;
        if (flag_headphone_output) flag_headphone_output_before=0;
        else flag_headphone_output_before=1;  // refresh
        send_string("[MCU] DAC Mute enable.\r\n");
        es9038_dac_mute_on();
      }
      //*/  
    }
    
  }
  
  
  // detect dsd on
  if(DSD_ON_READ & DSD_ON_PIN) flag_dsd_on=1; else flag_dsd_on=0;
  if(flag_dsd_on!=flag_dsd_on_before) {         // edge detect
    flag_dsd_on_before=flag_dsd_on;
    if(flag_dsd_on) {
      send_string("[SA9127] DSD mode detected.\r\n");
      if (flag_input_mode==MODE_USB) {
        send_string("[MCU] DSD Selected.\r\n");
        es9038_dac_mute_off();
      }
    }
    else {
      send_string("[SA9127] PCM mode detected.\r\n");
      if (flag_input_mode==MODE_USB) {
        send_string("[MCU] PCM Selected.\r\n");
        es9038_dac_mute_off();
      }
    }
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
  if(USB_DET_READ & USB_DET_PIN) flag_usb_detect=1; else flag_usb_detect=0;
  if(flag_usb_detect & !flag_usb_detect_before) {         // edge detect
    flag_usb_detect_before=1;
    send_string("[SA9127] USB is in normal mode.\r\n");
  }
  if(!flag_usb_detect & flag_usb_detect_before) {         // edge detect
    flag_usb_detect_before=0;
    send_string("[SA9127] USB is in suspended.\r\n");
  }
  
  //detect AK4118A INT0
  if(AK_INT0_READ & AK_INT0_PIN) flag_ak4118a_int0=1; else flag_ak4118a_int0=0;
  
  //detect AK4118A INT1
  if(AK_INT1_READ & AK_INT1_PIN) flag_ak4118a_int1=1; else flag_ak4118a_int1=0;
  
  //Detect AMLI
  if(AMLI_READ & AMLI_PIN) flag_amli=1; else flag_amli=0;
  
}


//////////////////////////////////////
// Calculate Sampling Rate frequency
// Fsr = DPLL x MCLK / (2^32)
//////////////////////////////////////
unsigned int read_sampling_rate(unsigned char mode, unsigned char dsd_on, unsigned char amli)
{
  unsigned long dpll_num;
  unsigned int sampling_rate;
  
  //if ((flag_es9038_left_locked==0)&&(flag_es9038_right_locked==0)) return 0;
  dpll_num = es9038_read_dpll_number(ES9038_ADDR0);
  
  switch (mode) {
    case MODE_COAX1:
    case MODE_COAX2:
    case MODE_AES1:
    case MODE_OPT1:
    case MODE_OPT2:
    case MODE_OPT3:
    case MODE_OPT4:
      dpll_num >>= 12;
      sampling_rate = (unsigned int) (dpll_num / 10.479);
    break;
    case MODE_USB:
      if (flag_dsd_on) {     // DSD
        dpll_num >>= 12;
        sampling_rate = (unsigned int) (dpll_num / 5.709);
      }
      else {            // PCM
        dpll_num >>= 12;
        sampling_rate = (unsigned int) (dpll_num / 10.479);
      }
    break;
  }
  if (sampling_rate < 10) sampling_rate=0;
  return sampling_rate;
}

void es9038_usb_start_playback(void)
{
  DAC_MUTE_ON;
  send_string("[MCU] DAC Mute Enable.\r\n");
  es9038_soft_reset(ES9038_ADDR0);
  es9038_soft_reset(ES9038_ADDR1);
  es9038_set_soft_start_time(ES9038_ADDR0,0);
  es9038_set_soft_start_time(ES9038_ADDR1,0);
  I2S_SEL_ON;
  DAC_MUTE_OFF;
  send_string("[MCU] DAC Mute disable.\r\n");
}
void es9038_usb_stop_playback(void)
{
  DAC_MUTE_ON;
  send_string("[MCU] DAC Mute Enable.\r\n");
  I2S_SEL_ON;
  es9038_soft_reset(ES9038_ADDR0);
  es9038_soft_reset(ES9038_ADDR1);
  DAC_MUTE_OFF;
  send_string("[MCU] DAC Mute disable.\r\n");
}
