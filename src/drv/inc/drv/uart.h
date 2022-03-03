#ifndef _UART_H_
#define _UART_H_

enum {
    UART_1,
    UART_2,
    UART_3,
    UART_4,
    UART_5,
    UART_6,
};

void uart_init(int idx, int baud);
void uart_send(int idx, unsigned char ch);
void uart_send_buffer(int idx, const unsigned char *buffer, size_t len);
unsigned char uart_recv(int idx);
void uart_flush(int idx);

#endif
