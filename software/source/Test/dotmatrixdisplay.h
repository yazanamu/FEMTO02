#ifndef __AVR_DOTMATRIXDISPLAY_H
#define __AVR_DOTMATRIXDISPLAY_H

void dot_matrix_set(void);
void dot_vol_hextodeci(unsigned char number);
void dot_0x_hexadecimal(unsigned char number);
void dot_matrix_bright(unsigned char bright);
void dot_matrix_digit2(unsigned char data, unsigned char xdigit);
void dot_matrix_digit(unsigned char data, unsigned char xdigit);
void dot_string_digit(void);
void dot_matrix_clear(void);

#endif