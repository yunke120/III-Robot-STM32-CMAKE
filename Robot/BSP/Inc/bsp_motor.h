#ifndef __BSP_MOTOR_H
#define __BSP_MOTOR_H

#include "main.h"

/* 左前轮引脚定义 */
#define WHEEL_LT_EN1_PIN MLT_EN1_Pin
#define WHEEL_LT_EN1_GPIO_Port MLT_EN1_GPIO_Port
#define WHEEL_LT_EN2_PIN MLT_EN2_Pin
#define WHEEL_LT_EN2_GPIO_Port MLT_EN2_GPIO_Port
#define WHEEL_LT_PWM_CHANNEL TIM_CHANNEL_1
#define WHEEL_LT_PWM_TIM htim8

/* 右前轮引脚定义 */
#define WHEEL_RT_EN1_PIN MRT_EN1_Pin
#define WHEEL_RT_EN1_GPIO_Port MRT_EN1_GPIO_Port
#define WHEEL_RT_EN2_PIN MRT_EN2_Pin
#define WHEEL_RT_EN2_GPIO_Port MRT_EN2_GPIO_Port
#define WHEEL_RT_PWM_CHANNEL TIM_CHANNEL_4
#define WHEEL_RT_PWM_TIM htim8

/* 左后轮引脚定义 */
#define WHEEL_LB_EN1_PIN MLB_EN1_Pin
#define WHEEL_LB_EN1_GPIO_Port MLB_EN1_GPIO_Port
#define WHEEL_LB_EN2_PIN MLB_EN2_Pin
#define WHEEL_LB_EN2_GPIO_Port MLB_EN2_GPIO_Port
#define WHEEL_LB_PWM_CHANNEL TIM_CHANNEL_2
#define WHEEL_LB_PWM_TIM htim8

/* 右后轮引脚定义 */
#define WHEEL_RB_EN1_PIN MRB_EN1_Pin
#define WHEEL_RB_EN1_GPIO_Port MRB_EN1_GPIO_Port
#define WHEEL_RB_EN2_PIN MRB_EN2_Pin
#define WHEEL_RB_EN2_GPIO_Port MRB_EN2_GPIO_Port
#define WHEEL_RB_PWM_CHANNEL TIM_CHANNEL_3
#define WHEEL_RB_PWM_TIM htim8

typedef enum
{
	eWheelEnable,
	eWheelDisable
} eWheelState; /* 状态枚举 */

typedef enum
{
	Wheel_Stop,
	Wheel_Foreward,
	Wheel_Backward
} eWheelDir; /* 轮子方向枚举 */

typedef enum
{
	Wheel_LT, /* 左前轮 A1 */
	Wheel_RT, /* 右前轮 A2 */
	Wheel_LB, /* 左后轮 B1 */
	Wheel_RB  /* 右后轮 B2 */
} eWheel;	  /* 轮子枚举 */

void motor_set_enable(eWheel wheel, eWheelState state);	  /* 使能轮子 */
void motor_set_dir(eWheel wheel, eWheelDir dir);		  /* 设置轮子方向 */
void motor_set_velocity(eWheel wheel, uint32_t velocity); /* 设置轮子转动速度 */

#endif /* __BSP_MOTOR_H */