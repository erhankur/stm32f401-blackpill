#include <stddef.h>
#include "stm32f4xx.h"
#include "drv/io.h"

const int s_gpio_clocks[] = {
    RCC_AHB1Periph_GPIOA,
    RCC_AHB1Periph_GPIOB,
    RCC_AHB1Periph_GPIOC,
    RCC_AHB1Periph_GPIOD,
    RCC_AHB1Periph_GPIOE,
    RCC_AHB1Periph_GPIOF,
    RCC_AHB1Periph_GPIOG,
    RCC_AHB1Periph_GPIOH,
    RCC_AHB1Periph_GPIOI,
    RCC_AHB1Periph_GPIOJ,
    RCC_AHB1Periph_GPIOK,
};

GPIO_TypeDef *gpio_ports[] = {
    GPIOA,
    GPIOB,
    GPIOC,
    GPIOD,
    GPIOE,
    GPIOF,
    GPIOG,
    GPIOH,
    GPIOI,
    GPIOJ,
    GPIOK,
};
#define NPORTS  (sizeof(s_gpio_clocks) / sizeof(int))

const io_pin_t io_pins[IO_MAX_NUM] = {
    /* buttons */
    {.port = IO_PORT_A, .pin = 0},     /* IO_INPUT_0 */
    {.port = IO_PORT_A, .pin = 1},     /* IO_INPUT_1 */
    {.port = IO_PORT_A, .pin = 2},     /* IO_INPUT_2 */

    /* leds */
    {.port = IO_PORT_B, .pin = 3},     /* LED1  */
    {.port = IO_PORT_B, .pin = 4},     /* LED2  */
    {.port = IO_PORT_B, .pin = 5},     /* LED3  */

    /* uarts */
    {.port = IO_PORT_A, .pin = 9},     /* UART1_TX  */
    {.port = IO_PORT_A, .pin = 10},    /* UART1_RX  */
    {.port = IO_PORT_A, .pin = 2},     /* UART2_TX  */
    {.port = IO_PORT_A, .pin = 3},     /* UART2_RX  */
};

void io_clocks_init(void)
{
    for (size_t i = 0; i < NPORTS; ++i) {
        RCC_AHB1PeriphClockCmd(s_gpio_clocks[i], ENABLE);
    }
}

void io_init(int pin, int mode)
{
    static int gpio_clock_init = 0;

    if (!gpio_clock_init) {
        io_clocks_init();
        gpio_clock_init = 1;
    }

    GPIO_InitTypeDef GPIO_InitStructure;

    switch (mode) {
    case IO_MODE_INPUT:
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        break;

    case IO_MODE_INPUT_PD:
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        break;

    case IO_MODE_INPUT_PU:
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        break;

    case IO_MODE_OUTPUT:
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        break;

    case IO_MODE_OUTPUT_OD:
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
        break;

    case IO_MODE_ALTERNATE:
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        break;

    case IO_MODE_ALTERNATE_OD:
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
        break;

    case IO_MODE_ALTERNATE_OD_PU:
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
        break;

    case IO_MODE_ANALOG:
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        break;
    }

    GPIO_InitStructure.GPIO_Pin = (1 << io_pins[pin].pin);
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
    GPIO_Init(gpio_ports[io_pins[pin].port], &GPIO_InitStructure);
}

