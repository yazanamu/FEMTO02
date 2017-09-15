//check : SUT0, CKSEL0

//#include <iom16.h>
#include <iotiny13.h>
#include <inavr.h>

unsigned char carry=0;
unsigned char key_data=0;
unsigned long data_code;
unsigned long cp_code=0xd77a0000;
//unsigned char cp_code[32]={1,1,0,1,0,1,1,1,0,1,1,1,1,0,1,0};
//unsigned char cpd=0;
//unsigned int Data_code1[16];

unsigned char flag=1;


void DelayTime(unsigned int time_end)
{
   while(time_end--);
}

void IR_0(unsigned int tmr)
{
  unsigned int i;
  
  carry=0;
  for(i=0; i<tmr; i++){
      carry^=1;
      PORTB_Bit4=carry;
    if(carry) DelayTime(4);
    else DelayTime(3);
  }
  PORTB_Bit4=0;
  //DelayTime(1);
}
void IR_1(unsigned int tmr){
  unsigned int i;
  
  for(i=0; i<tmr; i++){
    DelayTime(4);
  }
}

void IR_lead_trans(){
  IR_0(670); 
  IR_1(440);
  IR_0(43);
  //IR_0(644); 
  //IR_1(322);
  //IR_0(40);
}

void IR_data_trans(){
  unsigned char i,temp;

  //lead code
  IR_lead_trans();
  
  //1   data code
  for(i=0; i<32; i++){
    temp=(cp_code>>(31-i)) & 1;
    if( temp) { IR_1(170); IR_0(43);} // 1
    else      { IR_1(60); IR_0(43);} // 0
    //if( temp) { IR_1(120); IR_0(40);} // 1
    //else      { IR_1(40); IR_0(40);} // 0
    //cp_code<<=1;
  }
}

void in_key(){
  //0=vol-up, 1=ch-dn, 2=ch-up, 3=filter, 4=phase, 5=vol-dn, 6=mute
  //data_code=0x10, data_code+=key_data, data_code<<=3;
  /*
  key_data=PINB_Bit4;
  key_data<<=1;
  key_data+=PINB_Bit3;
  key_data<<=1;
  key_data+=PINB_Bit0;
  */
  key_data=PINB;
  key_data&=0x07;
}

unsigned char data_temp;

void in_key_save(){
  //unsigned char i;
  in_key();
  data_code=0;
  
  
  data_temp=0x10;
  data_temp+=key_data;
  data_temp<<=3;
  data_code+=data_temp;
  data_code<<=8;
  data_temp^=0xff; // invert data code
  data_code+=data_temp;
  cp_code+=data_code;
  
  if(key_data==0 || key_data==5)  flag=2;
  else flag=1;
}

unsigned char old_key=0;
void main(void){ 
  
  OSCCAL=0;
  //DDRB=0x04;
  //PORTB=0x3b;
  DDRB=0x10;
  PORTB=0x1f;
  //PORTB_Bit2=0; 
  //PORTB_Bit4=0; 
  
  in_key_save();
  old_key=key_data;
  while(flag){   
    if(flag<2) flag--;
    
    IR_data_trans();  
    //PORTB_Bit2=0;   
    in_key();
    if(old_key!=key_data) break;
    IR_1(4500);
  }
  
  
}


