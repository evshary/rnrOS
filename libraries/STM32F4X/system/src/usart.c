#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "usart.h"

#define MAX_BUF 1024

typedef struct {
    char buf[MAX_BUF];
    int start;
    int end;
} RING_BUF;

void ringbuf_init(RING_BUF *ring_buf)
{
    ring_buf->start = 0;
    ring_buf->end = 0;
}

#define RINGBUF_FULL(ring_buf) ring_buf->start != (ring_buf->end+1) % MAX_BUF
#define RINGBUF_EMPTY(ring_buf) ring_buf->start == ring_buf->end

int ringbuf_len(RING_BUF *ring_buf)
{
    if (ring_buf->end > ring_buf->start) {
        return (ring_buf->end - ring_buf->start);
    } else if (ring_buf->start > ring_buf->end) {
        return (ring_buf->end + MAX_BUF - ring_buf->start);
    } else {
        return 0;
    }
}

int ringbuf_write(RING_BUF *ring_buf, char data)
{
    if (RINGBUF_FULL(ring_buf)) {
        ring_buf->buf[ring_buf->end] = data;
        ring_buf->end = (ring_buf->end+1) % MAX_BUF;
        return 0;
    } else {
        return -1;
    }
}

int ringbuf_read(RING_BUF *ring_buf, char *data)
{
    if (RINGBUF_EMPTY(ring_buf)) return -1;

    *data = ring_buf->buf[ring_buf->start];
    ring_buf->start = (ring_buf->start+1) % MAX_BUF;
    return 0;
}

static RING_BUF usart2_buf;

void usart_init()
{
    GPIO_InitTypeDef gpio_struct;
    USART_InitTypeDef usart;

	/* pheripheral clock should be enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    /* init GPIO */
    /* PA2=TX, PA3=RX */
    gpio_struct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    gpio_struct.GPIO_Mode = GPIO_Mode_AF;
    gpio_struct.GPIO_OType = GPIO_OType_PP;
    gpio_struct.GPIO_PuPd = GPIO_PuPd_UP;
    gpio_struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_struct);

    /* Connect GPIO pins to AF */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	usart.USART_BaudRate = 9600;
    usart.USART_WordLength = USART_WordLength_8b;
    usart.USART_StopBits = USART_StopBits_1;
    usart.USART_Parity = USART_Parity_No;
    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &usart);
    USART_Cmd(USART2, ENABLE);

    /* Enable receiver interrupt for USART2 */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART2, ENABLE); // Enable USART2

    ringbuf_init(&usart2_buf);
}

void USART2_IRQHandler(void)
{	
    if ( USART_GetITStatus(USART2,USART_IT_RXNE) != RESET ) { // check USART2 interrupt flag  
        USART_ClearITPendingBit(USART2,USART_IT_RXNE); // clear interrupt flag
        ringbuf_write(&usart2_buf, USART_ReceiveData(USART2));
    }   
}

void usart_write(const char *str, int len)
{
    const char *ptr = str;
	int i;

	for (i = 0; i < len; i++) {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
        USART_SendData(USART2, *ptr);
        ptr++;
    }
}

int usart_read(char *str, int len)
{
    int i, min_len;

    if (len > ringbuf_len(&usart2_buf)) {
        min_len = ringbuf_len(&usart2_buf);
    } else {
        min_len = len;
    }

    for (i = 0; i < min_len; i++) {
        ringbuf_read(&usart2_buf, &str[i]);
    }
    return min_len;
}
