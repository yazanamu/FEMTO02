#include <iom128.h>
#include "define.h"
#include "dot_matrix_func.h"

extern unsigned char dot_light_reg;
extern unsigned char dot_string[16];
extern unsigned char flag_mute;
extern unsigned char ess_volume[4];


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
  
      DOT_DATA=dot_string[i*8+j];
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

#define DIV 20
void dot_vol_hextodeci(unsigned char number)	/* display hexadecimal number with 0x */
{
  unsigned char character, div, i;

  if(flag_mute){
    ess_volume[0]='M';
    ess_volume[1]='U';
    ess_volume[2]='T';
    ess_volume[3]='E';
    //dot_matrix_digit2('M',4);
    //dot_matrix_digit2('U',5);
    //dot_matrix_digit2('T',6);
    //dot_matrix_digit2('E',7);
  }
  else if(!number){  //display 100
    ess_volume[0]=' ';
    ess_volume[1]='1';
    ess_volume[2]='0';
    ess_volume[3]='0';
    //dot_matrix_digit2(' ',4);
    //dot_matrix_digit2('1',5);
    //dot_matrix_digit2('0',6);
    //dot_matrix_digit2('0',7);
  }
  else if(number==255){  //display 00.0
    ess_volume[0]='0';
    ess_volume[1]='0';
    ess_volume[2]='.';
    ess_volume[3]='0';
    //dot_matrix_digit2('0',4);
    //dot_matrix_digit2('0',5);
    //dot_matrix_digit2('.',6);
    //dot_matrix_digit2('0',7);
  }
  else{           //display xx.x
    //div = 20;
    number=200-number;
    
    for(i=0; i<2; i++){				// display number
      character = number / DIV;
      ess_volume[i]=character + '0',(4+i);
      //dot_matrix_digit2(character + '0',(4+i));
      number %= DIV;
      div /= 10;
    }
    if(number){
      ess_volume[2]='.';
      ess_volume[3]='5';
      //dot_matrix_digit2('.',6);
      //dot_matrix_digit2('5',7);
    }
    else{
      ess_volume[2]='.';
      ess_volume[3]='0';
      //dot_matrix_digit2('.',6);
      //dot_matrix_digit2('0',7);
    }
  }
  for(i=0; i<4; i++) dot_string[i+12]=ess_volume[i];
  dot_string_digit();
  
}

void dot_matrix_set(void){
  DOT_CLR_FLAG = 1;		//remove CLR(dot-matrix)
  dot_light_reg=1;			//DOT_CLR_FLAG = 1 -> dot_light_reg=1

  //bright = (0 = 100%), (1 = 80%), (2 = 53%), (3 = 40%), (4 = 27%), (5 = 20%), (6 = 13%), (7 = 0%)
  DOT_ADDR4=1;
  dot_matrix_bright(4);		//Bright 27%
}


