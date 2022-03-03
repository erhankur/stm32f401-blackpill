#include "drv/wdts.h"

uint8_t iwdg_init(iwdg_timeout_t timeout)
{
    uint8_t reset = 0;
    uint16_t reload = 0;

    /* Check if the system has resumed from IWDG reset */
    if (RCC->CSR & RCC_CSR_WDGRSTF) {
        /* Reset by IWDG */
        reset = 1;

        /* Clear reset flags */
        RCC_ClearFlag();
    }

    /* Enable write access to IWDG_PR and IWDG_RLR registers */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    /* Set proper clock depending on timeout user select */
    if (timeout >= IWDG_TIMEOUT_8s) {
        /* IWDG counter clock: LSI/256 = 128Hz */
        IWDG_SetPrescaler(IWDG_Prescaler_256);
    } else {
        /* IWDG counter clock: LSI/32 = 1024Hz */
        IWDG_SetPrescaler(IWDG_Prescaler_32);
    }

    /* Set counter reload value */
    if (timeout == IWDG_TIMEOUT_5ms) {
        reload = 5; /* 1024 Hz IWDG ticking */
    } else if (timeout == IWDG_TIMEOUT_10ms) {
        reload = 10; /* 1024 Hz IWDG ticking */
    } else if (timeout == IWDG_TIMEOUT_15ms) {
        reload = 15; /* 1024 Hz IWDG ticking */
    } else if (timeout == IWDG_TIMEOUT_30ms) {
        reload = 31; /* 1024 Hz IWDG ticking */
    } else if (timeout == IWDG_TIMEOUT_60ms) {
        reload = 61; /* 1024 Hz IWDG ticking */
    } else if (timeout == IWDG_TIMEOUT_120ms) {
        reload = 123; /* 1024 Hz IWDG ticking */
    } else if (timeout == IWDG_TIMEOUT_250ms) {
        reload = 255; /* 1024 Hz IWDG ticking */
    } else if (timeout == IWDG_TIMEOUT_500ms) {
        reload = 511; /* 1024 Hz IWDG ticking */
    } else if (timeout == IWDG_TIMEOUT_1s) {
        reload = 1023; /* 1024 Hz IWDG ticking */
    } else if (timeout == IWDG_TIMEOUT_2s) {
        reload = 2047; /* 1024 Hz IWDG ticking */
    } else if (timeout == IWDG_TIMEOUT_4s) {
        reload = 4095; /* 1024 Hz IWDG ticking */
    } else if (timeout == IWDG_TIMEOUT_8s) {
        reload = 1023; /* 128 Hz IWDG ticking */
    } else if (timeout == IWDG_TIMEOUT_16s) {
        reload = 2047; /* 128 Hz IWDG ticking */
    } else if (timeout == IWDG_TIMEOUT_32s) {
        reload = 4095; /* 128 Hz IWDG ticking */
    }

    /* Set reload */
    IWDG_SetReload(reload);

    /* Reload IWDG counter */
    IWDG_ReloadCounter();

    /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
    IWDG_Enable();

    /* Return status */
    return reset;
}

uint8_t wwdg_init(void)
{
    uint8_t reset = 0;

    /* Check if the system has resumed from WWDG reset */
    if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET) {
        /* WWDGRST flag set */
        reset = 1;

        /* Clear reset flags */
        RCC_ClearFlag();
    }

    /* Enable WWDG clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

    /* WWDG clock counter = (PCLK1 (16MHz)/4096)/2 = 1953.125Hz (~512us)  */
    WWDG_SetPrescaler(WWDG_Prescaler_2);

    /* Set Window value to 110; WWDG counter should be refreshed only when the counter
        is below 110 (and greater than 64) otherwise a reset will be generated.
        This value must be sync with portTICK_PERIOD_MS
    */
    WWDG_SetWindowValue(110);

    /* Enable WWDG and set counter value to 127, WWDG timeout = ~512 us * 64 = 32 ms
        In this case the refresh window is: ~512 * (127-110) = 10ms < refresh window < ~512 * 64 = 32ms
        ~8.7 ms < portTICK_PERIOD_MS < 32ms
    */
    WWDG_Enable(127);

    return reset;
}
