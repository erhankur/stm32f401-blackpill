#ifndef _IO_H_
#define _IO_H_

#include "stm32f4xx_gpio.h"

enum {
    IO_MODE_INPUT,
    IO_MODE_OUTPUT,
    IO_MODE_ALTERNATE,
    IO_MODE_ANALOG,

    IO_MODE_INPUT_PD,
    IO_MODE_INPUT_PU,
    IO_MODE_OUTPUT_OD,
    IO_MODE_ALTERNATE_OD,
    IO_MODE_ALTERNATE_OD_PU,
};

enum {
    IO_PORT_A,
    IO_PORT_B,
    IO_PORT_C,
    IO_PORT_D,
    IO_PORT_E,
    IO_PORT_F,
    IO_PORT_G,
    IO_PORT_H,
    IO_PORT_I,
    IO_PORT_J,
    IO_PORT_K,
};

/* must be same order with s_io_pins */
enum {
    /* buttons */
    IO_INPUT_0 = 0,
    IO_INPUT_1,
    IO_INPUT_2,

    /* leds */
    IO_LED_1,
    IO_LED_2,
    IO_LED_3,

    /* uarts */
    IO_UART1_TX,
    IO_UART1_RX,
    IO_UART2_TX,
    IO_UART2_RX,

    IO_MAX_NUM
};

typedef struct {
    int port;
    int pin;
} io_pin_t;

extern GPIO_TypeDef *gpio_ports[];
extern const io_pin_t io_pins[];

void io_init(int pin, int mode);

static inline void io_set(int pin)
{
    gpio_ports[io_pins[pin].port]->ODR |= (1 << io_pins[pin].pin);
}

static inline void io_reset(int pin)
{
    gpio_ports[io_pins[pin].port]->ODR &= ~(1 << io_pins[pin].pin);
}

static inline void io_write(int pin, int val)
{
    val ? io_set(pin) : io_reset(pin);
}

static inline void io_toggle_pin(int pin)
{
    gpio_ports[io_pins[pin].port]->ODR ^= (1 << io_pins[pin].pin);
}


#endif
