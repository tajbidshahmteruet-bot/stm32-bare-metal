#ifndef USART3_H
#define USART3_H

#include <stdint.h>

void usart3_init(void);
void usart3_send_char(char c);
void usart3_send_string(const char *str);
int _write(int file, char *ptr, int len);


#endif