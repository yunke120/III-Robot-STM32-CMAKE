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
#include "robot.h"
#include "utils.h"

uint32_t g_RobotVoltage;
osMutexId_t robotVoltageMutex;
const osMutexAttr_t robotVoltageMutex_attributes = {
	.name = "robotVoltageMutex"};

osThreadId_t robot_handle;
const osThreadAttr_t robot_attr = {
	.name = "robot",
	.stack_size = 256 * 4,
	.priority = (osPriority_t)osPriorityLow1,
};

static void robot_entry(void *param)
{	
	g_RobotVoltage = 0;
	while (1)
	{
		HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin); // normal running
		// UBaseType_t stackHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        // app_printf("robot thread: %u words\r\n", stackHighWaterMark);
		osMutexAcquire(robotVoltageMutex, portMAX_DELAY);
		g_RobotVoltage += 2.0;
		osMutexRelease(robotVoltageMutex);
		osDelay(500);
	}
}

void create_robot_thread(void)
{
	robotVoltageMutex = osMutexNew(&robotVoltageMutex_attributes);
	if(robotVoltageMutex == NULL)
	{
		printf("Failed to create robotVoltageMutex!\r\n");
		return;
	}

	robot_handle = osThreadNew(robot_entry, NULL, &robot_attr);

	if (robot_handle == NULL)
		printf("<create> [task]	: robot		> 0\r\n");
	else
		printf("<create> [task]	: robot		> 1\r\n");
}


