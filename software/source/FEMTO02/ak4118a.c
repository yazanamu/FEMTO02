#include "i2c_master.h"
#include "ak4118a.h"

unsigned char AK4118A_read_register(unsigned char devaddr, unsigned char regaddr)
{
  unsigned char data;
  
  i2c_readReg(devaddr,regaddr,&data,1);
  return data;
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


