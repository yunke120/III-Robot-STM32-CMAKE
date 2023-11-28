/****************************************Copyright (c)****************************************************

**--------------File Info---------------------------------------------------------------------------------
** File name:
** Last modified Date:
** Last Version:
** Descriptions:
**
**--------------------------------------------------------------------------------------------------------
** Created by:               yunke120
** Created date:           2023/04/09
** Version:                  1.0
** Descriptions:
**--------------------------------------------------------------------------------------------------------*/
#include "task_robot.h"	

#include <stdio.h>

#include "main.h"
#include "tim.h"
#include "bsp_motor.h"
#include "bsp_ina226.h"
#include "robot.h"
#include "utils.h"

uint32_t g_RobotVoltage = 0;
extern osMutexId_t robotVoltageMutex;

osThreadId_t robot_handle;
const osThreadAttr_t robot_attr = {
	.name = "robot",
	.stack_size = 256 * 4,
	.priority = (osPriority_t)osPriorityLow1,
};

static void robot_entry(void *param)
{	
	uint8_t ret;
	float f_vol;

	ina226_init();
	while (1)
	{
#if 0
		UBaseType_t stackHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        app_printf("robot thread: %u words\r\n", stackHighWaterMark);	// 获取线程所剩空间
#endif
		ina226_reset();
		ina226_send_request("%s\r\n", INA226_GET_VOLTAGE);	/* 获取输入电压值 */
		osDelay(100);
		ret = ina226_get_response(&f_vol);
		if(ret != 0)
		{
			app_printf("Failed to get voltage!\r\n");
			continue;
		}

		osMutexAcquire(robotVoltageMutex, portMAX_DELAY);
		g_RobotVoltage = f_vol;
		osMutexRelease(robotVoltageMutex);
		// app_printf("%.3f\r\n", f_vol);
		osDelay(500);
	}
}

void create_robot_thread(void)
{
	robot_handle = osThreadNew(robot_entry, NULL, &robot_attr);

	if (robot_handle == NULL)
		printf("<create> [task]	: robot		> 0\r\n");
	else
		printf("<create> [task]	: robot		> 1\r\n");
}


