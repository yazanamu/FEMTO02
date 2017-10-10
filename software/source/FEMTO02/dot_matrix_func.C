#include <iom128.h>
#include "define.h"
#include "dot_matrix_func.h"
#include "es9038.h"
#include <string.h>

#define COLUMN_CH 0
#define COLUMN_SR 3
#define COLUMN_VOL 7
#define COLUMN_FLT 12
#define COLUMN_OUT 15

#define CH_NUM_CH 3
#define CH_NUM_SR 4
#define CH_NUM_VOL 4
#define CH_NUM_FLT 2
#define CH_NUM_OUT 1

extern unsigned char dot_light_reg;
extern unsigned char flag_mute;
extern char flag_input_mode;
extern char flag_filter;
extern char flag_headphone_output;
extern unsigned int flag_first_display;
extern unsigned char sr_led_data;

char display_default_strings[]="CH SAM VOLU FI O";
char display_message_strings[]="Message Message ";
char display_first_strings[] = "FEMTO2 SIGNATURE";
char dot_strings[]=            "1234567890123456";
char display_mute_strings[] =  "      MUTE      ";
char *display_inputsource= &display_default_strings[COLUMN_CH];
char *display_samplingrate=&display_default_strings[COLUMN_SR];
char *display_ess_volume = &display_default_strings[COLUMN_VOL];
char *display_filter =     &display_default_strings[COLUMN_FLT];
char *display_output =     &display_default_strings[COLUMN_OUT];

unsigned int message_delay=DEFAULT_MESSAGE_DELAY;

char *ch_name[8] = {
  "C1 ",
  "C2 ",
  "A  ",
  "O1 ",
  "O2 ",
  "O3 ",
  "O4 ",
  "U  "
};
char *sr_name[7] = {
  " 44 ",
  " 48 ",
  " 88 ",
  " 96 ",
  "176 ",
  "192 ",
  "    "
};
char *filter_name[7] = {
  "F1 ",
  "F2 ",
  "F3 ",
  "F4 ",
  "F5 ",
  "F6 ",
  "F7 "
};
char *ouput_name[2] = {
  "L",
  "H"
};

char *volume_name = "VOLU";
char *phase_name=" INVERSE";
char *normal_name="  NORMAL";

