#include <iom128.h>
#include "define.h"
#include "main_func.h"
#include "es9038.h"
#include "dot_matrix_func.h"


extern unsigned char key_func;
extern unsigned char flag_mute;
extern unsigned char flag_headphone_output;
extern unsigned char es9018_reg10;
extern unsigned char vol_dB;
extern unsigned char vol_dB_HP;
extern unsigned char phase_data;
extern unsigned char flag_mute;
extern unsigned char ch_led_data;
extern unsigned char i2s_flag;
extern unsigned char interface_ch;
extern unsigned char rom_save_flag2;
extern unsigned char filter_flag;
extern void DelayTime_ms(unsigned int time_end);
extern unsigned int rom_tmr;
extern unsigned char key_condition;
extern char *dot_strings;
extern char *filter_name[7];
extern unsigned int key_func_tmr;
extern unsigned char flag_refresh_display;
extern unsigned char flag_input_mode;

extern void es9038_system_mute(unsigned char devaddr, unsigned char onoff);

extern void es9038_write_register(unsigned char devaddr, unsigned char regaddr, char data);

//////////////////////////////////////////////////////////////////////
U8 test_osc=1;
//ES9018
void ess_mute(void){
  if(!key_func){
    flag_mute=!flag_mute;	//0 = mute, 	1 = unmute
    es9038_system_mute(ES9038_ADDR0,flag_mute);
    es9038_system_mute(ES9038_ADDR1,flag_mute);
    dot_vol_hextodeci(vol_dB_HP);
    dot_vol_hextodeci(vol_dB);
  }
 /*
 test_osc=!test_osc;
 osc80_enable=test_osc;
 
 if(test_osc) I2C_Write(0x20, 0x04, interface_ch);
 else I2C_Write(0x20, 0x04, 0xb8);
 */
}

void volume_set(unsigned char volume){		//I2C write, dot-matrix
  //ES9018, i2c write
  U8 i;
  for(i=0; i<8; i++){
    es9038_write_register(ES9038_ADDR0,ES9038_REG_VOLUME1_CONTROL+i,volume);
    es9038_write_register(ES9038_ADDR1,ES9038_REG_VOLUME1_CONTROL+i,volume);
  }
  //rom_write_multi();
 
}

void init_vol_dn(U8 data){
  //gain
  //I2C_Write(0x90, 23, 0x7f-ess_lch_master_trim);
  //I2C_Write(0x92, 23, 0x7f-ess_rch_master_trim);
  //
  
  
  //I2C_Write(0x90, 20, 0xfe);
  //I2C_Write(0x92, 20, 0xfe);
  /*U8 i;
  if(data<199) data++;
  else           data=0xff;		//-127dB, display num = 00.0
  for(i=0; i<8; i++){
    I2C_Write(0x90,i,data);	//Lch volume of DAC0
    I2C_Write(0x92,i,data);	//Rch volume of DAC0
  }
 */
}

void audio_level_sp_down(void){
  //display num = 100 - (reg/2)
  //audio_level++;	//reg(#0~#7)
  U8 temp;
  if(!key_func){
    temp=vol_dB;
  
    if(vol_dB<197) vol_dB+=3;
    else               vol_dB=0xff;		//-127dB, display num = 00.0
  
    if(temp!=vol_dB) {
      if (flag_headphone_output) volume_set(vol_dB_HP);   // set es9038
      else                       volume_set(vol_dB);
      dot_vol_hextodeci(vol_dB);
    }
  }
}

void audio_level_sp_up(void){
  //display num = 100 - (reg/2)
  //audio_level--;	//reg(#0~#7)
  U8 temp;
  if(!key_func){
    temp=vol_dB;
  
    if(vol_dB==0xff) vol_dB=199;
    else if(vol_dB>2) vol_dB-=3;
    else vol_dB=0;
  
    if(temp!=vol_dB) {
      if (flag_headphone_output) volume_set(vol_dB_HP);   // set es9038
      else                       volume_set(vol_dB);
      dot_vol_hextodeci(vol_dB);
    }
  }
}

