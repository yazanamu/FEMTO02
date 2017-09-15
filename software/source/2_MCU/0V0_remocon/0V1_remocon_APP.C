//check : SUT0, CKSEL0

#include <iotiny13.h>
#include <inavr.h>

unsigned char carry=0;
unsigned char key_data=0;
unsigned long data_code;
unsigned long cp_code=0xd77a0000;

unsigned char flag=1;
unsigned int hold_tmr=1;

unsigned int lead_tmr0;
unsigned int lead_tmr1;
unsigned char signal_tmr0;
unsigned char signal_tmr1;


void IR_tmr(unsigned char h_data, unsigned int tmr)
{
  unsigned int i;
  
  carry=0;
  h_data^=1;
  for(i=0; i<tmr; i++){
    TCNT0=255-10; //13usec
    hold_tmr=1;
    carry^=h_data;
    PORTB_Bit4=carry;
    while(hold_tmr);
  }
  PORTB_Bit4=0;
}

void IR_lead_trans(){
  IR_tmr(0,lead_tmr0);  //low 9msec
  IR_tmr(1,lead_tmr1);  //high 4.5msec
  IR_tmr(0,signal_tmr0);   //low 0.56msec
}

void IR_data_trans(){
  unsigned char i,temp;

  //lead code
  IR_lead_trans();
  
  //1   data code
  for(i=0; i<32; i++){
    temp=(cp_code>>(31-i)) & 1;
    if( temp) { IR_tmr(1,signal_tmr1); IR_tmr(0,signal_tmr0);} // 1
    else      { IR_tmr(1,signal_tmr0); IR_tmr(0,signal_tmr0);} // 0
  }
}

void in_key(){
  //0=vol-up, 1=ch-dn, 2=ch-up, 3=filter, 4=phase, 5=vol-dn, 6=mute
  //data_code=0x10, data_code+=key_data, data_code<<=3;
  
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
void tmr_set(unsigned char j){
  if(j==0) {
    lead_tmr0=680;
    lead_tmr1=340;
    signal_tmr0=43;
    signal_tmr1=130;
  }
  else if(j==1){
    lead_tmr0=660;
    lead_tmr1=330;
    signal_tmr0=43;
    signal_tmr1=120;
  }
}

#pragma vector = TIM0_OVF_vect
__interrupt void TIMER0_OVF_Handler(void)
{
  hold_tmr=0;
}

unsigned char old_key=0;
void main(void){ 
  
  DDRB=0x10;
  PORTB=0x1f;
  
  TCCR0B=2;	// 1=1prescaling, 2=8prescaling, 3=64prescaling, 4=256prescaling
  TCNT0=255-10; //13sec 
  //TCNT0=0;
  TIMSK0 = 2; // (1<<TOIE0);
  TIFR0 = 2; // (1<<TOV0);
  
  tmr_set(1); // timer setting
              // 0 = basic
              // 1 = special 1
  
  __enable_interrupt(); 
  
  in_key_save();
  old_key=key_data;
  while(flag){   
    if(flag<2) flag--;
    
    IR_data_trans();     
    in_key();
    if(old_key!=key_data) break;
    IR_tmr(1,3850); //50msec wait
  }
  
  
}


