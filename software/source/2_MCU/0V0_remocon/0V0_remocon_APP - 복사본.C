
//2009.12.21

//#include <iom16.h>
#include <iotiny13.h>
#include <inavr.h>
//#include "remocon_code.c"



//#define IR_out	PORTB_Bit2

//#define KEY_A2       PINB_Bit4
//#define KEY_A1       PINB_Bit3
//#define KEY_A0       PINB_Bit0

//unsigned int custom_code=0xd77a;
/*
U8 mute=0xb0;
U8 vol_up=0x80;
U8 vol_down=0xa8;
U8 ch_up=0x90;
U8 ch_down=0x88;
U8 phase=0xa0;
U8 filter=0x98;
*/
unsigned char carry=0;
unsigned char key_data=0;
unsigned char Data_code[16]={0,1,0,1,1,1,1,0,1,1,1,0,1,0,1,1};

void DelayTime(unsigned int time_end)
{
   while(time_end--);
}

void IR_lead_trans(){
  unsigned int i;
  
  for(i=0; i<690; i++){
    if(carry==0) carry=1;
    else carry=0;
    PORTB_Bit2=carry;
    DelayTime(4);
  }
  PORTB_Bit2=0; 
  for(i=0; i<345; i++)   DelayTime(4);
  
  for(i=0; i<46; i++){
    if(carry==0) carry=1;
    else carry=0;
    PORTB_Bit2=carry;
    DelayTime(4);
  }
}

unsigned int cpd=0;
//unsigned int Data_code[16];
unsigned int Data_code1[16];

void IR_data_trans(){/*
  unsigned char i;
  unsigned int temp;
  
  if(key_data==1)         cpd=0xb04f; //mute
  else if(key_data==2)  cpd=0x98; //filter
  else if(key_data==3)  cpd=0xa0; //phase
  else if(key_data==4)  cpd=0x88; //ch_down
  else if(key_data==5)  cpd=0x90; //ch_up
  else if(key_data==6)  cpd=0xa8; //vol_down
  else if(key_data==7)  cpd=0x80; //vol_up
  

  for(i=0; i<16; i++){
    Data_code[i] = ( (0xd77a>>i) & 0x01 );
  }

  
  for(i=0; i<16; i++){
    Data_code[i] = ( (temp>>i) & 0x01 );
    //Data_code1[i+8] = !Data_code1[i];
  }

  for(i=0; i<16; i++){
    if(Data_code[i]){
      PORTB_Bit2=0; 
      for(i=0; i<172; i++)   DelayTime(4);
    }
    else {
      for(i=0; i<46; i++){
        if(carry==0) carry=1;
        else carry=0;
        PORTB_Bit2=carry;
        DelayTime(4);
      }
    }
  }
    
  for(i=0; i<16; i++){
    if(Data_code1[i]){
      PORTB_Bit2=0; 
      for(i=0; i<172; i++)   DelayTime(4);
    }
    else {
      for(i=0; i<46; i++){
        if(carry==0) carry=1;
        else carry=0;
        PORTB_Bit2=carry;
        DelayTime(4);
      }
    }
  }
  */
}

/*
U8 test_yu=0;6

void test_wave(U8 data){
  data=data*2;
   while(data){
    if(test_yu==0) test_yu=1;
    else test_yu=0;
    IR_out=test_yu;
    DelayTime(4);
    data--;
  }
}
void test_hold(U8 data){
  U8 i;
  IR_out=0;
  for(i=0; i<(data*2); i++)   DelayTime(4);
}
*/



#pragma vector = INT0_vect
__interrupt void INT0_Handler(void)
{  
    
    key_data=PINB_Bit4;
    key_data<<=1;
    key_data+=PINB_Bit3;
    key_data<<=1;
    key_data+=PINB_Bit0;
    /*
    key_data=PINB;
    key_data=key_data & 0x19;
    key_data = (key_data>>2) + (key_data &0x01);
    */
    IR_lead_trans();
    IR_data_trans();  
        
    
 /* 
    //
  for(i=0; i<50; i++){
   test_wave(23);
   test_hold(100);
  }
  //
  
  */
  PORTB_Bit2=0;  

  key_data=PINB_Bit4;
  key_data<<=1;
  key_data+=PINB_Bit3;
  key_data<<=1;
  key_data+=PINB_Bit0;
  
  if((key_data==1) || (key_data==2) || (key_data==3) || (key_data==4) || (key_data==5))  {
    while(1){
      if( (PINB_Bit1!=0)  ) break;
    }
  }
  
  DelayTime(0xffff);

}


///////////////////////////////////////////////////////////////////////////////
//	Main ÇÔ¼ö
///////////////////////////////////////////////////////////////////////////////
void main(void){ 
	//unsigned char temp;

  //OSCCAL=0;
  
  
  //DDRB=0x04;   //PB0=MOSI, PB1=MISO, PB2=SCK, PB3=I2C(SCL), PB4=I2C(SDA), PB5=/RESET
  //PORTB=0x3f;
  //PORTB_Bit2=0;

  
  //MCUCR = (1<<ISC01) | (0<<ISC00);    //The falling edge
  //GICR = (1<<INT0);   //mega16
  //GIMSK = (1<<INT0);
//  GIFR = (1<<INTF0);    //mega16
  //EIFR = (1<<INTF0);
  
  //MCUCR=0x60;
  //__enable_interrupt();
  
  
  //MCUCR=0x32;
  //GIMSK=0x40;
  //GIFR=0x40;

}


