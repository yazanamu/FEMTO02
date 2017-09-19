#include <iom128.h>
#include <inavr.h>
#include "i2c_master.h"
#include "initboard.h"
#include "define.h"
#include "es9038.h"


void es9038_set_dpll_bw(unsigned char serial,unsigned char dsd)
{
  //I2C_Write(ES9038_ADDR0,REG_JITTER_DPLLBW,(serial<<4)&(dsd));
  //I2C_Write(ES9038_ADDR1,REG_JITTER_DPLLBW,(serial<<4)&(dsd));
}

void es9038_set_automute_level(unsigned char level)
{
  //I2C_Write(ES9038_ADDR0,REG_AUTOMUTE_LEVEL,level & 0xEF);
  //I2C_Write(ES9038_ADDR1,REG_AUTOMUTE_LEVEL,level & 0xEF);
}

void es9038_serial_data_config_automute(unsigned char automute, unsigned char serial_mode)
{
  //I2C_Write(ES9038_ADDR0,REG_SERIALDATA_AUTOMUTE,(automute<<6) & 0x3C & (serial_mode&0x03));
  //I2C_Write(ES9038_ADDR1,REG_SERIALDATA_AUTOMUTE,(automute<<6) & 0x3C & (serial_mode&0x03));
}

unsigned char es9038_read_register(unsigned char devaddr, unsigned char regaddr)
{
  unsigned char data;

  i2c_readReg(devaddr, regaddr, &data, 1);
  
  return data;
}

void es9038_write_register(unsigned char devaddr, unsigned char regaddr, unsigned char data)
{
  i2c_writeReg(devaddr, regaddr, &data, 1);
}

void es9038_dac_mute_on(void)
{
  DAC_MUTE_DDR |= DAC_MUTE_PIN;
  DAC_MUTE_PORT |= DAC_MUTE_PIN;
}

void es9038_dac_mute_off(void)
{
  DAC_MUTE_DDR |= DAC_MUTE_PIN;
  DAC_MUTE_PORT &= ~DAC_MUTE_PIN;
}

unsigned char Is_there_ES9038(void)
{
  unsigned char temp0,temp1,ret_value=0;
  
  temp1=es9038_read_register(ES9038_ADDR1,ES9038_REG_CHIP_ID_STATUS);
  if ((temp1>>2)==ES9038PRO_CHIP_ID) ret_value+=2;
  temp0=es9038_read_register(ES9038_ADDR0,ES9038_REG_CHIP_ID_STATUS);
  if ((temp0>>2)==ES9038PRO_CHIP_ID) ret_value+=1;
  
  return ret_value;
}

void es9038_system_mute(unsigned char devaddr, unsigned char onoff)
{
  unsigned char reg;
  
  reg=es9038_read_register(devaddr,ES9038_REG_FILTERBW_MUTE);
  if (onoff) reg|=0x01; else reg&=0xFE;
  es9038_write_register(devaddr,ES9038_REG_FILTERBW_MUTE,reg);
}

void es9038_ch1_volume(unsigned char devaddr, unsigned char onoff)
{
  unsigned char reg;
  
  reg=es9038_read_register(devaddr,ES9038_REG_GPIO_INPUT_SEL_VOLUME_CONFIG);
  if (onoff) reg|=0x01<<1; else reg&=~(0x01<<1);
  es9038_write_register(devaddr,ES9038_REG_GPIO_INPUT_SEL_VOLUME_CONFIG,reg);
}

void es9038_audio_auto_select(unsigned char devaddr, unsigned char select)
{
  unsigned char reg;
  
  reg=es9038_read_register(devaddr,ES9038_REG_INPUT_SEL);
  reg&=~(ES9038_INPUT_AUTO_SEL(0x03));
  reg|=ES9038_INPUT_AUTO_SEL(select);
  es9038_write_register(devaddr,ES9038_REG_INPUT_SEL,reg);
}

void es9038_audio_input_select(unsigned char devaddr, unsigned char select)
{
  unsigned char reg;
  
  reg=es9038_read_register(devaddr,ES9038_REG_INPUT_SEL);
  reg&=~ES9038_INPUT_SEL(0x03);
  reg|=ES9038_INPUT_SEL(select);
  es9038_write_register(devaddr,ES9038_REG_INPUT_SEL,reg);
}



void es9038_automute_level(unsigned char devaddr, unsigned char level)
{
  es9038_write_register(devaddr,ES9038_REG_AUTOMUTE_LEVEL,level & 0x3F);
}

