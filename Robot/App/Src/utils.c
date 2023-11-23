#include "utils.h"

#include "usart.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#if SYSTEM_SUPPORT_OS
    #include "FreeRTOS.h"					//FreeRTOS使用		  
    #include "task.h"
    #include "cmsis_os.h"
#endif


static uint32_t sysTickCnt=0;

/********************************************************
	*getSysTickCnt()
	*调度开启之前 返回 sysTickCnt
	*调度开启之前 返回 xTaskGetTickCount()
*********************************************************/
uint32_t getSysTickCnt(void)
{
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)	/*系统已经运行*/
		return xTaskGetTickCount();
	else
		return sysTickCnt;
}		


uint32_t delay_init(void)
{
    /* 禁用 TRC */
    CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
    /* 启动 TRC */
    CoreDebug->DEMCR |=  CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;
    /* Disable clock cycle counter */
    DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
    /* 启动  时钟周期计数器 */
    DWT->CTRL |=  DWT_CTRL_CYCCNTENA_Msk; //0x00000001;
    /* 重置时钟周期计数器值 */
    DWT->CYCCNT = 0;
    /* 3 NO OPERATION instructions 3空 */
    __ASM volatile ("NOP");
    __ASM volatile ("NOP");
    __ASM volatile ("NOP");
    /* 检查时钟周期计数器是否已启动*/
    if(DWT->CYCCNT)
    {
       return 0; /*clock cycle counter started*/
    }
    else
    {
      return 1; /*clock cycle counter not started*/
    }
}

 
inline void delay_ms(volatile uint32_t milliseconds)
{
  uint32_t au32_initial_ticks = DWT->CYCCNT;
  uint32_t au32_ticks = (HAL_RCC_GetHCLKFreq() / 1000);
  milliseconds *= au32_ticks;
  while ((DWT->CYCCNT - au32_initial_ticks) < milliseconds);
}

inline void delay_us(volatile uint32_t microseconds)
{
  uint32_t clk_cycle_start = DWT->CYCCNT;
 
  /* Go to number of cycles for system */
  microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);
 
  /* Delay till end */
  while ((DWT->CYCCNT - clk_cycle_start) < microseconds);
}



#if SYSTEM_SUPPORT_OS
/* 创建互斥锁 */
osMutexId_t printMutexHandle;
const osMutexAttr_t printMutex_attributes = {
	.name = "printMutex"};

void printf_init(void)
{
    printMutexHandle = osMutexNew(&printMutex_attributes);
    if(printMutexHandle == NULL)
    {
        printf("Failed to create printMutexHandle.\r\n");
    }
}
#endif

/*
	线程安全的printf
*/
void app_printf(const char *fmt, ...) 
{
#if SYSTEM_SUPPORT_OS
    osMutexAcquire(printMutexHandle, portMAX_DELAY);
#endif
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
#if SYSTEM_SUPPORT_OS
    osMutexRelease(printMutexHandle);
#endif
}

// 重定向标准输出流的函数
int __io_putchar(int ch) {
    // 发送字符到USART3
    HAL_UART_Transmit(&huart3, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
    return ch;
}