#ifndef _AVR_ES9038_H_
#define _AVR_ES9038_H_

#include "I2C.h"

#define ES9038_ADDR0  0x90
#define ES9038_ADDR1  0x92
#define REG_SYSTEM 0x00
#define REG_INPUT_SEL 0x01
#define REG_FILTERBW_MUTE 0x07
#define REG_VOLUME 0x10        // 0x10~0x17
#define REG_JITTER_DPLLBW 0x0C
#define REG_SERIALDATA_AUTOMUTE 0x02
enum automute_cfg { AUTOMUTE_NORMAL, AUTOMUTE_ONLYMUTE, AUTOMUTE_ALLGND, AUTOMUTE_RAMPNGND };
enum serial_mode { I2S_MODE, LEFTJUST_MODE, RIGHTJUST_MODE };
#define REG_AUTOMUTE_LEVEL 0x05



#define ES9038_ADDR0_MUTE I2C_Write(ES9038_ADDR0,REG_FILTERBW_MUTE,0x41)
#define ES9038_ADDR1_MUTE I2C_Write(ES9038_ADDR1,REG_FILTERBW_MUTE,0x41)


void es9038_set_volume(unsigned char volume)
{
  unsigned char i;
  
  for (i=0;i<8;i++) {
    I2C_Write(ES9038_ADDR0,REG_VOLUME+i,volume);
    I2C_Write(ES9038_ADDR1,REG_VOLUME+i,volume);
  }
}

void es9038_set_dpll_bw(unsigned char serial,unsigned char dsd)
{
  I2C_Write(ES9038_ADDR0,REG_JITTER_DPLLBW,(serial<<4)&(dsd));
  I2C_Write(ES9038_ADDR1,REG_JITTER_DPLLBW,(serial<<4)&(dsd));
}

void es9038_set_automute_level(unsigned char level)
{
  I2C_Write(ES9038_ADDR0,REG_AUTOMUTE_LEVEL,level & 0xEF);
  I2C_Write(ES9038_ADDR1,REG_AUTOMUTE_LEVEL,level & 0xEF);
}

void es9038_serial_data_config_automute(unsigned char automute, unsigned char serial_mode)
{
  I2C_Write(ES9038_ADDR0,REG_SERIALDATA_AUTOMUTE,(automute<<6) & 0x3C & (serial_mode&0x03));
  I2C_Write(ES9038_ADDR1,REG_SERIALDATA_AUTOMUTE,(automute<<6) & 0x3C & (serial_mode&0x03));
}




#endif
