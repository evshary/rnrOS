#ifndef _INIT_H
#define _INIT_H

void usart_init();
void usart_write(const char *str, int len);
int usart_read(char *str, int len);

#endif
