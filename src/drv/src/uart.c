#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#include "stm32f4xx.h"

#include "io.h"
#include "uart.h"

static USART_TypeDef *s_uarts[] = {
    USART1,
    USART2,
    USART3,
};

#define N_UARTS     (sizeof(s_uarts) / sizeof(USART_TypeDef *))

struct uart_config {
    int           io_tx;
    int           io_rx;
    uint8_t       af;     /* alternate function */
    uint32_t      clk;
};

static struct uart_config s_uart_cfg[] = {
    {IO_UART1_TX, IO_UART1_RX, GPIO_AF_USART1, RCC_APB2Periph_USART1},
    {IO_UART2_TX, IO_UART2_RX, GPIO_AF_USART2, RCC_APB1Periph_USART2},
};

void uart_init(int idx, int baud)
{
    assert_param((size_t)idx < N_UARTS);

    int io = s_uart_cfg[idx].io_tx;
    int port = io_pins[io].port;
    int pin = io_pins[io].pin;

    io_init(io, IO_MODE_ALTERNATE);
    GPIO_PinAFConfig(gpio_ports[port], pin, s_uart_cfg[idx].af);

    io = s_uart_cfg[idx].io_tx;
    port = io_pins[io].port;
    pin = io_pins[io].pin;

    io_init(s_uart_cfg[idx].io_rx, IO_MODE_ALTERNATE);
    GPIO_PinAFConfig(gpio_ports[port], pin, s_uart_cfg[idx].af);

    if (idx == UART_1 || idx == UART_6) {
        RCC_APB2PeriphClockCmd(s_uart_cfg[idx].clk, ENABLE);
    } else {
        RCC_APB1PeriphClockCmd(s_uart_cfg[idx].clk, ENABLE);
    }

    USART_InitTypeDef uart_init;
    uart_init.USART_BaudRate = baud;
    uart_init.USART_WordLength = USART_WordLength_8b;
    uart_init.USART_StopBits = USART_StopBits_1;
    uart_init.USART_Parity = USART_Parity_No;
    uart_init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    uart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

    USART_Init(s_uarts[idx], &uart_init);
    USART_Cmd(s_uarts[idx], ENABLE);
}

void uart_send(int idx, unsigned char ch)
{
    USART_SendData(s_uarts[idx], ch);
    /* wait until tx complete */
    while (!USART_GetFlagStatus(s_uarts[idx], USART_FLAG_TC));
}

void uart_send_buffer(int idx, const unsigned char *buffer, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        uart_send(idx, buffer[i]);
    }
}

unsigned char uart_recv(int idx)
{
    while (!USART_GetFlagStatus(s_uarts[idx], USART_FLAG_RXNE));
    return (unsigned char)USART_ReceiveData(s_uarts[idx]);
}

void uart_flush(int idx)
{
    while (USART_GetFlagStatus(s_uarts[idx], USART_FLAG_RXNE)) {
        USART_ReceiveData(s_uarts[idx]);
    }
}


