#define U8 unsigned char
#define U16 unsigned int
unsigned char inByte,grAck;

void I2C_START(void)
{
   SDA_Out=High;
   SCL=High;      DelayTime(10);
   SDA_Out=Low;   DelayTime(40);
   SCL=Low;       DelayTime(150);
}
	 
void I2C_STOP(void)
{
   DelayTime(150);
   SDA_Out=Low;   DelayTime(10);
   SCL=High;      DelayTime(40);
   SDA_Out=High;
}

U8 I2C_SHOUT(U8 outByte)  //I2C DATA Write[Device Address, Sub Address, Data]
{	
   U8 index,goAck;
   for(index = 0; index < 8; index++){
   SCL=Low; DelayTime(10);                      //클럭의 low, high의 사이클 맞춤                    
   SDA_Out=((outByte&0x80)?High:Low);
   outByte<<=1;
   SCL=High; 
   DelayTime(10);                            
   }
   SCL=Low;   DelayTime(20);

   SDA_ST&=Low;
   SDA_Out=High;
   DelayTime(10);
   
   if(SDA_In==High)   goAck=High;
   else               goAck=Low;
   
   SDA_Out=goAck;
   DelayTime(20);

   SCL=High;    DelayTime(30);
   SCL=Low;    
   SDA_ST=High; DelayTime(100);
   return goAck;                                 //ACK 정보 리턴
}



void I2C_SHIN(U8 ck)       //I2C DATA Read[Device Address, Sub Address, Data]
{	

   U8 index; 
   
   inByte=0; grAck=0;  
   DelayTime(10); 
   SDA_ST=Low;
   SDA_Out=High;
   DelayTime(10);   

   for(index = 0; index < 8; index++)
   {
        SCL=Low; DelayTime(10);                 //클럭의 low, high의 사이클 맞춤                    
        inByte|=(SDA_In?High:Low);
        if(index<7) inByte<<=1;
        SCL=High; DelayTime(10);
   }
   
   SCL=Low;
   DelayTime(30);
   SDA_ST=High; 
   SDA_Out=Low;
   
   if(ck==Low) grAck=Low;
   else        grAck=High;
   
   SDA_Out=grAck;
   DelayTime(20);
   

   SCL=High;   DelayTime(30);
   SCL=Low;    DelayTime(100);  

}
 
void I2C_Write(U8 slave_add, U8 start_add, U8 add_num) //I2C Write Mode
{

   I2C_START();
   I2C_SHOUT(slave_add);	//device address
   I2C_SHOUT(start_add);//Sub address   
   I2C_SHOUT(add_num);
   I2C_STOP();	

}

void rom_cnt_check(){
  //AT24C16

  if(rom_cnt>0x000c3500){    
    if(rom_add_pt<0x07ff){
      rom_add_pt+=5;
      if((rom_add_pt&0x000f) == 0x000f) rom_add_pt++;
      rom_cnt=0;
      if(rom_add_pt<0x07ff){
        rom_add_pt_high=(U8)((rom_add_pt >> 8) & 0xff);
        rom_add_pt_low=(U8)(rom_add_pt & 0xff);
      }
      else {
        rom_add_pt_high=0x80;
        rom_add_pt_low=0;
      }
      rom_add_check_sum=rom_add_pt_high+rom_add_pt_low;
      
      I2C_START();
      I2C_SHOUT(0xa0);        //device address
      I2C_SHOUT(0x00);        //Sub address   
      I2C_SHOUT(rom_add_pt_high);       //
      I2C_SHOUT(rom_add_pt_low);       //
      I2C_SHOUT(rom_add_check_sum);       //
      I2C_STOP();
    
      DelayTime_ms(10);  //5msec
    }
  }
  else{
    rom_cnt++;
  }
}

void rom_write_multi(){
  //AT24C16BN
  
  rom_cnt_check();
  
  if(rom_add_pt<0x07ff){
    rom_device_high=(U8)(0xa0 + ((rom_add_pt>>7) & 0x000e));
    rom_device_low=(U8)(rom_add_pt & 0x00ff);
    
    rom_cnt_high=(U8)((rom_cnt>>16)&0xff);
    rom_cnt_mid=(U8)((rom_cnt>>8)&0xff);
    rom_cnt_low=(U8)(rom_cnt & 0xff);
    
    rom_func_data=ch_led_data;
    rom_func_data<<=3;
    rom_func_data+=filter_flag;
    rom_func_data<<=1;
    rom_func_data+=phase_data;
    
    I2C_START();
    I2C_SHOUT(rom_device_high);	//device address
    I2C_SHOUT(rom_device_low);       //Sub address   
    I2C_SHOUT(rom_cnt_high);       //counter,MSB    
    I2C_SHOUT(rom_cnt_mid);       //counter
    I2C_SHOUT(rom_cnt_low);       //counter, LSB
    I2C_SHOUT(vol_dB);       //volume
    I2C_SHOUT(rom_func_data);       //ch, filter, phase
    I2C_STOP();
  }
}

