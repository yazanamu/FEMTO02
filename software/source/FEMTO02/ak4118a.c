#include "i2c_master.h"
#include "initboard.h"
#include "ak4118a.h"


unsigned char AK4118A_read_register(unsigned char devaddr, unsigned char regaddr)
{
  unsigned char data[40];
  i2c_receive(devaddr|I2C_READ, data, 40);
  return data[regaddr];
}


unsigned char Is_there_AK4118A(void)
{
  unsigned char temp1,temp2,temp3;
  
  //i2c_readReg(AK4118A_I2C_ADDRESS(0)|I2C_READ, AK4118A_REG_STC_DAT_DETECT, &temp1, 1);
  temp1=AK4118A_read_register(AK4118A_I2C_ADDRESS(0),AK4118A_REG_STC_DAT_DETECT);
  //i2c_readReg(AK4118A_I2C_ADDRESS(0)|I2C_READ, AK4118A_REG_RX_CHANNEL_STATUS_BYTE5, &temp2, 1);
  temp2=AK4118A_read_register(AK4118A_I2C_ADDRESS(0),AK4118A_REG_RX_CHANNEL_STATUS_BYTE5);
  //i2c_readReg(AK4118A_I2C_ADDRESS(0)|I2C_READ, AK4118A_REG_TX_CHANNEL_STATUS_BYTE5, &temp3, 1);
  temp3=AK4118A_read_register(AK4118A_I2C_ADDRESS(0),AK4118A_REG_TX_CHANNEL_STATUS_BYTE5);
  
  if ((temp1>>2)!=AK4118A_RECONIZE_26H) return 0;
  else if ((temp2>>2)!=AK4118A_RECONIZE_27H) return 0;
  else if ((temp3>>2)!=AK4118A_RECONIZE_28H) return 0;
  
  return 1;

}