void dot_matrix_clear(void){
  DOT_CLR_FLAG = 0;
  dot_light_reg=0;
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
void dot_string_digit(void){
  U8 i,j,xdigit;
  U16 k;
  xdigit=0;
  DOT_CLR_FLAG=1;  
  DOT_ADDR3=1;
  for(i=0; i<2; i++){
    for(j=0; j<8; j++){
      if(xdigit==0){ DOT_ADDR2=0; DOT_ADDR1=0; DOT_ADDR0=0;}
      else if(xdigit==1){ DOT_ADDR2=0; DOT_ADDR1=0; DOT_ADDR0=1;}
      else if(xdigit==2){ DOT_ADDR2=0; DOT_ADDR1=1; DOT_ADDR0=0;}
      else if(xdigit==3){ DOT_ADDR2=0; DOT_ADDR1=1; DOT_ADDR0=1;}
      else if(xdigit==4){ DOT_ADDR2=1; DOT_ADDR1=0; DOT_ADDR0=0;}
      else if(xdigit==5){ DOT_ADDR2=1; DOT_ADDR1=0; DOT_ADDR0=1;}
      else if(xdigit==6){ DOT_ADDR2=1; DOT_ADDR1=1; DOT_ADDR0=0;}
      else if(xdigit==7){ DOT_ADDR2=1; DOT_ADDR1=1; DOT_ADDR0=1;}
      
      if(i) M2_DOT_ENABLE=0;	              //M2 enable
      else M1_DOT_ENABLE=0;	//M1 enable
      
      k=8;while(k--); //about 0.5us
      DOT_WR=0;
      k=8;while(k--); //about 0.5us
  
      DOT_DATA=dot_strings[i*8+j];
      k=16;while(k--); //about 1us
      DOT_WR=1;
      
      if(i) M2_DOT_ENABLE=1;	              //M2 enable
      else M1_DOT_ENABLE=1;	//M1 enable
      
      k=16;while(k--); //about 1us
      xdigit++;
    }
    k=1600;while(k--);  //0.1msec
    xdigit=0;
  }
}


void dot_matrix_digit(unsigned char data, unsigned char xdigit){
  U16 k;
  DOT_CLR_FLAG=1;  
  DOT_ADDR3=1;
  if(xdigit==0){ DOT_ADDR2=0; DOT_ADDR1=0; DOT_ADDR0=0;}
  else if(xdigit==1){ DOT_ADDR2=0; DOT_ADDR1=0; DOT_ADDR0=1;}
  else if(xdigit==2){ DOT_ADDR2=0; DOT_ADDR1=1; DOT_ADDR0=0;}
  else if(xdigit==3){ DOT_ADDR2=0; DOT_ADDR1=1; DOT_ADDR0=1;}
  else if(xdigit==4){ DOT_ADDR2=1; DOT_ADDR1=0; DOT_ADDR0=0;}
  else if(xdigit==5){ DOT_ADDR2=1; DOT_ADDR1=0; DOT_ADDR0=1;}
  else if(xdigit==6){ DOT_ADDR2=1; DOT_ADDR1=1; DOT_ADDR0=0;}
  else if(xdigit==7){ DOT_ADDR2=1; DOT_ADDR1=1; DOT_ADDR0=1;}
  k=8;while(k--); //about 0.5us
  
  M1_DOT_ENABLE=0;	//enable
  k=8;while(k--); //about 0.5us
  DOT_WR=0;
  k=8;while(k--); //about 0.5us
  
  DOT_DATA=data;
  k=16;while(k--); //about 1us
  DOT_WR=1;
  M1_DOT_ENABLE=1;	//disable
  k=16;while(k--); //about 1us
}

void dot_matrix_digit2(unsigned char data, unsigned char xdigit){
  U16 k;
  DOT_CLR_FLAG=1;  
  DOT_ADDR3=1;
  if(xdigit==0){ DOT_ADDR2=0; DOT_ADDR1=0; DOT_ADDR0=0;}
  else if(xdigit==1){ DOT_ADDR2=0; DOT_ADDR1=0; DOT_ADDR0=1;}
  else if(xdigit==2){ DOT_ADDR2=0; DOT_ADDR1=1; DOT_ADDR0=0;}
  else if(xdigit==3){ DOT_ADDR2=0; DOT_ADDR1=1; DOT_ADDR0=1;}
  else if(xdigit==4){ DOT_ADDR2=1; DOT_ADDR1=0; DOT_ADDR0=0;}
  else if(xdigit==5){ DOT_ADDR2=1; DOT_ADDR1=0; DOT_ADDR0=1;}
  else if(xdigit==6){ DOT_ADDR2=1; DOT_ADDR1=1; DOT_ADDR0=0;}
  else if(xdigit==7){ DOT_ADDR2=1; DOT_ADDR1=1; DOT_ADDR0=1;}
  k=8;while(k--); //about 0.5us
  
  M2_DOT_ENABLE=0;	//enable
  k=8;while(k--); //about 0.5us
  DOT_WR=0;
  k=8;while(k--); //about 0.5us
  
  DOT_DATA=data;
  k=16;while(k--); //about 1us
  DOT_WR=1;
  M2_DOT_ENABLE=1;	//disable
  k=16;while(k--); //about 1us
}

////////////////////////////////////////////////////////////////////////////////////////////////
//Bright 4digit or 8digit

void dot_matrix_bright(unsigned char bright){
  U8 i;
  U16 k;
//bright = (0 = 100%), (1 = 80%), (2 = 53%), (3 = 40%), (4 = 27%), (5 = 20%), (6 = 13%), (7 = 0%)
  for(i=0; i<8; i++){
    DOT_CLR_FLAG=1;
    DOT_ADDR3=0;	//function mode
    if(i==0){ DOT_ADDR2=0; DOT_ADDR1=0; DOT_ADDR0=0;}
    else if(i==1){ DOT_ADDR2=0; DOT_ADDR1=0; DOT_ADDR0=1;}
    else if(i==2){ DOT_ADDR2=0; DOT_ADDR1=1; DOT_ADDR0=0;}
    else if(i==3){ DOT_ADDR2=0; DOT_ADDR1=1; DOT_ADDR0=1;}
    else if(i==4){ DOT_ADDR2=1; DOT_ADDR1=0; DOT_ADDR0=0;}
    else if(i==5){ DOT_ADDR2=1; DOT_ADDR1=0; DOT_ADDR0=1;}
    else if(i==6){ DOT_ADDR2=1; DOT_ADDR1=1; DOT_ADDR0=0;}
    else if(i==7){ DOT_ADDR2=1; DOT_ADDR1=1; DOT_ADDR0=1;}
    k=8;while(k--); //about 0.5us
    
    M1_DOT_ENABLE=0;	//enable
    k=8;while(k--); //about 0.5us
    DOT_WR=0;
    k=8;while(k--); //about 0.5us
    
    DOT_DATA=bright;
    k=16;while(k--); //about 1us
    DOT_WR=1;
    M1_DOT_ENABLE=1;	//disable
    k=16;while(k--); //about 1us
  }
  
  for(i=0; i<8; i++){
    DOT_CLR_FLAG=1;
    DOT_ADDR3=0;	//function mode
    if(i==0){ DOT_ADDR2=0; DOT_ADDR1=0; DOT_ADDR0=0;}
    else if(i==1){ DOT_ADDR2=0; DOT_ADDR1=0; DOT_ADDR0=1;}
    else if(i==2){ DOT_ADDR2=0; DOT_ADDR1=1; DOT_ADDR0=0;}
    else if(i==3){ DOT_ADDR2=0; DOT_ADDR1=1; DOT_ADDR0=1;}
    else if(i==4){ DOT_ADDR2=1; DOT_ADDR1=0; DOT_ADDR0=0;}
    else if(i==5){ DOT_ADDR2=1; DOT_ADDR1=0; DOT_ADDR0=1;}
    else if(i==6){ DOT_ADDR2=1; DOT_ADDR1=1; DOT_ADDR0=0;}
    else if(i==7){ DOT_ADDR2=1; DOT_ADDR1=1; DOT_ADDR0=1;}
    k=8;while(k--); //about 0.5us
    
    M2_DOT_ENABLE=0;	//enable
    k=8;while(k--); //about 0.5us
    DOT_WR=0;
    k=8;while(k--); //about 0.5us
    
    DOT_DATA=bright;
    k=16;while(k--); //about 1us
    DOT_WR=1;
    M2_DOT_ENABLE=1;	//disable
    k=16;while(k--); //about 1us
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////

void dot_0x_hexadecimal(unsigned char number)	/* display hexadecimal number with 0x */
{
  unsigned char i, character;

  dot_matrix_digit('0',3);
  dot_matrix_digit('x',2);

  for(i = 2; i > 0; i--){
    character = (number >> 4*(i-1)) & 0x0F;
    if(character < 10) dot_matrix_digit(character + '0',(i-1));
    else               dot_matrix_digit(character - 10 + 'A',(i-1));
  }
}

void dot_vol_hextodeci(unsigned char volume_db)	/* display hexadecimal number with 0x */
{
  unsigned char number;

  if(volume_db==ES9038_MAX_VOLUME){  //display 100
    volume_name=" 100";
  }
  else if(volume_db==ES9038_MIN_VOLUME){  //display  MUTE
    volume_name="MUTE";
  }
  else{           //display xx.x
    //div = 20;
    number=200-volume_db; 
    if(number%2){       // odd num
      volume_name[2]='.';
      volume_name[3]='5';
    }
    else{               // even num
      volume_name[2]='.';
      volume_name[3]='0';
    }
    number/=2; //(1~199) -> (1~99)
    volume_name[0] = '0' + (number/10);
    if (volume_name[0]=='0') display_ess_volume[0] = ' ';
    number%=10;
    volume_name[1] = '0' + number;
  }
  if (memcmp(volume_name, display_ess_volume, 4)) memcpy(display_ess_volume,volume_name,4);
  //for(i=0; i<4; i++) dot_string[i+12]=display_ess_volume[i];
  //dot_string_digit();
  
}

void dot_matrix_set(void){
  DOT_CLR_FLAG = 1;		//remove CLR(dot-matrix)
  dot_light_reg=1;			//DOT_CLR_FLAG = 1 -> dot_light_reg=1

  //bright = (0 = 100%), (1 = 80%), (2 = 53%), (3 = 40%), (4 = 27%), (5 = 20%), (6 = 13%), (7 = 0%)
  DOT_ADDR4=1;
  dot_matrix_bright(4);		//Bright 27%
}

unsigned char need_display_update(char *str1, char *str2)
{
  unsigned char i;
  for(i=0;i<16;i++) if (str1[i]!=str2[i]) return 1;
  return 0;  
}

void display_dot_matrix(void) {
  unsigned char i;
  
  //for(i=0;i<16;i++) {
  //  if(i<6)             display_default_strings[i]=ch_name[flag_input_mode][i];
  //  else if(i<12)       display_default_strings[i]=sr_name[sr_led_data][i-6];
  //  else                display_default_strings[i]=display_ess_volume[i-12];
  //}
  if (flag_first_display)
    memcpy(display_default_strings,display_first_strings,16);
  else if (flag_mute) \
    memcpy(display_default_strings,display_mute_strings,16);
  else {
    memcpy(display_default_strings+COLUMN_CH,ch_name[flag_input_mode],CH_NUM_CH);
    memcpy(display_default_strings+COLUMN_SR,sr_name[sr_led_data],CH_NUM_SR);
    memcpy(display_default_strings+COLUMN_VOL,volume_name,CH_NUM_VOL);
    memcpy(display_default_strings+COLUMN_FLT,filter_name[flag_filter],CH_NUM_FLT);
    memcpy(display_default_strings+COLUMN_OUT,ouput_name[flag_headphone_output],CH_NUM_OUT);
  }
  if (message_delay) {
    for(i=0;i<16;i++) dot_strings[i]=display_message_strings[i];
    message_delay--;
  }
  else for(i=0;i<16;i++) dot_strings[i]=display_default_strings[i];
}