unsigned char I2C_Read(U8 slave_add, U8 start_add)  //I2C Read Mode
{
   U8 data;

   
   I2C_START();
   I2C_SHOUT(slave_add);	//device address (write)
   I2C_SHOUT(start_add);	//sub address
   I2C_STOP();
   I2C_START();
   I2C_SHOUT(slave_add+1);	//device address (read)
   I2C_SHIN(High);
   data = inByte;
   I2C_STOP();
   
   return data ;
}
/*
void rom_I2C_Write(U16 address, U8 data){
  U8 temp1,temp2;
  
  temp1=(0xa0 + ((address>>7) & 0x000e));
  temp2=(address & 0x00ff);
  I2C_Write(temp1, temp2, data);
  DelayTime_ms(5);  //5msec
}
*/
unsigned char rom_I2C_Read(U16 address){
  U8 data;
  U8 temp1, temp2;
  
  temp1=(0xa0 + ((address>>7) & 0x000e));
  temp2=(address & 0x00ff);
  data=I2C_Read(temp1,temp2);
  
  return data;
}

void reset_eeprom(U16 address){
  U8 temp1,temp2;
  
  temp1=(0xa0 + ((address>>7) & 0x000e));
  temp2=(address & 0x00ff);
  I2C_Write(temp1, temp2, 0xff);
  DelayTime_ms(10);  //5msec
}

void test_set_eeprom(U16 address, U8 data){
  U8 temp1,temp2;
  
  temp1=(0xa0 + ((address>>7) & 0x000e));
  temp2=(address & 0x00ff);
  I2C_Write(temp1, temp2, data);
  DelayTime_ms(5);  //5msec
}


