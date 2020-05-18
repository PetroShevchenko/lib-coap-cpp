#ifndef RTC_H
#define RTC_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

bool rtc_init(void);
uint32_t rtc_get_time (void);
bool rtc_set_time (uint32_t time);

int stime (const time_t *when);

#ifdef __cplusplus
}
#endif

#endif