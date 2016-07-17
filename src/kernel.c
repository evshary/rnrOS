#include "usart.h"

asm(".word 0x20001000");
asm(".word main");
int main()
{
    char str[1024] = {0};
    int len = 0;

    usart_init();
    usart_write("Hello, rnrOS!\r\n", 15);
    while (1){
        len = usart_read(str, 1024);
        usart_write(str, len);
        len = 0;
    }
}
