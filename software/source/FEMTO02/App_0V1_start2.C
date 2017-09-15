//2013.12.16

// OCDEN, JTAGEN, CKOPT, SPIEN

#include <iom128.h>
#include <inavr.h>
#include "InitBoard.c"
//#include "uart.h" // added by jang 2017.9.13

///////////////////////////////////////////////////////////////////////////////
void sample_rate_cal(){
  U8 i,temp;
  
  //if ( (old_ess_lock_ck!=ess_lock_ck) || (old_ess_automute!=ess_automute) ) {
    
    //if (old_ess_lock_ck!=ess_lock_ck) old_ess_lock_ck=ess_lock_ck;
    //if (old_ess_automute!=ess_automute) old_ess_automute=ess_automute;
  
    if( ( (ch_led_data!=7) && ess_lock_ck) || (ess_lock_ck && !ess_automute) ) {
       sample_rate=I2C_Read(0x90, 31);
       sample_rate<<=8;
       sample_rate+=I2C_Read(0x90, 30);
       sample_rate2=I2C_Read(0x92, 31);
       sample_rate2<<=8;
       sample_rate2+=I2C_Read(0x92, 30);
    
        //0=44.1kHz,    1=48kHz,    2=88.2kHz,    3=96kHz,    4=176.4kHz,   5=196kHz
  
        //192kHz
        if( (sample_rate>min192 && sample_rate<max192) || (sample_rate2>min192 && sample_rate2<max192) )	sr_led_data=5;
    
        //176.4kHz
        else if( (sample_rate>min176 && sample_rate<max176) || (sample_rate2>min176 && sample_rate2<max176) )	sr_led_data=4;
        
        //96kHz
        else if( (sample_rate>min96 && sample_rate<max96) || (sample_rate2>min96 && sample_rate2<max96) )	sr_led_data=3;
    
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
   
    if(!old_exMute) temp=vol_dB;
    else temp=224;    //-114dB
      
    for(i=0; i<8; i++){
      I2C_Write(0x90,i,temp);	//Lch volume of DAC0
      I2C_Write(0x92,i,temp);	//Rch volume of DAC0
    }
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
__interrupt void INT3_Handler(void)
{
  U8 data=0;
  
  key_int_flag=0;
  data=KEY_DATA2;
  data<<=1;
  data+=KEY_DATA1;
  data<<=1;
  data+=KEY_DATA0;
  
  // data =1 : ch-dn,   2: vol-up,  3 : mute,   4 : ch-up,  5 : vol-dn,   6 : inverse,  7 : filter
  if(tmr_osc_ck){
    //channel down
    if(data==1){
      channel_down();
    }
  
    //volume up
    else if(data==2) {
      audio_level_up();                            //Master Volume Up
    }
  
    //mute
    else if(data==3){
      ess_mute();	//mute
    }
  
    //channel up
    else if(data==4){
      channel_up();
    }
  
    //volume down
    else if(data==5){
      audio_level_down();                        //Master Volume Down
    }
  
    //inverse
    else if(data==6){
      if(!key_condition) key_func=1;
      else key_func=3;
      femto_function();
      //phase_write();	//phase
      //phase_ess();
    }
  
    //filter
    else if(data==7){
      if(!key_condition) key_func=2;
      else key_func=4;
      femto_function();
      //filter_led<<=1;
      //if(filter_led>0x04)  filter_led=1;	//filter1
      //ess_filter(filter_led);
    }
    key_int_flag=1;
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
      I2C_Write(0x90, 0x0a, es9018_reg10);
      I2C_Write(0x92, 0x0a, es9018_reg10);
      init_vol_dn(vol_dB);
      DelayTime_ms(50);  //50msec
      init_vol(vol_dB);
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
  _system_init();
  //_system_init_1();
    
  while(1){
    if(PINB_Bit6==0) PORTB_Bit7=0;       //Analog Power Disable
    //if(tmr_osc_ck) _system_init_1();
    /*
    if(tmr_osc_ck){
      tmr_osc_ck2=1;
      tmr_osc_ck=0;
      //one time
      if(KeyReady){
        _system_init_se();
        es9018_reg10=0xce;			
        I2C_Write(0x90, 0x0a, es9018_reg10);
        I2C_Write(0x92, 0x0a, es9018_reg10);
        //led_start_flag=1;
        KeyReady=0;
      }
      
    }//end if
    */
  }//end while
}


