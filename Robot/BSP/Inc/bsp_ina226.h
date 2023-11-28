// Header:
// File Name: 
// Author:	yunke120
// Date:	2023/04/15

#ifndef __BSP_INA226_H
#define	__BSP_INA226_H

#include "main.h"

#define INA226_USART_IRQHandler		USART2_IRQHandler

#define INA226_UART_RX_BUF_SIZE		128
#define INA226_UART_TX_BUF_SIZE		128


#define INA226_AT_TEST					"AT"            /* AT测试 */
#define INA226_RESET					"AT+RESET"      /* 复位 */
#define INA226_GET_VOLTAGE			    "AT+V"          /* 获取电压值 */
#define INA226_GET_CURRENT			    "AT+C"          /* 获取电流值 */
#define INA226_GET_POWER				"AT+P"          /* 获取功率值 */

void    ina226_init(void);  
void    ina226_reset(void);      
void    ina226_send_request(char *fmt, ...);       
uint8_t	ina226_get_finish_flag(void);
uint8_t ina226_get_response(float * data);


#endif /* __BSP_INA226_H */


