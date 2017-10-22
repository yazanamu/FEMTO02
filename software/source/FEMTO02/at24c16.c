#include <iom128.h>
#include <inavr.h>
#include <string.h>
#include "at24c16.h"
#include "define.h"
#include "twi.h"

#ifndef _BV
#define _BV(x) 1<<(x)
#endif
#define ADDRESS_AT24C16 0xA0
#define EEPAGES 128
#define EEPAGESIZE 16
#define EEMAXADDR 0x0800
#ifndef ERROR
#define ERROR 1
#endif
#ifndef SUCCESS
#define SUCCESS (!ERROR)
#endif
#define EEPROM_ADDRESS_IDENTITY         0x0000 // "FEMTO02"
#define EEPROM_ADDRESS_VERSION          0x0010 // Version
#define EEPROM_ADDRESS_REVISION         0x0020 // Revisiton
#define EEPROM_ADDRESS_MODE             0x0030 // C1=0, C2=1, A=2, O1=3,..., O4=7, U=8
#define EEPROM_ADDRESS_LINE_VOLUME      0x0040 //
#define EEPROM_ADDRESS_HEADPHONE_VOLUME 0x0050 //
#define EEPROM_ADDRESS_MUTE             0x0060 //
#define EEPROM_ADDRESS_FILTER           0x0070 //
#define EEPROM_ADDRESS_OUTPUT_HP        0x0080 // Line-out = 0 or Headphone-out = 1 





unsigned char eeprom_write_byte(unsigned int u16addr, char u8data)
{
  begin:
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
  while ((TWCR & (1<<TWINT)) == 0);
 
  int status = TWSR & 0xF8; 
  switch( status )
  {
      case TW_REP_START: //repeated start condition transmitted 0x10
      case TW_START:    //start condition transmitted 0x08
      break;
      
      case TW_MT_ARB_LOST: //arbitration lost in SLA+W or data 0x38
      goto begin;
        
      default:
      //printf( "error:send start condition\n");
      return -1;
  }
    
  TWDR = (ADDRESS_AT24C16)|(unsigned char)((u16addr & 0x0700)>>7);
  TWCR = (1<<TWINT)|(1<<TWEN);  //버스 사용가능시 주소를 전송한다. 
  while ((TWCR & (1<<TWINT)) == 0);  //전송이 완료되면 TWINT비트는 0으로 세트된다. 즉 인터럽트 플레그가 세트된다. 
 
  status = TWSR & 0xF8;
    
  switch(status)
  {
    case TW_MT_SLA_ACK://SLA+W transmitted, ACK received 0x18
      break;
        
    case TW_MT_SLA_NACK: //SLA+W transmitted, NACK received 0x20
      goto begin;
        
    case TW_MT_ARB_LOST: //arbitration lost in SLA+W or data 0x38
      goto begin;
        
    default:
      //printf( "error: send address ");
      TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN); /* send stop condition */
      return -1;
  }
  TWDR = (unsigned char)(u16addr);
 
  TWCR = (1<<TWINT)|(1<<TWEN); //버스 사용가능시 주소를 전송한다. 
  while ((TWCR & (1<<TWINT)) == 0); //전송이 완료되면 TWINT비트는 0으로 세트된다. 즉 인터럽트 플레그가 세트된다.
    
  status = TWSR & 0xF8;
  switch(status)
  {
    case TW_MT_DATA_ACK: //data transmitted, ACK received 0x28
      break;
        
    case TW_MT_DATA_NACK: //data transmitted, NACK received 0x30
    case TW_MT_ARB_LOST:  //arbitration lost in SLA+W or data 0x38
    default:
      //printf( "error: send address ");
      TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN); /* send stop condition */
      return -1;
    }

  TWDR = u8data;      
  TWCR = (1<<TWINT)|(1<<TWEN);//데이터를 전송을 시작한다. 
  while ((TWCR & (1<<TWINT)) == 0); //전송이 완료되면 TWINT비트는 0으로 세트된다. 즉 인터럽트 플레그가 세트된다. 
 
  status = TWSR & 0xF8;
  switch(status)
  {
    case TW_MT_DATA_ACK: //data transmitted, ACK received 0x28
      break;
        
    case TW_MT_DATA_NACK: //data transmitted, NACK received 0x30
      //printf( "device write protected\n" );
    default:
      //printf( "error: send address ");
      TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN); /* send stop condition */
      return -1;
  }
 
  TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
  while (TWCR & (1<<TWSTO));//전송이 완료되면 TWSTO비트는 0으로 세트된다.

  return SUCCESS;
 
}

unsigned char eeprom_read_byte(unsigned int u16addr, char *u8data)
{
    begin:
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); //버스사용가능시 start 상태를 전송
 
    while ((TWCR & (1<<TWINT)) == 0);
 
    int status = TWSR & 0xF8;
    switch(status)
    {
        case TW_START://start condition transmitted
            break;
            
        case TW_MT_ARB_LOST:
            goto begin;    
            
        default:
            //printf( "error:send start condition\n");
            return -1;
    }
    TWDR = (ADDRESS_AT24C16)|((unsigned char)((u16addr & 0x0700)>>7));
    TWCR = (1<<TWINT)|(1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0); //전송이 완료되면 TWINT비트는 0으로 세트된다.
    status = TWSR & 0xF8;
    if (status != 0x18) //SLA+W transmitted, ACK received  TW_MT_SLA_ACK
        return ERROR;
 
    TWDR = (unsigned char)(u16addr);
    TWCR = (1<<TWINT)|(1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0);
    status = TWSR & 0xF8;
    if (status != 0x28)  //data transmitted, ACK received  TW_MT_DATA_ACK
        return ERROR;
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);  //버스 사용 가능시 start 시그널을 전송한다.
    while ((TWCR & (1<<TWINT)) == 0);
    if ( (TWSR & 0xF8) != 0x10)  //repeated start condition transmitted TW_REP_START
        return ERROR;
    TWDR = (ADDRESS_AT24C16)|((unsigned char)((u16addr & 0x0700)>>7))|1; //TW_READ =1
    TWCR = (1<<TWINT)|(1<<TWEN); //Wait till complete TWDR byte received
    while ((TWCR & (1<<TWINT)) == 0);
    if ((TWSR & 0xF8) != 0x40)  //SLA+R transmitted, ACK received  TW_MR_SLA_ACK
        return ERROR;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0); //전송이 완료될때 까지 대기 
    *u8data = TWDR; //데이터를 읽어온다.
    if ((TWSR & 0xF8) != 0x58)  //data received, NACK returned  TW_MR_DATA_NACK
        return ERROR;
    TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
    while (TWCR & (1<<TWSTO));
 
    return SUCCESS;
}