/*
void system_reset(){
  dot_matrix_clear();
}

unsigned char I2C_Read(unsigned char device_address, unsigned char start_address){
	unsigned char DATA=0;
	unsigned char error=0;
	unsigned int counter=0;
	unsigned char i2c_error_num=10;
	unsigned int counter_num=1000;

	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN) ;		//START condition	
	while ( ( ! ( TWCR & (1<<TWINT) ) )  && (counter<counter_num) )   counter++;	//Wait for TWINT Flag set.
	if(counter==counter_num)  error++;
	if ( ( TWSR & 0xF8 ) == 0x08 )  ;//error+=1 ;			//Check value of TWI Status Register. ok flag
	//error<<=1;
	counter =0;
	
	TWDR = device_address;					//Load SLA_W into TWDR Register.
	TWCR = ( 1<<TWINT ) | ( 1<<TWEN ) ;			//Clear TWINT bit in TWCR to start transmission of address
	while ( ( ! ( TWCR & (1<<TWINT) ) )  && (counter<counter_num) )   counter++;	//Wait for TWINT Flag set.
	if(counter==counter_num)  error++;
	if ( ( TWSR & 0xF8 ) == 0x18 )  ;//error+=1 ;			//Check value of TWI Status Register. ok flag
	//error<<=1;
	counter =0;
	
	TWDR = start_address ;					//Load SLA_W into TWDR Register.
	TWCR = ( 1<<TWINT ) | ( 1<<TWEN ) ;			//Clear TWINT bit in TWCR to start transmission of address
	while ( ( ! ( TWCR & (1<<TWINT) ) )  && (counter<counter_num) )   counter++;	//Wait for TWINT Flag set.
	if(counter==counter_num)  error++;
	if ( ( TWSR & 0xF8 ) == 0x28 )  ;//error+=1 ;			//Check value of TWI Status Register. ok flag
	//error<<=1;
	counter =0;
		
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN) ;		//Repeated START condition
	while ( ( ! ( TWCR & (1<<TWINT) ) )  && (counter<counter_num) )   counter++;	//Wait for TWINT Flag set.
	if(counter==counter_num)  error++;
	if ( ( TWSR & 0xF8 ) == 0x10 )  ;//error+=1 ;			//Check value of TWI Status Register. ok flag	
	//error<<=1 ;
	counter =0;
	
	TWDR = device_address+0x01 ;				//SLA+R
	TWCR = ( 1<<TWINT ) | ( 1<<TWEN ) ;			//Clear TWINT bit in TWCR to start transmission of data	
	while ( ( ! ( TWCR & (1<<TWINT) ) )  && (counter<counter_num) )   counter++;	//Wait for TWINT Flag set.
	if(counter==counter_num)  error++;
	if ( ( TWSR & 0xF8 ) == 0x40 )  ;//error+=1 ;			//Check value of TWI Status Register. ok flag
	//error<<=1;
	counter =0;
	
	TWCR = ( 1<<TWINT ) | ( 1<<TWEN ) ;			//Read data with no acknowledge
	while ( ( ! ( TWCR & (1<<TWINT) ) )  && (counter<counter_num) )   counter++;	//Wait for TWINT Flag set.
	if(counter==counter_num)  error++;
	if ( ( TWSR & 0xF8 ) == 0x58 )  ;//error+=1 ;			//Check value of TWI Status Register. ok flag
	DATA = TWDR ;						//Read Data
	
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO) ;		//STOP condition
	
	//I2C_result[y][2]=error;
	
	if(error) I2C_error++;
	else I2C_error=0;
	
	if(I2C_error>i2c_error_num)     system_reset();
	return DATA ;						//Return Data
}

void I2C_Write(unsigned char device_address, unsigned char start_address, unsigned char DATA){
	
	unsigned char error=0;
	unsigned int counter=0;
	unsigned char i2c_error_num=10;
	unsigned int counter_num=1000;
	
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN) ;		//START condition	
	while ( ( ! ( TWCR & (1<<TWINT) ) )  && (counter<counter_num) )   counter++;	//Wait for TWINT Flag set.
	if(counter==counter_num)  error++;
	if ( ( TWSR & 0xF8 ) == 0x08 )  ;//error+=1 ;			//Check value of TWI Status Register. ok flag
	//error<<=1;
	counter =0;
	
	TWDR = device_address ;					//Load SLA_W into TWDR Register.
	TWCR = ( 1<<TWINT ) | ( 1<<TWEN ) ;			//Clear TWINT bit in TWCR to start transmission of address
	while ( ( ! ( TWCR & (1<<TWINT) ) )  && (counter<counter_num) )   counter++;	//Wait for TWINT Flag set.
	if(counter==counter_num)  error++;
	if ( ( TWSR & 0xF8 ) == 0x18 )  ;//error+=1 ;			//Check value of TWI Status Register. ok flag
	//error<<=1;
	counter =0;
	
	TWDR = start_address ;					//Load SLA_W into TWDR Register.
	TWCR = ( 1<<TWINT ) | ( 1<<TWEN ) ;			//Clear TWINT bit in TWCR to start transmission of address
	while ( ( ! ( TWCR & (1<<TWINT) ) )  && (counter<counter_num) )   counter++;	//Wait for TWINT Flag set.
	if(counter==counter_num)  error++;
	if ( ( TWSR & 0xF8 ) == 0x28 )  ;//error+=1 ;			//Check value of TWI Status Register. ok flag
	//error<<=1;
	counter =0;
	
	TWDR = DATA ;						//Load DATA into TWDR Register.
	TWCR = ( 1<<TWINT ) | ( 1<<TWEN ) ;			//Clear TWINT bit in TWCR to start transmission of data	
	while ( ( ! ( TWCR & (1<<TWINT) ) )  && (counter<counter_num) )   counter++;	//Wait for TWINT Flag set.
	if(counter==counter_num)  error++;
	if ( ( TWSR & 0xF8 ) == 0x28 ) ;// error+=1 ;			//Check value of TWI Status Register. ok flag
	
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO) ;		//STOP condition
	
	//I2C_result[y][2]=error;
	
	if(error) I2C_error++;
	else I2C_error=0;
	
	if(I2C_error>i2c_error_num)     system_reset();
}
*/


