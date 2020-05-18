#ifndef DRIVERS_H
#define DRIVERS_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "stm32h7xx_hal.h"
#include "stm32h747i_discovery.h"
#include "rtc.h"

extern UART_HandleTypeDef huart1;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
bool SystemClock_Config(void);
bool drivers_init(void);

#ifdef __cplusplus
}
#endif

#endif