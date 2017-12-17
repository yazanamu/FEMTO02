#include <iom128.h>
#include <inavr.h>
#include "i2c_master.h"
#include "initboard.h"
#include "define.h"
#include "es9038.h"

unsigned char es9038_read_volume(unsigned char devaddr);

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
  char data;

  i2c_readReg(devaddr, regaddr, &data, 1);
  
  return data;
}

void es9038_write_register(unsigned char devaddr, unsigned char regaddr, char data)
{
  i2c_writeReg(devaddr, regaddr, &data, 1);
}

void es9038_dac_mute_on(void)
{
  es9038_system_mute(ES9038_ADDR0,1); es9038_system_mute(ES9038_ADDR1,1);
}

void es9038_dac_mute_off(void)
{
  es9038_system_mute(ES9038_ADDR0,0); es9038_system_mute(ES9038_ADDR1,0);
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

void es9038_set_ch1_volume_for_all(unsigned char devaddr,unsigned char onoff)
{
  unsigned char reg;
  
  reg=es9038_read_register(devaddr,ES9038_REG_GPIO_INPUT_SEL_VOLUME_CONFIG);
  if (onoff) reg|=ES9038_VOLUME_CFG_CH1_VOL; else reg&=~ES9038_VOLUME_CFG_CH1_VOL;
  es9038_write_register(devaddr,ES9038_REG_GPIO_INPUT_SEL_VOLUME_CONFIG,reg);
  
}

unsigned char es9038_read_volume(unsigned char devaddr)
{  return es9038_read_register(devaddr,ES9038_REG_VOLUME1_CONTROL); }

unsigned char es9038_set_volume(unsigned char devaddr,unsigned char volume_db)
{
  es9038_set_ch1_volume_for_all(devaddr,1);
  es9038_write_register(devaddr, ES9038_REG_VOLUME1_CONTROL,volume_db);
  //es9038_write_register(devaddr, ES9038_REG_VOLUME2_CONTROL,volume_db);
  //es9038_write_register(devaddr, ES9038_REG_VOLUME3_CONTROL,volume_db);
  //es9038_write_register(devaddr, ES9038_REG_VOLUME4_CONTROL,volume_db);
  //es9038_write_register(devaddr, ES9038_REG_VOLUME5_CONTROL,volume_db);
  //es9038_write_register(devaddr, ES9038_REG_VOLUME6_CONTROL,volume_db);
  //es9038_write_register(devaddr, ES9038_REG_VOLUME7_CONTROL,volume_db);
  es9038_write_register(devaddr, ES9038_REG_VOLUME8_CONTROL,volume_db);
  if (es9038_read_volume(devaddr)==volume_db) return 1; else return 0;
}

void es9038_soft_reset(unsigned char devaddr)
{
  es9038_write_register(devaddr,ES9038_REG_SYSTEM,ES9038_SYSTEM_SOFT_RESET);
}

void es9038_set_dpll_bw_dsd(unsigned char devaddr,unsigned char bandwidth)
{
  unsigned char reg;
  
  reg=es9038_read_register(devaddr,ES9038_REG_JITTER_DPLLBW);
  reg&=~ES9038_DPLL_BW_DSD(0xF);
  reg|=ES9038_DPLL_BW_DSD(bandwidth);
  es9038_write_register(devaddr,ES9038_REG_JITTER_DPLLBW,reg);  
}

void es9038_18db_channel_gain_control(unsigned char devaddr,unsigned char ch, unsigned char onoff)
{
  unsigned char sel_ch=0x01,reg;
  
  reg=es9038_read_register(devaddr,ES9038_REG_18DB_CHANNEL_GAIN);
  sel_ch<<=(ch-1); // Channel = 1 to 8
  if(onoff) reg|=sel_ch; else reg&=~sel_ch;
  es9038_write_register(devaddr,ES9038_REG_18DB_CHANNEL_GAIN,reg);
}

unsigned long es9038_read_dpll_number(unsigned char devaddr)
{
  unsigned long dpll_num=0;
  unsigned char reg;
  int i;
  
  for(i=3;i>=0;i--) {
    reg = es9038_read_register(devaddr,ES9038_REG_DPLL_NUM+i);
    //send_byte2hex(reg);
    dpll_num |= reg;
    if (i>0) dpll_num <<=8;
  }
  return dpll_num;
}

unsigned char es9038_is_dsd_mode(unsigned char devaddr)
{
  //unsigned char reg;
  
  //reg=es9038_read_register(devaddr,ES9038_REG_STATUS);
  
  //if (reg & ES9038_STATUS_DSD_PCM) return 1;
  //else 
  return 0;
}

void es9038_set_volume_rate(unsigned char devaddr, unsigned char volume_rate)
{
  unsigned char reg;
  
  reg=es9038_read_register(devaddr,ES9038_REG_VOLUME_RAMP_RATE);
  reg&=~ES9038_VOLUME_RATE(0x7);
  reg|=ES9038_VOLUME_RATE(volume_rate);
  es9038_write_register(devaddr,ES9038_REG_VOLUME_RAMP_RATE,reg); 
}

void es9038_set_filter_shape(unsigned char devaddr, unsigned char filter)
{
  unsigned char reg;
  
  reg  = es9038_read_register(devaddr,ES9038_REG_FILTERBW_MUTE);
  reg &= ~ES9038_FILTER_SHAPE(0x7);
  reg |=  ES9038_FILTER_SHAPE(filter);
  es9038_write_register(devaddr,ES9038_REG_FILTERBW_MUTE,reg); 
}

void es9038_set_soft_start_time(unsigned char devaddr, unsigned char time)
{
  unsigned char reg;
  
  reg  = es9038_read_register(devaddr,ES9038_REG_SOFT_START_CFG);
  reg &= ~ES9038_SOFT_START_TIME(0xF);
  reg |=  ES9038_SOFT_START_TIME(time);
  es9038_write_register(devaddr,ES9038_REG_SOFT_START_CFG,reg); 
}

void es9038_set_gpio1(unsigned char devaddr, unsigned char gpio_cfg)
{
  unsigned char reg;
  
  reg  = es9038_read_register(devaddr,ES9038_REG_GPIO12_CFG);
  reg &= ~ES9038_GPIO1_CFG(0xF);
  reg |=  ES9038_GPIO1_CFG(gpio_cfg);
  es9038_write_register(devaddr,ES9038_REG_GPIO12_CFG,reg); 
}

unsigned char es9038_read_lock_status(unsigned char devaddr)
{
  if (es9038_read_register(devaddr,ES9038_REG_CHIP_ID_STATUS)&ES9038_LOCK_STATUS) return 1;
  else return 0;
}