void channel_change(void){
  /*
  U8 temp;
  
  if(flag_mute) {			//0 = mute, 	1 = unmute
      es9038_system_mute(ES9038_ADDR0,1);       //mute
      es9038_system_mute(ES9038_ADDR1,1);       //mute
    }
  
  if(ch_led_data==7) temp=1;    //USB mode     : I2S_SEL=1;      // Modified by Jang WS 2017.9.16
  else temp=0;                       //CS8416 mode : I2S_SEL=0;  // Modified by Jang WS 2017.9.16
    
  if(temp!=i2s_flag){
    i2s_flag=temp;
    I2S_SEL=i2s_flag;
  }
  
  //ch_led_data : 0=coax1,  1=coax2,    2=aes1,   3=aes2,   4=bnc,  5=opt1,   6=opt2,   7=usb
  interface_ch = interface_ch&0xc7;		// D5~D3 = 0
  interface_ch+=(ch_led_data<<3);
  //I2C_Write(0x20, 0x04, interface_ch);
  
  if(!flag_mute) {			//0 = mute, 	1 = unmute

      es9038_system_mute(ES9038_ADDR0,0);       //unmute
      es9038_system_mute(ES9038_ADDR1,0);       //unmute
    }
  
  //rom_write_multi();

  //dot_vol_hextodeci(vol_dB);
  */
}

void audio_level_up(void) {
}

void audio_level_down(void) {
}

void channel_up(void){
  //channel_vol_save();
  if(!key_func){
    flag_input_mode++;
    flag_input_mode&=0x07;
    //if(ch_led_data<7) ch_led_data++;
    //else ch_led_data=0;
    channel_change();
  }
}

void channel_down(void){
  //channel_vol_save();
  if(!key_func){
    flag_input_mode--;
    flag_input_mode&=0x07;
    //if(ch_led_data) ch_led_data--;
    //else ch_led_data=7;
    channel_change();
  }
}

