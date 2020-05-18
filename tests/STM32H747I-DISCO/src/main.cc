#include <cstdbool>
#include <cstring>
#include <ctime>
#include <sys/time.h>

#include "main.h"
#include "coapcpp.h"
#include "cmsis_os.h"
#include "drivers.h"

LOG_USING_NAMESPACE
LOG_GLOBAL_DECLARE

osThreadId_t defaultTaskHandle;

osThreadAttr_t defaultTask_attributes = {"defaultTask", 0};

void StartDefaultTask(void *argument);

int main ()
{
    if (!drivers_init()) {
      Error_Handler();
    }

    LOG_CREATE(ALL, std::cout);
    LOG(DEBUGGING, "The device has been inited!");

  	osKernelInitialize();

    defaultTask_attributes.priority = (osPriority_t) osPriorityNormal;
    defaultTask_attributes.stack_size = 4096;


  	defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  	osKernelStart();

  	while(1)
    {
    }

    LOG_DELETE;
    return 0;
}

/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
void StartDefaultTask(void *argument)
{
  (void)argument;
  for(;;)
  {
    HAL_GPIO_WritePin(GPIOI, LED1_PIN|LED2_PIN, GPIO_PIN_SET);
    LOG(DEBUGGING, "StartDefaultTask");
    rtc_get_time();
    osDelay(1000);
    HAL_GPIO_WritePin(GPIOI, LED1_PIN|LED2_PIN, GPIO_PIN_RESET);
     osDelay(1000);
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  HAL_GPIO_WritePin(GPIOI, LED1_PIN|LED2_PIN|LED3_PIN|LED4_PIN, GPIO_PIN_SET);
  while(1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  LOG(logging::ERROR,"ASSERT: file: ", file, " line: ", line);
  Error_Handler();
}
#endif /* USE_FULL_ASSERT */
