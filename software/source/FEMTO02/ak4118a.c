#include "i2c_master.h"
#include "ak4118a.h"

unsigned char AK4118A_read_register(unsigned char devaddr, unsigned char regaddr)
{
  unsigned char data;
  
  i2c_readReg(devaddr,regaddr,&data,1);
  return data;
}

void AK4118A_write_register(unsigned char devaddr, unsigned char regaddr,unsigned char data)
{
  i2c_writeReg(devaddr, regaddr, &data, 1);
}

unsigned char Is_there_AK4118A(unsigned char ic_addr)
{
  unsigned char temp1,temp2,temp3;
  
  temp1=AK4118A_read_register(AK4118A_I2C_ADDRESS(ic_addr),AK4118A_REG_STC_DAT_DETECT);
  temp2=AK4118A_read_register(AK4118A_I2C_ADDRESS(ic_addr),AK4118A_REG_RX_CHANNEL_STATUS_BYTE5);
  temp3=AK4118A_read_register(AK4118A_I2C_ADDRESS(ic_addr),AK4118A_REG_TX_CHANNEL_STATUS_BYTE5);
  
  if ((temp1>>2)!=AK4118A_RECONIZE_26H) return 0;
  else if ((temp2>>2)!=AK4118A_RECONIZE_27H) return 0;
  else if ((temp3>>2)!=AK4118A_RECONIZE_28H) return 0;
  
  return 1;
}

void AK4118A_reset(unsigned char devaddr)
{
  unsigned char reg;
  
  reg = AK4118A_read_register(devaddr, AK4118A_REG_CLK_POWER_DN_CTL);
  reg &=~AK4118A_CLK_PWR_RSTN;
  i2c_writeReg(devaddr, AK4118A_REG_CLK_POWER_DN_CTL, &reg, 1);
}

void AK4118A_power_down(unsigned char devaddr)
{
  unsigned char reg;
  
  reg = AK4118A_read_register(devaddr, AK4118A_REG_CLK_POWER_DN_CTL);
  reg &=~AK4118A_CLK_PWR_PWN;
  i2c_writeReg(devaddr, AK4118A_REG_CLK_POWER_DN_CTL, &reg, 1);
}

unsigned char AK4118A_read_current_channel(unsigned char devaddr)
{  
  return (AK4118A_read_register(devaddr, AK4118A_REG_IN_OUT_CTL1)&0x07);  
}

unsigned char AK4118A_input_select(unsigned char devaddr, unsigned char channel)
{
  unsigned char reg;
  
  reg = AK4118A_read_register(devaddr, AK4118A_REG_IN_OUT_CTL1);
  reg &= ~AK4118A_CONTROL1_IPS(0x07);
  reg |= AK4118A_CONTROL1_IPS(channel);
  i2c_writeReg(devaddr, AK4118A_REG_IN_OUT_CTL1, &reg, 1);

  reg = AK4118A_read_register(devaddr, AK4118A_REG_IN_OUT_CTL1);
  if (AK4118A_CONTROL1_IPS(channel)==(reg&0x07)) return 1; else return 0;
}

unsigned int AK4118A_read_status(unsigned char devaddr)
{
  unsigned int ret_value;
  
  ret_value = AK4118A_read_register(devaddr, AK4118A_REG_RECEIVER_STATUS0);
  ret_value <<= 8;
  ret_value |= AK4118A_read_register(devaddr, AK4118A_REG_RECEIVER_STATUS1);
  return ret_value;
}

void AK4118A_TX0_enable(unsigned char devaddr)
{
  unsigned char reg;
  
  reg = AK4118A_read_register(devaddr, AK4118A_REG_IN_OUT_CTL0);
  reg |=AK4118A_CONTROL0_TX0E;
  i2c_writeReg(devaddr, AK4118A_REG_IN_OUT_CTL0, &reg, 1);
}
 
void AK4118A_TX1_enable(unsigned char devaddr)
{
  unsigned char reg;
  
  reg = AK4118A_read_register(devaddr, AK4118A_REG_IN_OUT_CTL0);
  reg |=AK4118A_CONTROL0_TX1E;
  i2c_writeReg(devaddr, AK4118A_REG_IN_OUT_CTL0, &reg, 1);
}
 
void AK4118A_TX0_disable(unsigned char devaddr)
{
  unsigned char reg;
  
  reg = AK4118A_read_register(devaddr, AK4118A_REG_IN_OUT_CTL0);
  reg &=~AK4118A_CONTROL0_TX0E;
  i2c_writeReg(devaddr, AK4118A_REG_IN_OUT_CTL0, &reg, 1);
}

void AK4118A_TX1_disable(unsigned char devaddr)
{
  unsigned char reg;
  
  reg = AK4118A_read_register(devaddr, AK4118A_REG_IN_OUT_CTL0);
  reg &=~AK4118A_CONTROL0_TX1E;
  i2c_writeReg(devaddr, AK4118A_REG_IN_OUT_CTL0, &reg, 1);
}

unsigned char AK4118A_read_channel_detect(unsigned char devaddr)
{  
  return (AK4118A_read_register(devaddr, AK4118A_REG_STC_DAT_DETECT));  
}

unsigned char AK4118A_read_sampling_freq(unsigned char devaddr)
{ 
  unsigned char reg;
  
  reg = AK4118A_read_register(devaddr, AK4118A_REG_RECEIVER_STATUS1);
  reg>>=4;
  return reg;  
}
          
void AK4118A_BCU_enable(unsigned char devaddr)
{
  unsigned char reg;
  
  reg = AK4118A_read_register(devaddr, AK4118A_REG_CLK_POWER_DN_CTL);
  reg |=AK4118A_CLK_PWR_BCU;
  i2c_writeReg(devaddr, AK4118A_REG_CLK_POWER_DN_CTL, &reg, 1);
}
void AK4118A_BCU_disable(unsigned char devaddr)
{
  unsigned char reg;
  
  reg = AK4118A_read_register(devaddr, AK4118A_REG_CLK_POWER_DN_CTL);
  reg &=~AK4118A_CLK_PWR_BCU;
  i2c_writeReg(devaddr, AK4118A_REG_CLK_POWER_DN_CTL, &reg, 1);
}

void AK4118A_set_audio_format(unsigned char devaddr, unsigned char mode)
{
  unsigned char reg;
  
  reg = AK4118A_read_register(devaddr, AK4118A_REG_FORMAT_DE_EM_CTL);
  reg &= ~AK4118A_FORMAT_DIF(0x07);
  reg |= AK4118A_FORMAT_DIF(mode);
  i2c_writeReg(devaddr, AK4118A_REG_FORMAT_DE_EM_CTL, &reg, 1);
}