unsigned char eeprom_read_identity_error(void)   ///  not complete
{
  char string[8], i;
  char *identity = EEPROM_STRING_IDENTITY;
  unsigned char len=strlen(identity);
  
  for(i=0;i<len;i++) {
    eeprom_read_byte(EEPROM_ADDRESS_IDENTITY,&string[i]);
    //if (string[i]!=identity[i]) return ERROR;
    /////////////////////////////////////// need debugging 2017.10.22
  }

  return SUCCESS;
}

unsigned char eeprom_write_string(unsigned int address, char *str)
{
  unsigned char len=strlen(str), i, error=1;
  char read_ch;
  
  for(i=0;i<len;i++) {
    error=eeprom_read_byte(address, &read_ch);
    if (error) return error;
    if (read_ch!=str[i]) {
      error=eeprom_write_byte(address, str[i]);
      if (error) return error;
    }
  }
  return SUCCESS;
}

unsigned char eeprom_write_identity(char *str){return eeprom_write_string(EEPROM_ADDRESS_IDENTITY,str);}
unsigned char eeprom_write_version (char *str){return eeprom_write_string(EEPROM_ADDRESS_VERSION, str);}
unsigned char eeprom_write_revision(char *str){return eeprom_write_string(EEPROM_ADDRESS_REVISION,str);}
unsigned char eeprom_write_mode(unsigned char mode)
{ return eeprom_write_byte(EEPROM_ADDRESS_MODE, mode); }
unsigned char eeprom_write_line_volume(unsigned char line_volume)
{ return eeprom_write_byte(EEPROM_ADDRESS_LINE_VOLUME, line_volume); }
unsigned char eeprom_write_headphone_volume(unsigned char headphone_volume)
{ return eeprom_write_byte(EEPROM_ADDRESS_HEADPHONE_VOLUME, headphone_volume); }
unsigned char eeprom_write_mute(unsigned char mute)
{ return eeprom_write_byte(EEPROM_ADDRESS_MUTE, mute); }
unsigned char eeprom_write_filter(unsigned char filter)
{ return eeprom_write_byte(EEPROM_ADDRESS_FILTER, filter); }
unsigned char eeprom_write_output_headphone(unsigned char output_headphone)
{ return eeprom_write_byte(EEPROM_ADDRESS_OUTPUT_HP, output_headphone); }
unsigned char eeprom_read_mode(char *mode)
{ return eeprom_read_byte(EEPROM_ADDRESS_MODE, mode); }
unsigned char eeprom_read_line_volume(char *line_volume)
{ return eeprom_read_byte(EEPROM_ADDRESS_LINE_VOLUME, line_volume); }
unsigned char eeprom_read_headphone_volume(char *headphone_volume)
{ return eeprom_read_byte(EEPROM_ADDRESS_HEADPHONE_VOLUME, headphone_volume); }
unsigned char eeprom_read_mute(char *mute)
{ return eeprom_read_byte(EEPROM_ADDRESS_MUTE, mute); }
unsigned char eeprom_read_filter(char *filter)
{ return eeprom_read_byte(EEPROM_ADDRESS_FILTER, filter); }
unsigned char eeprom_read_output_headphone(char *output_headphone)
{ return eeprom_read_byte(EEPROM_ADDRESS_OUTPUT_HP, output_headphone); }

unsigned char eeprom_save(unsigned char mode,\
                          unsigned char line_vol, unsigned char headphone_vol,\
                          unsigned char mute, unsigned char filter, \
                          unsigned char output_headphone)
{
  char read_mode, read_line_vol, read_headphone_vol, read_mute, read_filter, read_out_hp;
  unsigned char error=0;
  
  if (eeprom_read_mode((&read_mode)))                    return ERROR;
  if (eeprom_read_line_volume(&read_line_vol))           return ERROR;
  if (eeprom_read_headphone_volume(&read_headphone_vol)) return ERROR;
  if (eeprom_read_mute(&read_mute))                      return ERROR;
  if (eeprom_read_filter(&read_filter))                  return ERROR;
  if (eeprom_read_output_headphone(&read_out_hp))        return ERROR;
  
  if (read_mode!=mode) if (eeprom_write_mode(mode&0x07))                         return ERROR;
  if (read_line_vol!=line_vol) \
                       if (eeprom_write_line_volume((char)line_vol))                return ERROR;
  if (read_headphone_vol!=headphone_vol) \
                       if (eeprom_write_headphone_volume((char)headphone_vol))      return ERROR;
  if (read_mute!=mute) if (eeprom_write_mute(mute&0x01))                         return ERROR;
  if (read_filter!=filter) \
                       if (eeprom_write_filter(filter))                          return ERROR;
  if (read_out_hp!=output_headphone) \
                       if (eeprom_write_output_headphone(output_headphone&0x01)) return ERROR;
  return error;
}