/****************************************Copyright (c)****************************************************

**--------------File Info---------------------------------------------------------------------------------
** File name:
** Last modified Date:
** Last Version:
** Descriptions:
**
**--------------------------------------------------------------------------------------------------------
** Created by:               yunke120
** Created date:           2023/04/08
** Version:                  1.0
** Descriptions:
**--------------------------------------------------------------------------------------------------------*/

#include "robot.h"

/* 定义小车运动方向，默认停止 */
static eRobotDir g_robot_Dir = Robot_Stop;


/**
 * 机器人使能
*/
void robot_enable(void)
{
	motor_set_enable(Wheel_LT, eWheelEnable);
	motor_set_enable(Wheel_RT, eWheelEnable);
	motor_set_enable(Wheel_LB, eWheelEnable);
	motor_set_enable(Wheel_RB, eWheelEnable);
}

/**
 * 机器人失能
*/
void robot_disable(void)
{
	motor_set_enable(Wheel_LT, eWheelDisable);
	motor_set_enable(Wheel_RT, eWheelDisable);
	motor_set_enable(Wheel_LB, eWheelDisable);
	motor_set_enable(Wheel_RB, eWheelDisable);
}



/**
 * @brief  设置小车方向
 *
 * @param dir 小车方向
 */
void robot_set_dir(eRobotDir dir)
{
	eRobotDir r = dir;
	if (g_robot_Dir == r)
		return;
	switch (r)
	{
	case Robot_Stop: /* 停止 */
		motor_set_dir(Wheel_LT, Wheel_Stop);
		motor_set_dir(Wheel_RT, Wheel_Stop);
		motor_set_dir(Wheel_LB, Wheel_Stop);
		motor_set_dir(Wheel_RB, Wheel_Stop);
		break;
	case Robot_Foreward: /* 四个轮子同时同速前进 */
		motor_set_dir(Wheel_LT, Wheel_Foreward);
		motor_set_dir(Wheel_RT, Wheel_Foreward);
		motor_set_dir(Wheel_LB, Wheel_Foreward);
		motor_set_dir(Wheel_RB, Wheel_Foreward);
		break;
	case Robot_Backward: /* 四个轮子同时同速后退 */
		motor_set_dir(Wheel_LT, Wheel_Backward);
		motor_set_dir(Wheel_RT, Wheel_Backward);
		motor_set_dir(Wheel_LB, Wheel_Backward);
		motor_set_dir(Wheel_RB, Wheel_Backward);
		break;
	case Robot_Right_Up:
		motor_set_dir(Wheel_LT, Wheel_Stop);
		motor_set_dir(Wheel_RT, Wheel_Foreward);
		motor_set_dir(Wheel_LB, Wheel_Foreward);
		motor_set_dir(Wheel_RB, Wheel_Stop);
		break;
	case Robot_Left_Up:
		motor_set_dir(Wheel_LT, Wheel_Foreward);
		motor_set_dir(Wheel_RT, Wheel_Stop);
		motor_set_dir(Wheel_LB, Wheel_Stop);
		motor_set_dir(Wheel_RB, Wheel_Foreward);
		break;
	case Robot_Right_Down:
		motor_set_dir(Wheel_LT, Wheel_Backward);
		motor_set_dir(Wheel_RT, Wheel_Stop);
		motor_set_dir(Wheel_LB, Wheel_Stop);
		motor_set_dir(Wheel_RB, Wheel_Backward);
		break;
	case Robot_Left_Down:
		motor_set_dir(Wheel_LT, Wheel_Stop);
		motor_set_dir(Wheel_RT, Wheel_Backward);
		motor_set_dir(Wheel_LB, Wheel_Backward);
		motor_set_dir(Wheel_RB, Wheel_Stop);
		break;
	case Robot_Shift_Right:
		motor_set_dir(Wheel_LT, Wheel_Foreward);
		motor_set_dir(Wheel_RT, Wheel_Backward);
		motor_set_dir(Wheel_LB, Wheel_Backward);
		motor_set_dir(Wheel_RB, Wheel_Foreward);
		break;
	case Robot_Shift_Left:
		motor_set_dir(Wheel_LT, Wheel_Backward);
		motor_set_dir(Wheel_RT, Wheel_Foreward);
		motor_set_dir(Wheel_LB, Wheel_Foreward);
		motor_set_dir(Wheel_RB, Wheel_Backward);
		break;
	case Robot_Rotate_Right:
		motor_set_dir(Wheel_LT, Wheel_Foreward);
		motor_set_dir(Wheel_RT, Wheel_Backward);
		motor_set_dir(Wheel_LB, Wheel_Foreward);
		motor_set_dir(Wheel_RB, Wheel_Backward);
		break;
	case Robot_Rotate_Left:
		motor_set_dir(Wheel_LT, Wheel_Backward);
		motor_set_dir(Wheel_RT, Wheel_Foreward);
		motor_set_dir(Wheel_LB, Wheel_Backward);
		motor_set_dir(Wheel_RB, Wheel_Foreward);
		break;
	}
	g_robot_Dir = r;
}

/**
 * @brief 获取小车当前运动方向
 *
 * @return erobotDir 小车方向
 */
eRobotDir robot_get_dir(void)
{
	return g_robot_Dir;
}
