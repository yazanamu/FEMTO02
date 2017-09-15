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
      PORTB_Bit2=carry;
    DelayTime(4);
  }
  PORTB_Bit2=0;
}
void IR_1(unsigned int tmr){
  unsigned int i;
  
  //PORTB_Bit2=0;
  for(i=0; i<tmr; i++){
    DelayTime(4);
  }
  //PORTB_Bit2=0;
}

void IR_lead_trans(){
  IR_0(670); 
  IR_1(440);
  IR_0(43);
}

void IR_data_trans(){
  unsigned char i,temp;

  //lead code
  IR_lead_trans();
  
  //1   data code
  for(i=0; i<32; i++){
    temp=(cp_code>>(31-i)) & 1;
    //temp=cp_code[i];
    if( temp) { IR_1(170); IR_0(43);} // 1
    else      { IR_1(43); IR_0(43);} // 0
    //cp_code<<=1;
  }
}


unsigned char flag=0;
void main(void){ 
  unsigned char i;
  OSCCAL=0;
  DDRB=0x04;
  PORTB=0x3b;
  PORTB_Bit2=0;

//0=vol-up, 1=ch-dn, 2=ch-up, 3=filter, 4=phase, 5=vol-dn, 6=mute
//data_code=0x10, data_code+=key_data, data_code<<=3;
  key_data=PINB_Bit4;
  key_data<<=1;
  key_data+=PINB_Bit3;
  key_data<<=1;
  key_data+=PINB_Bit0;
  
  i=0x10;
  i+=key_data;
  i<<=3;
  data_code+=i;
  data_code<<=8;
  i^=0xff; // invert data code
  data_code+=i;
  cp_code+=data_code;
  
  if(key_data==0 || key_data==5)  flag=2;
  else flag=1;
  
  while(flag){
    if(flag<2) flag--;
    
    IR_data_trans();  
    PORTB_Bit2=0;   
    IR_1(4500); 
  }
  
  
}


