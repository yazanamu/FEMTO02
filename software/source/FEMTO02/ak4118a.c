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
  unsigned char reset = ~AK4118A_CLK_PWR_RSTN;
  
  i2c_writeReg(devaddr, AK4118A_REG_CLK_POWER_DN_CTL, &reset, 1);
}

void AK4118A_power_down(unsigned char devaddr, unsigned char onoff)
{
  unsigned char reg;
  
  reg = AK4118A_read_register(devaddr, AK4118A_REG_CLK_POWER_DN_CTL);
  if (onoff) reg &=~AK4118A_CLK_PWR_PWN; else reg |=AK4118A_CLK_PWR_PWN;
  
  i2c_writeReg(devaddr, AK4118A_REG_CLK_POWER_DN_CTL, &reg, 1);
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