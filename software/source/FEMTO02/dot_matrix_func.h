#ifndef AVR_DOT_MATRIX_FUNC_H
#define AVR_DOT_MATRIX_FUNC_H


#define DEFAULT_MESSAGE_DELAY 6 // 3 sconds delay
#define MESSAGE_DELAY_OFF 0

void dot_matrix_clear(void);
void dot_string_digit(void);
void dot_matrix_digit(unsigned char data, unsigned char xdigit);
void dot_matrix_digit2(unsigned char data, unsigned char xdigit);
void dot_matrix_bright(unsigned char bright);
void dot_0x_hexadecimal(unsigned char number);
void dot_vol_hextodeci(unsigned char number);
void dot_matrix_set(void);

#endif // AVR_DOT_MATRIX_FUNC_H