void es9038_set_volume(unsigned char volume)
{
  unsigned char i;
  
  for (i=0;i<8;i++) {
    es9038_write_register(ES9038_ADDR0,ES9038_REG_VOLUME1_CONTROL+i,volume);
    es9038_write_register(ES9038_ADDR1,ES9038_REG_VOLUME1_CONTROL+i,volume);
  }
}

void es9038_dac_channel_mapping(unsigned char devaddr,unsigned char dac_ch,unsigned char input_ch)
{
  unsigned char reg;
  
  switch (dac_ch) {
    case 1:
      reg=es9038_read_register(devaddr,ES9038_REG_DAC_CH_MAPPING_DAC12);
      reg&=~ES9038_DAC_CH1_MAP(0xF);
      reg|=ES9038_DAC_CH1_MAP(input_ch);
      es9038_write_register(devaddr,ES9038_REG_DAC_CH_MAPPING_DAC12,reg);
      break;
    case 2:
      reg=es9038_read_register(devaddr,ES9038_REG_DAC_CH_MAPPING_DAC12);
      reg&=~ES9038_DAC_CH2_MAP(0xF);
      reg|=ES9038_DAC_CH2_MAP(input_ch);
      es9038_write_register(devaddr,ES9038_REG_DAC_CH_MAPPING_DAC12,reg);
      break;
    case 3:
      reg=es9038_read_register(devaddr,ES9038_REG_DAC_CH_MAPPING_DAC34);
      reg&=~ES9038_DAC_CH3_MAP(0xF);
      reg|=ES9038_DAC_CH3_MAP(input_ch);
      es9038_write_register(devaddr,ES9038_REG_DAC_CH_MAPPING_DAC34,reg);
      break;
    case 4:
      reg=es9038_read_register(devaddr,ES9038_REG_DAC_CH_MAPPING_DAC34);
      reg&=~ES9038_DAC_CH4_MAP(0xF);
      reg|=ES9038_DAC_CH4_MAP(input_ch);
      es9038_write_register(devaddr,ES9038_REG_DAC_CH_MAPPING_DAC34,reg);
      break;
    case 5:
      reg=es9038_read_register(devaddr,ES9038_REG_DAC_CH_MAPPING_DAC56);
      reg&=~ES9038_DAC_CH5_MAP(0xF);
      reg|=ES9038_DAC_CH5_MAP(input_ch);
      es9038_write_register(devaddr,ES9038_REG_DAC_CH_MAPPING_DAC56,reg);
      break;
    case 6:
      reg=es9038_read_register(devaddr,ES9038_REG_DAC_CH_MAPPING_DAC56);
      reg&=~ES9038_DAC_CH6_MAP(0xF);
      reg|=ES9038_DAC_CH6_MAP(input_ch);
      es9038_write_register(devaddr,ES9038_REG_DAC_CH_MAPPING_DAC56,reg);
      break;
    case 7:
      reg=es9038_read_register(devaddr,ES9038_REG_DAC_CH_MAPPING_DAC78);
      reg&=~ES9038_DAC_CH7_MAP(0xF);
      reg|=ES9038_DAC_CH7_MAP(input_ch);
      es9038_write_register(devaddr,ES9038_REG_DAC_CH_MAPPING_DAC78,reg);
      break;
    case 8:
      reg=es9038_read_register(devaddr,ES9038_REG_DAC_CH_MAPPING_DAC78);
      reg&=~ES9038_DAC_CH8_MAP(0xF);
      reg|=ES9038_DAC_CH8_MAP(input_ch);
      es9038_write_register(devaddr,ES9038_REG_DAC_CH_MAPPING_DAC78,reg);
      break;
  }
}



void es9038_soft_reset(unsigned char devaddr)
{
  es9038_write_register(devaddr,ES9038_REG_SYSTEM,ES9038_SYSTEM_SOFT_RESET);
}

void es9038_18db_channel_gain_control(unsigned char devaddr,unsigned char ch, unsigned char onoff)
{
  unsigned char sel_ch=0x01,reg;
  
  reg=es9038_read_register(devaddr,ES9038_REG_18DB_CHANNEL_GAIN);
  sel_ch<<=(ch-1); // Channel = 1 to 8
  if(onoff) reg|=sel_ch; else reg&=~sel_ch;
  es9038_write_register(devaddr,ES9038_REG_18DB_CHANNEL_GAIN,reg); 
}