/*
unsigned char I2C_Read(unsigned char device_address, unsigned char start_address){
	unsigned char DATA=0;
	//unsigned char error=0;
	
	//I2C_result[y][0]=device_address;
	//I2C_result[y][1]=start_address;
	
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN) ;		//START condition	
	while ( ! ( TWCR & (1<<TWINT) ) ) ;				//Wait for TWINT Flag set.
	if ( ( TWSR & 0xF8 ) == 0x08 )  ;//error+=1 ;			//Check value of TWI Status Register. ok flag
	//error<<=1;
	
	TWDR = device_address;					//Load SLA_W into TWDR Register.
	TWCR = ( 1<<TWINT ) | ( 1<<TWEN ) ;			//Clear TWINT bit in TWCR to start transmission of address
	while ( ! ( TWCR & (1<<TWINT) ) ) ;				//Wait for TWINT Flag set.
	if ( ( TWSR & 0xF8 ) == 0x18 )  ;//error+=1 ;			//Check value of TWI Status Register. ok flag
	//error<<=1;
	
	TWDR = start_address ;					//Load SLA_W into TWDR Register.
	TWCR = ( 1<<TWINT ) | ( 1<<TWEN ) ;			//Clear TWINT bit in TWCR to start transmission of address
	while ( ! ( TWCR & (1<<TWINT) ) ) ;				//Wait for TWINT Flag set.
	if ( ( TWSR & 0xF8 ) == 0x28 )  ;//error+=1 ;			//Check value of TWI Status Register. ok flag
	//error<<=1;
		
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN) ;		//Repeated START condition
	while ( ! ( TWCR & (1<<TWINT) ) ) ;				//Wait for TWINT Flag set.
	if ( ( TWSR & 0xF8 ) == 0x10 )  ;//error+=1 ;			//Check value of TWI Status Register. ok flag	
	//error<<=1 ;
	
	TWDR = device_address+0x01 ;				//SLA+R
	TWCR = ( 1<<TWINT ) | ( 1<<TWEN ) ;			//Clear TWINT bit in TWCR to start transmission of data	
	while ( ! ( TWCR & (1<<TWINT) ) ) ;				//Wait for TWINT Flag set.
	if ( ( TWSR & 0xF8 ) == 0x40 )  ;//error+=1 ;			//Check value of TWI Status Register. ok flag
	//error<<=1;
	
	TWCR = ( 1<<TWINT ) | ( 1<<TWEN ) ;			//Read data with no acknowledge
	while ( ! ( TWCR & (1<<TWINT) ) ) ;				//Wait for TWINT Flag set.
	if ( ( TWSR & 0xF8 ) == 0x58 )  ;//error+=1 ;			//Check value of TWI Status Register. ok flag
	DATA = TWDR ;						//Read Data
	
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO) ;		//STOP condition
	
	//I2C_result[y][2]=error;
	
	return DATA ;						//Return Data
}

void I2C_Write(unsigned char device_address, unsigned char start_address, unsigned char DATA){
	
	//unsigned char error=0;
	
	//I2C_result[y][0]=device_address;
	//I2C_result[y][1]=start_address;
	
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN) ;		//START condition	
	while ( ! ( TWCR & (1<<TWINT) ) ) ;				//Wait for TWINT Flag set.	
	if ( ( TWSR & 0xF8 ) == 0x08 )  ;//error+=1 ;			//Check value of TWI Status Register. ok flag
	//error<<=1;
	
	TWDR = device_address ;					//Load SLA_W into TWDR Register.
	TWCR = ( 1<<TWINT ) | ( 1<<TWEN ) ;			//Clear TWINT bit in TWCR to start transmission of address
	while ( ! ( TWCR & (1<<TWINT) ) ) ;				//Wait for TWINT Flag set	
	if ( ( TWSR & 0xF8 ) == 0x18 )  ;//error+=1 ;			//Check value of TWI Status Register. ok flag
	//error<<=1;
	
	TWDR = start_address ;					//Load SLA_W into TWDR Register.
	TWCR = ( 1<<TWINT ) | ( 1<<TWEN ) ;			//Clear TWINT bit in TWCR to start transmission of address
	while ( ! ( TWCR & (1<<TWINT) ) ) ;				//Wait for TWINT Flag set	
	if ( ( TWSR & 0xF8 ) == 0x28 )  ;//error+=1 ;			//Check value of TWI Status Register. ok flag
	//error<<=1;
	
	TWDR = DATA ;						//Load DATA into TWDR Register.
	TWCR = ( 1<<TWINT ) | ( 1<<TWEN ) ;			//Clear TWINT bit in TWCR to start transmission of data	
	while ( ! ( TWCR & (1<<TWINT) ) ) ;				//Wait for TWINT Flag set.
	if ( ( TWSR & 0xF8 ) == 0x28 ) ;// error+=1 ;			//Check value of TWI Status Register. ok flag
	
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO) ;		//STOP condition
	
	//I2C_result[y][2]=error;
}
*/
