#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "stm32f4xx.h"

#include "drv/types.h"
#include "drv/log.h"
#include "drv/uart.h"

#define LOG_UART    UART_1
static log_level_t s_log_level = LOG_LEVEL_OFF;
static char s_tag[32] = { 0 };
static const char *const s_level_str[] = { "OFF", "E", "W", "I", "D" };
static char s_log_buffer[4096] = {0};
static SemaphoreHandle_t s_log_mutex = NULL;

#define MAX_MUTEX_WAIT_MS 50
#define MAX_MUTEX_WAIT_TICKS ((MAX_MUTEX_WAIT_MS + portTICK_PERIOD_MS - 1) / portTICK_PERIOD_MS)

bool log_lock_timeout(void)
{
    if (!s_log_mutex) {
        s_log_mutex = xSemaphoreCreateMutex();
    }
    if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) {
        return true;
    }
    return xSemaphoreTake(s_log_mutex, MAX_MUTEX_WAIT_TICKS) == pdTRUE;
}

void log_unlock(void)
{
    if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) {
        return;
    }
    xSemaphoreGive(s_log_mutex);
}

static void log_writev(log_level_t level, const char *str)
{
    if (!log_lock_timeout()) {
        return;
    }
    if (s_log_level >= level) {
        uart_send_buffer(LOG_UART, (uint8_t *)str, strlen(str));
    }
    log_unlock();
}

static void log_init(void)
{
    uart_init(LOG_UART, 115200);
    uart_flush(LOG_UART);
}

void log_write_format(log_level_t level, const char *fmt, ...)
{
    if (s_log_level >= level) {
        va_list ap;
        snprintf(s_log_buffer, sizeof(s_log_buffer), "[%s:%s:%ld]:", s_tag, s_level_str[level], xTaskGetTickCount() * portTICK_RATE_MS);
        va_start(ap, fmt);
        vsnprintf(&s_log_buffer[strlen(s_log_buffer)], sizeof(s_log_buffer) - strlen(s_log_buffer), fmt, ap);
        va_end(ap);
        strcat(s_log_buffer, "\r\n");
        log_writev(level, s_log_buffer);
    }
}

void log_level_init(const char *tag, log_level_t level)
{
    static bool first = true;

    s_log_level = level;
    if (s_log_level == LOG_LEVEL_OFF) {
        return;
    }

    if (first) {
        log_init();
        first = false;
    }

    if (tag) {
        strcpy(s_tag, tag);
    }
}

void log_hex_dump(log_level_t level, const char *caption, void *buf, size_t buf_size)
{
    if (s_log_level >= level) {
        uint8_t c;
        size_t x, line;
        uint8_t *ptr = (uint8_t *)buf;
        char lineStr[128] = {0};
        int lineInx = 0;

        log_write_format(level, "\n\r%s ptr:%p size:%d", caption, ptr, buf_size);
        log_write_format(level, "---- -----------------------------------------------  ----------------");
        for (line = 0; line < ((buf_size % 16) ? (buf_size / 16) + 1 : (buf_size / 16)); line++) {
            lineInx += sprintf(lineStr + lineInx, "%04X ", line * 16);
            for (x = 0; x < 16; x++) {
                if (x + (line * 16) < buf_size) {
                    c = ptr[x + line * 16];
                    lineInx += sprintf(lineStr + lineInx, "%02X ", c);
                } else {
                    lineInx += sprintf(lineStr + lineInx, "%s", "   ");
                }
            }
            lineInx += sprintf(lineStr + lineInx, "%s", " ");
            for (x = 0; x < 16; x++) {
                if (x + (line * 16) < buf_size) {
                    c = ptr[x + line * 16];
                    if ((c > 0x1f) && (c < 0x7f)) {
                        lineInx += sprintf(lineStr + lineInx, "%c", c);
                    } else {
                        lineInx += sprintf(lineStr + lineInx, "%s", ".");
                    }
                } else {
                    lineInx += sprintf(lineStr + lineInx, "%s", " ");
                }
            }
            log_write_format(level, "%s", lineStr);
            lineInx = 0;
        }
        log_write_format(level, "---- -----------------------------------------------  ----------------");
    }
}

void assert_failed(uint8_t *file, uint32_t line)
{
#ifdef USE_FULL_ASSERT
#ifdef DEBUG_BUILD
    const char *file_name = strrchr((char *)file, '/') ? strrchr((char *)file, '/') + 1 : (char *)file;
    log_write_format(0, "<<<<<< Assertion Failed at F: %s L: %d >>>>>>", file_name, line);
    __asm("bkpt 1");
#else
    (void)(file); (void)(line);
    NVIC_SystemReset();
#endif
#else
    (void)(file); (void)(line);
    NVIC_SystemReset();
#endif
}
