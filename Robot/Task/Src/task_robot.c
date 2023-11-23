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

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <uxr/client/transport.h>
#include <rmw_microxrcedds_c/config.h>
#include <rmw_microros/rmw_microros.h>
#include <std_msgs/msg/int32.h>

#include "main.h"
#include "tim.h"
#include "bsp_motor.h"
#include "robot.h"
#include "utils.h"

extern rcl_publisher_t publisher;
extern std_msgs__msg__Int32 msg;
extern osMutexId_t robotVelocityMutexHandle;

osThreadId_t robot_handle;
const osThreadAttr_t robot_attr = {
	.name = "robot",
	.stack_size = 1000,
	.priority = (osPriority_t)osPriorityLow1,
};


static void robot_entry(void *param)
{

	while (1)
	{
		rcl_ret_t ret = rcl_publish(&publisher, &msg, NULL);
		if (ret != RCL_RET_OK)
		{
			app_printf("Error publishing (line %d)\r\n", __LINE__);
		}
		else
		{
			HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin); // normal running
		}
		msg.data ++;

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


