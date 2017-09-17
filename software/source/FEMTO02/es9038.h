#ifndef _AVR_ES9038_H_
#define _AVR_ES9038_H_

#include "i2c_master.h"
#include "initboard.h"

#define ES9038_ADDR0  0x90      // Left channel
#define ES9038_ADDR1  0x92      // Right channel
#define ES9038_REG_SYSTEM 0x00
#define ES9038_REG_INPUT_SEL 0x01
#define ES9038_REG_SERIALDATA_AUTOMUTE 0x02
#define ES9038_REG_AUTOMUTE_TIME 0x04
#define ES9038_REG_AUTOMUTE_LEVEL 0x05
#define ES9038_REG_FILTERBW_MUTE 0x07
#define ES9038_REG_JITTER_ELIMINATOR_DPLL_BW 0x0B
#define ES9038_REG_GPIO_INPUT_SEL_VOLUME_CONFIG 0x0F
#define ES9038_REG_VOLUME 0x10        // 0x10~0x17
#define ES9038_REG_JITTER_DPLLBW 0x0C

#define ES9038_REG_CHIP_ID_STATUS 0x40
#define ES9038_REG_VOLUME1_CONTROL 0x16
#define ES9038_REG_VOLUME2_CONTROL 0x11
#define ES9038_REG_VOLUME3_CONTROL 0x12
#define ES9038_REG_VOLUME4_CONTROL 0x13
#define ES9038_REG_VOLUME5_CONTROL 0x14
#define ES9038_REG_VOLUME6_CONTROL 0x15
#define ES9038_REG_VOLUME7_CONTROL 0x16
#define ES9038_REG_VOLUME8_CONTROL 0x17
#define ES9038_REG_MASTER_TRIM 0x18 //32bit
#define ES9038_REG_DAC_CH_MAPPING_DAC12 0x26
#define ES9038_REG_DAC_CH_MAPPING_DAC34 0x27
#define ES9038_REG_DAC_CH_MAPPING_DAC56 0x28
#define ES9038_REG_DAC_CH_MAPPING_DAC78 0x29

#define ES9038_SYSTEM_SOFT_RESET        0x01<<0
#define ES9038_GPIO_INPUT_SEL(x)        (((x)&0x03)<<6)
#define ES9038_VOLUME_CFG_STEREO_MODE   0x01<<2
#define ES9038_VOLUME_CFG_CH1_VOL       0x01<<1
#define ES9038_VOLUME_CFG_LATCH_VOL     0x01<<0
#define ES9038_FILTER_SHAPE(x)          (((x)&0x07)<<5)
#define ES9038_FILTER_IIR_BW(x)         (((x)&0x03)<<1)
#define ES9038_DPLL_BW_SERIAL(x)        (((x)&0x04)<<4)
#define ES9038_DPLL_BW_DSD(x)           (((x)&0x04)<<0)

#define ES9038_SYSTEM_MUTE              0x01<<0
#define ES9038_AUTOMUTE_TIME(x)         (x)
#define ES9038_AUTOMUTE_LEVEL(x)        (((x)&0x3F)<<0)   

#define ES9038_DAC_CH1_MAP(x)           (((x)&0x0F)<<0)
#define ES9038_DAC_CH2_MAP(x)           (((x)&0x0F)<<4)
#define ES9038_DAC_CH3_MAP(x)           (((x)&0x0F)<<0)
#define ES9038_DAC_CH4_MAP(x)           (((x)&0x0F)<<4)
#define ES9038_DAC_CH5_MAP(x)           (((x)&0x0F)<<0)
#define ES9038_DAC_CH6_MAP(x)           (((x)&0x0F)<<4)
#define ES9038_DAC_CH7_MAP(x)           (((x)&0x0F)<<0)
#define ES9038_DAC_CH8_MAP(x)           (((x)&0x0F)<<4)



#define ES9038_CHIP_ID(x) (((x)&0x3F)<<2)
#define ES9038PRO_CHIP_ID 0x2A




enum automute_cfg { AUTOMUTE_NORMAL, AUTOMUTE_ONLYMUTE, AUTOMUTE_ALLGND, AUTOMUTE_RAMPNGND };
enum serial_mode { I2S_MODE, LEFTJUST_MODE, RIGHTJUST_MODE };
#define REG_AUTOMUTE_LEVEL 0x05



#define ES9038_ADDR0_MUTE es9038_write_register(ES9038_ADDR0,ES9038_REG_FILTERBW_MUTE,0x41)
#define ES9038_ADDR1_MUTE es9038_write_register(ES9038_ADDR1,ES9038_REG_FILTERBW_MUTE,0x41)


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

void es9038_resetb(void)
{
  
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
  
  i2c_readReg(ES9038_ADDR0, ES9038_REG_CHIP_ID_STATUS, &temp0, 1);
  i2c_readReg(ES9038_ADDR1, ES9038_REG_CHIP_ID_STATUS, &temp1, 1);
  
  if ((temp0>>2)==ES9038PRO_CHIP_ID) ret_value|=0x01;
  if ((temp1>>2)==ES9038PRO_CHIP_ID) ret_value|=0x02;
  
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

void es9038_automute_level(unsigned char devaddr, unsigned char level)
{
  es9038_write_register(devaddr,ES9038_REG_AUTOMUTE_LEVEL,level & 0x3F);
}

void es9038_set_volume(unsigned char volume)
{
  unsigned char i;
  
  for (i=0;i<8;i++) {
    //I2C_Write(ES9038_ADDR0,REG_VOLUME+i,volume);
    //I2C_Write(ES9038_ADDR1,REG_VOLUME+i,volume);
  }
}



#endif
