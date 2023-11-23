#ifndef __APP_UTILS_H
#define __APP_UTILS_H

#include "sys.h"  

#define RATE_1_HZ		  1
#define RATE_5_HZ		  5
#define RATE_10_HZ		10
#define RATE_20_HZ		20
#define RATE_25_HZ		25
#define RATE_50_HZ		50
#define RATE_100_HZ		100
#define RATE_200_HZ 	200
#define RATE_250_HZ 	250
#define RATE_500_HZ 	500
#define RATE_1000_HZ 	1000


#define M2T(X) ((unsigned int)((X)*(configTICK_RATE_HZ/1000.0)))
#define F2T(X) ((unsigned int)((configTICK_RATE_HZ/(X))))
 
uint32_t getSysTickCnt(void);

uint32_t delay_init(void);
void delay_ms(volatile uint32_t milliseconds);
void delay_us(volatile uint32_t microseconds);

void printf_init(void);
void app_printf(const char *format, ...);

#endif
