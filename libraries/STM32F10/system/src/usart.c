#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "usart.h"

void usart_init()
{
    GPIO_InitTypeDef gpio_struct;
    USART_InitTypeDef usart;

	/* pheripheral clock should be enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    /* Configure USART2 Rx pin as floating input. */
    gpio_struct.GPIO_Pin = GPIO_Pin_3;
    gpio_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpio_struct);

    /* Configure USART2 Tx as alternate function push-pull. */
    gpio_struct.GPIO_Pin = GPIO_Pin_2;
    gpio_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_struct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpio_struct);

	usart.USART_BaudRate = 9600;
    usart.USART_WordLength = USART_WordLength_8b;
    usart.USART_StopBits = USART_StopBits_1;
    usart.USART_Parity = USART_Parity_No;
    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &usart);
    USART_Cmd(USART2, ENABLE);
}

void usart_print(const char *str)
{
    const char *ptr = str;
    while (*ptr != '\0') {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
        USART_SendData(USART2, *ptr);
        ptr++;
    }
}
