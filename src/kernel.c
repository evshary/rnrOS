#include "usart.h"

asm(".word 0x20001000");
asm(".word main");
int main()
{
    usart_init();
    usart_print("Hello, rnrOS!\r\n");
    while (1);
}