void ess_filter(void){
//  S32 coeff;
//  U8 i,c,num; 
  U8 num;
  
  rom_save_flag2=0;
  EIFR =  (0<<INTF3);   //INTF3 disable
  if(filter_flag==3) filter_flag=0;
  num=filter_flag; 
  
  //rom_write_multi();
  DelayTime_ms(5);  //5msec  
  
/* 12-07-24 change 
  if(num==0) {   
    I2C_Write(0x90, 37, 0x00);
    I2C_Write(0x92, 37, 0x00);    
    I2C_Write(0x90, 14, 0x0b);
    I2C_Write(0x92, 14, 0x0b);
  }
  else if (num==1 || num==2) {
  
    I2C_Write(0x90, 37, 0x10); // enable stage 1 programming
    I2C_Write(0x92, 37, 0x10); // enable stage 1 programming
    for ( i = 0; i < 64; i++) // for all 64 stage 1 coefficients
    {
    
      if(num==1) coeff = f1_coeff_st1[i]; // get the stage1 coefficient
      else if(num==2) coeff = f2_coeff_st1[i]; // get the stage1 coefficient
      for (c = 0; c < 8; c++) // for all 8 channels
      {
        I2C_Write(0x90, 41, ((coeff >> 24) & 0xff));
        I2C_Write(0x92, 41, ((coeff >> 24) & 0xff));
        I2C_Write(0x90, 40, ((coeff >> 16) & 0xff));
        I2C_Write(0x92, 40, ((coeff >> 16) & 0xff));
        I2C_Write(0x90, 39, ((coeff >> 8) & 0xff));
        I2C_Write(0x92, 39, ((coeff >> 8) & 0xff));
        I2C_Write(0x90, 38, ((coeff) & 0xff));
        I2C_Write(0x92, 38, ((coeff) & 0xff));
      }
    }
    I2C_Write(0x90, 38, 0x00); // clear the stage 1 coefficient pipe
    I2C_Write(0x92, 38, 0x00); // clear the stage 1 coefficient pipe
    I2C_Write(0x90, 37, 0x00); // disable stage 1 programming
    I2C_Write(0x92, 37, 0x00); // disable stage 1 programming
    
    I2C_Write(0x90, 37, 0x01); // enable stage 2 programming
    I2C_Write(0x92, 37, 0x01); // enable stage 2 programming
    for (i = 0; i < 16; i++) // for all 16 stage 2 coefficients
    {
      if(num==1) coeff = f1_coeff_st2[i]; // get the stage2 coefficient
      else if(num==2) coeff = f2_coeff_st2[i]; // get the stage1 coefficient
      for (c = 0; c < 8; c++) // for all 8 channels
      {
        I2C_Write(0x90, 45, ((coeff >> 24) & 0xff));
        I2C_Write(0x92, 45, ((coeff >> 24) & 0xff));
        I2C_Write(0x90, 44, ((coeff >> 16) & 0xff));
        I2C_Write(0x92, 44, ((coeff >> 16) & 0xff));
        I2C_Write(0x90, 43, ((coeff >> 8) & 0xff));
        I2C_Write(0x92, 43, ((coeff >> 8) & 0xff));
        I2C_Write(0x90, 42, ((coeff) & 0xff));
        I2C_Write(0x92, 42, ((coeff) & 0xff));
      }
    }
    I2C_Write(0x90, 42, 0x00); // clear the stage 2 coefficient pipe
    I2C_Write(0x92, 42, 0x00); // clear the stage 2 coefficient pipe
    I2C_Write(0x90, 37, 0x00); // disable stage 2 programming
    I2C_Write(0x92, 37, 0x00); // disable stage 2 programming
    
    I2C_Write(0x90, 37, 0x22);
    I2C_Write(0x92, 37, 0x22);
    if(num==1)  { I2C_Write(0x90, 14, 0x0a);    I2C_Write(0x92, 14, 0x0c);}
    else if(num==2)  { I2C_Write(0x90, 14, 0x0a);    I2C_Write(0x92, 14, 0x0e);}

  }  */
  
  if(num==0){
    //I2C_Write(0x90, 14, 0x0b);
    //I2C_Write(0x92, 14, 0x0b);
  }
  else if(num==1){
    //I2C_Write(0x90, 14, 0x0d);
    //I2C_Write(0x92, 14, 0x0d);
  }
  else if(num==2){
    //I2C_Write(0x90, 14, 0x0f);
    //I2C_Write(0x92, 14, 0x0f);
  }
  
  if(num) DelayTime_ms(200);  //0.2sec
  
  rom_save_flag2=1;
  rom_tmr=0;
  
  EIFR =  (1<<INTF3);   //INTF3 enable
  
}

void femto_function(void){
  //U8 i;
  
  if(key_func==1 || key_func==2)  key_condition=1;  //inverse or filter
  
  else if(key_func==3){ //change inverse
    phase_data=!phase_data;
    //phase_ess();
  }
  else if(key_func==4){ //change filter
    filter_flag++;
    
    if(flag_mute) {			//0 = mute, 	1 = unmute
      es9038_system_mute(ES9038_ADDR0,1);       //mute
      es9038_system_mute(ES9038_ADDR1,1);       //mute
    }
    
    ess_filter();
    if (flag_headphone_output) volume_set(vol_dB_HP);   // set es9038
    else                       volume_set(vol_dB);
    DelayTime_ms(5);  //5msec
    
    
    if(!flag_mute) {			//0 = mute, 	1 = unmute
      es9038_system_mute(ES9038_ADDR0,0);       //unmute
      es9038_system_mute(ES9038_ADDR1,0);       //unmute
    }
    //rom_write_multi();
    //DelayTime_ms(10);  //10msec
  }
  
  //for(i=0; i<16; i++) {
  //  if(i<8) dot_strings[i]=filter_name[filter_flag][i];
  //  else {
  //    if(!phase_data) dot_strings[i]=phase_name[i-8];  //phase on
  //    else dot_strings[i]=normal_name[i-8];   //phase off
  //  }
  //}
  
  key_func_tmr=0;
  flag_refresh_display=1; // refresh display
  //dot_string_digit();
}