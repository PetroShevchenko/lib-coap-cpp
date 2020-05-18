#include <stdbool.h>
#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "main.h"

RTC_HandleTypeDef hrtc;

bool rtc_init(void)
{
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    return false;
  }

  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    return false;
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    return false;
  }
  return true;
}


uint32_t rtc_get_time ()
{
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
    struct tm tmp_time;

    memset ( &tmp_time, 0 , sizeof (tmp_time) );

    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);

    tmp_time.tm_year = (sDate.Year + 2000 - 1900 );
    tmp_time.tm_mon = (sDate.Month - 1 );
    tmp_time.tm_mday = sDate.Date;
    tmp_time.tm_hour = sTime.Hours;
    tmp_time.tm_min = sTime.Minutes;
    tmp_time.tm_sec = sTime.Seconds;

    if (sDate.WeekDay == 7 ) {
    	tmp_time.tm_wday = 0;
    }
    else {
        tmp_time.tm_wday = sDate.WeekDay;
    }
#if 0
    printf("\n\rRTC_get_time");
    printf("\n\r--------------------");
    printf("\n\rtm_year=%d", tmp_time.tm_year);
    printf("\n\rtm_mon=%d",  tmp_time.tm_mon);
    printf("\n\rtm_mday=%d",  tmp_time.tm_mday);
    printf("\n\rtm_hour=%d",  tmp_time.tm_hour);
    printf("\n\rtm_min=%d",  tmp_time.tm_min);
    printf("\n\rtm_sec=%d",  tmp_time.tm_sec);
    printf("\n\r--------------------");
#endif

    return mktime (&tmp_time);
}

bool rtc_set_time (uint32_t time)
{
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
    struct tm ts;

//    ts = *gmtime ( (time_t *) &time );
    ts = *localtime ( (time_t *) &time );
#if 0
    printf("\nRTC_set_time");
    printf("\n--------------------");
    printf("\ntm_year=%d", ts.tm_year);
    printf("\ntm_mon=%d",  ts.tm_mon);
    printf("\ntm_mday=%d",  ts.tm_mday);
    printf("\ntm_hour=%d",  ts.tm_hour);
    printf("\ntm_min=%d",  ts.tm_min);
    printf("\ntm_sec=%d",  ts.tm_sec);
    printf("\n--------------------");
#endif
    sTime.Hours = ts.tm_hour;
    sTime.Minutes = ts.tm_min;
    sTime.Seconds = ts.tm_sec;
    sTime.SubSeconds = 0x00;
    sTime.TimeFormat     = RTC_HOURFORMAT_24;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;

    if(HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
    {
        return false;
    }

    sDate.Year = ( ts.tm_year - 100 );
    sDate.Month = ( ts.tm_mon + 1 );
    sDate.Date = ts.tm_mday;

    if ( ts.tm_wday == 0 ) {
        sDate.WeekDay = 7;
    }
    else {
        sDate.WeekDay = ts.tm_wday;
    }

    if(HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
    {
        return false;
    }
    return true;
}
