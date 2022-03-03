#ifndef _WDTS_H_
#define _WDTS_H_

#include <stdint.h>
#include "stm32f4xx.h"

typedef enum {
    IWDG_TIMEOUT_5ms = 0x00,   /*!< System reset called every 5ms */
    IWDG_TIMEOUT_10ms = 0x01,  /*!< System reset called every 10ms */
    IWDG_TIMEOUT_15ms = 0x02,  /*!< System reset called every 15ms */
    IWDG_TIMEOUT_30ms = 0x03,  /*!< System reset called every 30ms */
    IWDG_TIMEOUT_60ms = 0x04,  /*!< System reset called every 60ms */
    IWDG_TIMEOUT_120ms = 0x05, /*!< System reset called every 120ms */
    IWDG_TIMEOUT_250ms = 0x06, /*!< System reset called every 250ms */
    IWDG_TIMEOUT_500ms = 0x07, /*!< System reset called every 500ms */
    IWDG_TIMEOUT_1s = 0x08,    /*!< System reset called every 1s */
    IWDG_TIMEOUT_2s = 0x09,    /*!< System reset called every 2s */
    IWDG_TIMEOUT_4s = 0x0A,    /*!< System reset called every 4s */
    IWDG_TIMEOUT_8s = 0x0B,    /*!< System reset called every 8s */
    IWDG_TIMEOUT_16s = 0x0C,   /*!< System reset called every 16s */
    IWDG_TIMEOUT_32s = 0x0D    /*!< System reset called every 32s. This is maximum value allowed with IWDG timer */
} iwdg_timeout_t;

/**
 * @brief  Initialize Independent Watchdog timer
 * @note   After you initialize it, you can't disable it unless reset occur.
 * @param  timeout. Timeout value when watchdog reset happen if not reset by user.
 *            This parameter can be a value of @ref iwdg_timeout_t enumeration
 * @retval Value if system was reset because of watchdog timer
 *            - 1: Reset happen because of watchdog
 *            - 0: Otherwise
 */
uint8_t iwdg_init(iwdg_timeout_t timeout);

static inline void iwdg_reset(void)
{
    IWDG_ReloadCounter();
}

/* =============================================================================== */

uint8_t wwdg_init(void);

static inline void wwdg_reset(void)
{
    WWDG_SetCounter(127);
}

static inline void wwdg_deinit(void)
{
    WWDG_DeInit();
}

#endif
