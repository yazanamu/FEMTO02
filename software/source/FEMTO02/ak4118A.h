#ifndef _AVR_AK4118A_H_
#define _AVR_AK4118A_H_

#include "i2c_master.h"
#include "initboard.h"

#define AK4118A_I2C_BASE_ADDRESS     0x20
#define AK4118A_I2C_ADDRESS(x)       (AK4118A_I2C_BASE_ADDRESS | (((x) & 0x3)<<1))

#define AK4118A_REG_CLK_POWER_DN_CTL 0x00
#define AK4118A_REG_FORMAT_DE_EM_CTL 0x01
#define AK4118A_REG_IN_OUT_CTL0      0x02
#define AK4118A_REG_IN_OUT_CTL1      0x03
#define AK4118A_REG_INT0_MASK        0x04
#define AK4118A_REG_INT1_MASK        0x05
#define AK4118A_REG_RECEIVER_STATUS0 0x06
#define AK4118A_REG_RECEIVER_STATUS1 0x07
#define AK4118A_REG_RX_CH_STAT_BYTE0 0x08
#define AK4118A_REG_RX_CH_STAT_BYTE1 0x09
#define AK4118A_REG_RX_CH_STAT_BYTE2 0x0A
#define AK4118A_REG_RX_CH_STAT_BYTE3 0x0B
#define AK4118A_REG_RX_CH_STAT_BYTE4 0x0C
#define AK4118A_REG_RX_CH_STAT_BYTE5 0x27
#define AK4118A_REG_TX_CH_STAT_BYTE0 0x0D
#define AK4118A_REG_TX_CH_STAT_BYTE1 0x0E
#define AK4118A_REG_TX_CH_STAT_BYTE2 0x0F
#define AK4118A_REG_TX_CH_STAT_BYTE3 0x10
#define AK4118A_REG_TX_CH_STAT_BYTE4 0x11
#define AK4118A_REG_TX_CH_STAT_BYTE5 0x28
#define AK4118A_REG_BURST_PREAMBLE_PC_BYTE0 0x12
#define AK4118A_REG_BURST_PREAMBLE_PC_BYTE1 0x13
#define AK4118A_REG_BURST_PREAMBLE_PD_BYTE0 0x14
#define AK4118A_REG_BURST_PREAMBLE_PD_BYTE1 0x15
#define AK4118A_REG_STC_DAT_DETECT          0x26
#define AK4118A_REG_RX_CHANNEL_STATUS_BYTE5 0x27
#define AK4118A_REG_TX_CHANNEL_STATUS_BYTE5 0x28

#define AK4118A_QINT                (0x01<<7)
#define AK4118A_AUTO                (0x01<<6)
#define AK4118A_CINT                (0x01<<5)
#define AK4118A_UNLCK               (0x01<<4)
#define AK4118A_DTSCD               (0x01<<3)
#define AK4118A_PEM                 (0x01<<2)
#define AK4118A_AUDION              (0x01<<1)
#define AK4118A_PAR                 (0x01<<0)

#define AK4118A_STATUS1_FS(x)               (((x)&0x0F)<<4)
#define AK4118A_STATUS1_QCRC                (0x01<<1)
#define AK4118A_STATUS1_CCRC                (0x01<<0)

#define AK4118A_CLK_PWR_CS12        (0x01<<7)
#define AK4118A_CLK_PWR_BCU         (0x01<<6)
#define AK4118A_CLK_PWR_CM(x)       (((x)&0x03)<<4)
#define AK4118A_CLK_PWR_OCKS(x)     (((x)&0x03)<<2)
#define AK4118A_CLK_PWR_PWN         (0x01<<1)
#define AK4118A_CLK_PWR_RSTN        (0x01<<0)

#define AK4118A_FORMAT_MONO         (0x01<<7)
#define AK4118A_FORMAT_DIF(x)       (((x)&0x07)<<4)
#define AK4118A_FORMAT_DEAU         (0x01<<3)
#define AK4118A_FORMAT_DEM(x)       (((x)&0x03)<<1)
#define AK4118A_FORMAT_DFS          (0x01<<0)

#define AK4118A_CONTROL0_TX1E       (0x01<<7)
#define AK4118A_CONTROL0_OPS1(x)    (((x)&0x07)<<4)
#define AK4118A_CONTROL0_TX0E       (0x01<<3)
#define AK4118A_CONTROL0_OPS0(x)    (((x)&0x07)<<0)

#define AK4118A_CONTROL1_EFH(x)     (((x)&0x03)<<6)
#define AK4118A_CONTROL1_UDIT       (0x01<<5)
#define AK4118A_CONTROL1_TLR        (0x01<<4)
#define AK4118A_CONTROL1_DIT        (0x01<<3)
#define AK4118A_CONTROL1_IPS(x)     (((x)&0x07)<<0)

#define AK4118A_RECONIZE_26H        0x0
#define AK4118A_RECONIZE_27H        0x0
#define AK4118A_RECONIZE_28H        0x10

#define AK4118A_DETECT_CH7          (0x01<<7)
#define AK4118A_DETECT_CH6          (0x01<<6)
#define AK4118A_DETECT_CH5          (0x01<<5)
#define AK4118A_DETECT_CH4          (0x01<<4)
#define AK4118A_DETECT_CH3          (0x01<<3)
#define AK4118A_DETECT_CH2          (0x01<<2)
#define AK4118A_DETECT_CH1          (0x01<<1)
#define AK4118A_DETECT_CH0          (0x01<<0)

unsigned char Is_there_AK4118A(unsigned char ic_addr);

#endif



