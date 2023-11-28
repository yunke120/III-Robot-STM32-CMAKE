// Header:
// File Name: drv_motor.h
// Author:	yunke120
// Date:	2023/04/08

#ifndef __DRV_MOTOR_H
#define __DRV_MOTOR_H

#include "bsp_motor.h"

typedef enum
{
	Robot_Stop,			/* 停止 	*/
	Robot_Foreward,		/* 前进     */
	Robot_Backward,		/* 后退     */
	Robot_Right_Up,		/* 右上 	*/
	Robot_Left_Up,		/* 左上 	*/
	Robot_Right_Down,	/* 右下 	*/
	Robot_Left_Down,	/* 左下 	*/
	Robot_Shift_Right,	/* 右移 · 	*/
	Robot_Shift_Left,	/* 左移·  	*/
	Robot_Rotate_Right, /* 原地右旋转 */
	Robot_Rotate_Left	/* 原地左旋转 */
} eRobotDir;			/* 小车运动方向枚举 */

void robot_enable(void);
void robot_disable(void);
void robot_set_dir(eRobotDir dir); /* 设置小车方向 */
eRobotDir robot_get_dir(void);	   /* 获取小车当前运动方向 */

#endif /* __DRV_MOTOR_H */
