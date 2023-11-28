/****************************************Copyright (c)****************************************************
 
**--------------File Info---------------------------------------------------------------------------------
** File name:                 
** Last modified Date:          
** Last Version:           
** Descriptions:           
**                        
**--------------------------------------------------------------------------------------------------------
** Created by:               yunke120
** Created date:           2023/04/15
** Version:                  1.0
** Descriptions:         
** 

	GND			GND(黑色)
	PD5(TX)		RX(棕色)
	PD6(RX)		TX(白色)
	VCC			VCC(红色)
	
**--------------------------------------------------------------------------------------------------------*/


#include "bsp_ina226.h"
#include "usart.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



// extern UART_HandleTypeDef huart2;
static uint8_t rx_temp;

static struct 
{
    uint8_t buf[INA226_UART_RX_BUF_SIZE];              		/* 帧接收缓冲 */
    struct
    {
        uint16_t len    : 15;                               /* 帧接收长度，sta[14:0] */
        uint16_t finsh  : 1;                                /* 帧接收完成标志，sta[15] */
    } sta;                                                  /* 帧状态信息 */
} g_uart_rx_frame = {0};                                    /* INA226 UART接收帧缓冲信息结构体 */

static uint8_t g_uart_tx_buf[INA226_UART_TX_BUF_SIZE]; 		/* INA226 UART发送缓冲 */

/**
 * @brief 电源管理INA226模块初始化
 * 
 */
void ina226_init(void)
{
	ina226_reset();
	
	__HAL_UART_CLEAR_FLAG(&huart2,UART_FLAG_IDLE);
	__HAL_UART_CLEAR_FLAG(&huart2,UART_FLAG_RXNE);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);                         
	__HAL_UART_DISABLE_IT(&huart2,UART_IT_IDLE); 
	HAL_UART_Receive_IT(&huart2, &rx_temp, 1);				/* 开启接收中断 */
	
}

/**
 * @brief 串口2打印
 * 
 * @param fmt 
 * @param ... 
 */
void ina226_send_request(char *fmt, ...)
{
    va_list ap;
    uint16_t len;
    
    va_start(ap, fmt);
    vsprintf((char *)g_uart_tx_buf, fmt, ap);
    va_end(ap);
	
    len = strlen((const char *)g_uart_tx_buf);
    HAL_UART_Transmit(&huart2, g_uart_tx_buf, len, HAL_MAX_DELAY);
}

/**
 * @brief 接收复位，每一次发送前需调用
 * 
 */
void ina226_reset(void)
{
	memset(g_uart_rx_frame.buf, 0, g_uart_rx_frame.sta.len);
    g_uart_rx_frame.sta.len     = 0;
    g_uart_rx_frame.sta.finsh   = 0;
}

/**
 * @brief 串口2中断函数
 * 
 */
void INA226_USART_IRQHandler(void)
{
	if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_RXNE)!= RESET) { // 接收中断：接收到数据
		
		uint8_t data;
		data=READ_REG(huart2.Instance->DR); 
		if (g_uart_rx_frame.sta.len == 0)
		{ 
			ina226_reset();
			__HAL_UART_CLEAR_FLAG(&huart2,UART_FLAG_IDLE); 
			__HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE); 
			HAL_UART_Receive_IT(&huart2, &rx_temp, 1);
		}
		if (g_uart_rx_frame.sta.len < INA226_UART_RX_BUF_SIZE) 
		{ 
			g_uart_rx_frame.buf[g_uart_rx_frame.sta.len]=data; 
			g_uart_rx_frame.sta.len++; 
		}
	} 
	else if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE)!= RESET) { // 串口空闲中断
			__HAL_UART_CLEAR_FLAG(&huart2,UART_FLAG_IDLE);
			__HAL_UART_DISABLE_IT(&huart2,UART_IT_IDLE); 
			g_uart_rx_frame.buf[g_uart_rx_frame.sta.len] = '\0';
			g_uart_rx_frame.sta.finsh = 1; 
			// printf("RX: %s\r\n", g_uart_rx_frame.buf);
	}
	HAL_UART_IRQHandler(&huart2);
}

/**
 * @brief 获取接收完成标志位
 * 
 * @return uint8_t 
 */
uint8_t	ina226_get_finish_flag(void)
{
	return g_uart_rx_frame.sta.finsh;
}


/**
 * @brief 解析接收数据
 * 
 * @param data 解析值
 * @return uint8_t 0：解析成功；1：解析失败
 */
uint8_t ina226_get_response(float * data)
{
	if(g_uart_rx_frame.sta.finsh == 0)	return 1;	
	if(strstr((const char *)g_uart_rx_frame.buf, "OK") == NULL)	return 2;
	char *p = strchr((char *)g_uart_rx_frame.buf, '=');
	if(p == NULL) return 3;
	*data = atof(p+1);
	return 0;
}








