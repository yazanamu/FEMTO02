//check : SUT0, CKSEL0

//#include <iom16.h>
#include <iotiny13.h>
#include <inavr.h>

unsigned char carry=0;
unsigned char key_data=0;
unsigned int data_code;
unsigned long cp_code;
//unsigned char cp_code[32]={1,1,0,1,0,1,1,1,0,1,1,1,1,0,1,0};
//unsigned char cpd=0;
//unsigned int Data_code1[16];


void DelayTime(unsigned int time_end)
{
   while(time_end--);
}

void ck_key(){
  key_data=PINB_Bit4;
  key_data<<=1;
  key_data+=PINB_Bit3;
  key_data<<=1;
  key_data+=PINB_Bit0;
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
  
  for(i=0; i<tmr; i++){
    DelayTime(4);
  }
  PORTB_Bit2=0;
}
//unsigned char test;
void IR_data_trans(){
  unsigned char i;

  //0=vol-up, 1=ch-dn, 2=ch-up, 3=filter, 4=phase, 5=vol-dn, 6=mute
  //data_code=0x10, data_code+=key_data, data_code<<=3;
  /*
  if(key_data==1)       cpd=0xb0; //mute
  else if(key_data==2)  cpd=0x98; //filter
  else if(key_data==3)  cpd=0xa0; //phase
  else if(key_data==4)  cpd=0x88; //ch_down
  else if(key_data==5)  cpd=0x90; //ch_up
  else if(key_data==6)  cpd=0xa8; //vol_down
  else if(key_data==7)  cpd=0x80; //vol_up
  */
  ck_key();
  i=0x10;
  i+=key_data;
  i<<=3;
  data_code+=i;
  data_code<<=8;
  i^=0xff; // invert data code
  data_code+=i;
  
//  cp_code=0xd77a0000;
  //cp_code+=data_code;
  cp_code=0xd77a8877;
  
  //lead code
  IR_0(690); 
  IR_1(450);
  IR_0(43);
  /*
  //0   data code
  for(i=0; i<32; i++){
    if(cp_code&0x80000000)  { IR_1(172); IR_0(46);} // 1
    else                    { IR_1(46); IR_0(46);} // 0
    cp_code<<=1;
  }*/
  
  //1   data code
  for(i=0; i<32; i++){
    if( (cp_code>>(31-i)) & 1) { IR_1(171); IR_0(43);} // 1
    else                          { IR_1(44); IR_0(43);} // 0
    //cp_code<<=1;
  }

/*
test=0xff;
  for(i=0; i<8; i++){    
    if(test&1)  { IR_1(172); IR_0(46);} // 1
    else                    { IR_1(46); IR_0(46);} // 0
    test>>=1;
  }
*/
/*
  for(i=0; i<16; i++){
    Data_code[i] = ( (0xd77a>>i) & 0x01 );
  }

  
  for(i=0; i<16; i++){
    Data_code[i] = ( (temp>>i) & 0x01 );
    //Data_code1[i+8] = !Data_code1[i];
  }

  for(i=0; i<16; i++){
    if(Data_code[i])  IR_1(172);
    else              IR_0(46);
    
  }
    
  for(i=0; i<16; i++){
    if(Data_code1[i]){
      PORTB_Bit2=0; 
      for(i=0; i<172; i++)   DelayTime(4);
    }
    else {
      for(i=0; i<46; i++){
        carry^=1;
        PORTB_Bit2=carry;
        DelayTime(4);
      }
    }
  }
 */
}

#pragma vector = INT0_vect
__interrupt void INT0_Handler(void)
{  
    //__disable_interrupt();
    //IR_lead_trans();
    IR_data_trans();  

  PORTB_Bit2=0;  

  ck_key();
  
  if(key_data<6)  {
    while(1){
      if( (PINB_Bit1!=0)  ) break;
    }
  }
  
  DelayTime(0xffff);    //0.1sec
  DelayTime(0xffff);    //0.1sec
  DelayTime(0xffff);    //0.1sec
  //__enable_interrupt();

}

void main(void){ 
  OSCCAL=0;
  DDRB=0x04;
  PORTB=0x3f;
  PORTB_Bit2=0;
  MCUCR=(1<<ISC01) || (0<<ISC00); 
  GIMSK = (1<<INT0);
  GIFR = (1<<INTF0);
  __enable_interrupt();
  //MCUCR=0x1a;
  MCUCR=(1<<SM1) || (0<<SM0);
  MCUCR=(1<<SE);
  
  
}


