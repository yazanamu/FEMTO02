#ifndef _AVR_TWI_H
#define _AVR_TWI_H

#define 	TW_START   0x08
 
#define 	TW_REP_START   0x10
 
#define 	TW_MT_SLA_ACK   0x18
 
#define 	TW_MT_SLA_NACK   0x20
 
#define 	TW_MT_DATA_ACK   0x28
 
#define 	TW_MT_DATA_NACK   0x30
 
#define 	TW_MT_ARB_LOST   0x38
 
#define 	TW_MR_ARB_LOST   0x38
 
#define 	TW_MR_SLA_ACK   0x40
 
#define 	TW_MR_SLA_NACK   0x48
 
#define 	TW_MR_DATA_ACK   0x50
 
#define 	TW_MR_DATA_NACK   0x58
 
#define 	TW_ST_SLA_ACK   0xA8
 
#define 	TW_ST_ARB_LOST_SLA_ACK   0xB0
 
#define 	TW_ST_DATA_ACK   0xB8
 
#define 	TW_ST_DATA_NACK   0xC0
 
#define 	TW_ST_LAST_DATA   0xC8
 
#define 	TW_SR_SLA_ACK   0x60
 
#define 	TW_SR_ARB_LOST_SLA_ACK   0x68
 
#define 	TW_SR_GCALL_ACK   0x70
 
#define 	TW_SR_ARB_LOST_GCALL_ACK   0x78
 
#define 	TW_SR_DATA_ACK   0x80
 
#define 	TW_SR_DATA_NACK   0x88
 
#define 	TW_SR_GCALL_DATA_ACK   0x90
 
#define 	TW_SR_GCALL_DATA_NACK   0x98
 
#define 	TW_SR_STOP   0xA0
 
#define 	TW_NO_INFO   0xF8
 
#define 	TW_BUS_ERROR   0x00
 
#define 	TW_STATUS_MASK 0xF8
 
#define 	TW_STATUS   (TWSR & TW_STATUS_MASK)
 
#define 	TW_READ   1
 
#define 	TW_WRITE   0


/* enables an unsigned char to be used as a series of booleans */
#define BIT(x)	            (1 << (x))
#define SETBIT(x, y)   	    (x |= y)
#define CLEARBIT(x, y) 	    (x &= ~y)
#define CHECKBIT(x, y) 	    (x & y)

// ***** Define I/O pins *****
#define BIT7 0x80
#define BIT6 0x40
#define BIT5 0x20
#define BIT4 0x10
#define BIT3 0x08
#define BIT2 0x04
#define BIT1 0x02
#define BIT0 0x01

#endif