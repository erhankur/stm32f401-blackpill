#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "stm32f4xx.h"

#include "drv/io.h"
#include "drv/wdts.h"
#include "drv/delay.h"
#include "drv/log.h"
#include "app/version.h"


void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)(xTask); (void)(pcTaskName);
    LOG_E("Stack overflow at task: %s", pcTaskName);
    assert_param(pdTRUE);
}

/* make sure the FreeRTOS task switching interrupt is not blocked for a long time. */
void vApplicationTickHook(void)
{
    wwdg_reset();
}

/* detect instances of tasks running for a prolonged period of time without yielding. */
void vApplicationIdleHook(void)
{
    iwdg_reset();
}

static void cpu_init(void)
{
    /* Reset the RCC clock configuration to the default reset state. */
    /* HSI ON, PLL OFF, HSE OFF, system clock = 16MHz, cpu_clock = 16MHz */
    RCC_DeInit();
    SystemCoreClockUpdate();  /* 160Mhz */
}

static void __attribute__ ((used)) wdt_init(void)
{
    if (iwdg_init(IWDG_TIMEOUT_4s)) {
        LOG_W("Application reset by IWDG");
    }
    /* Must be bigger then portTICK_PERIOD_MS */
    if (wwdg_init()) {
        LOG_W("Application reset by WWDG");
    }
}

static void print_clock_info(void)
{
    RCC_ClocksTypeDef rcc_clocks;
    RCC_GetClocksFreq(&rcc_clocks);

    LOG_I("PCLK1_Frequency: %d", rcc_clocks.PCLK1_Frequency);
    LOG_I("SYSCLK_Frequency: %d", rcc_clocks.SYSCLK_Frequency);
}

static void led1_task(void *params)
{
    (void)(params);

    io_init(IO_LED_1, IO_MODE_OUTPUT);

    while (pdTRUE) {
        io_toggle_pin(IO_LED_1);
        vTaskDelay(pdMS_TO_TICKS(750));
    }
}

static void led2_task(void *params)
{
    (void)(params);

    io_init(IO_LED_2, IO_MODE_OUTPUT);

    while (pdTRUE) {
        io_toggle_pin(IO_LED_2);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
static void led3_task(void *params)
{
    (void)(params);

    io_init(IO_LED_3, IO_MODE_OUTPUT);

    while (pdTRUE) {
        io_toggle_pin(IO_LED_3);
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}

static void dummy_task(void *params)
{
    (void)(params);
    TaskStatus_t xTaskDetails;

    vTaskGetInfo( /* The handle of the task being queried. */
        NULL,
        /* The TaskStatus_t structure to complete with information
        on xTask. */
        &xTaskDetails,
        /* Include the stack high water mark value in the
        TaskStatus_t structure. */
        pdTRUE,
        /* Include the task state in the TaskStatus_t structure. */
        eInvalid);

    while (pdTRUE) {
        LOG_I("I am %s", xTaskDetails.pcTaskName);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void led_process_start(void)
{
    xTaskCreate(led1_task, "led1_task", 512, NULL, 1, NULL);
    xTaskCreate(led2_task, "led2_task", 512, NULL, 1, NULL);
    xTaskCreate(led3_task, "led3_task", 512, NULL, 1, NULL);
    xTaskCreate(dummy_task, "dummy_task", 130, NULL, 1, NULL);
}

int main(void)
{
    cpu_init();

    log_level_init("BlackPill", LOG_LEVEL_INFO);
    LOG_I("application (%s) has started...", PROJECT_VER);
    print_clock_info();

    led_process_start();

    wdt_init();

    vTaskStartScheduler();

    while (pdTRUE)
        ;
}
