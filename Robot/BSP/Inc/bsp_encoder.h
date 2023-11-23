// Header:		__BSP_ENCODER_H
// File Name: 	bsp_encoder.h
// Author:		yunke120
// Date:		2023/04/09

#ifndef __BSP_ENCODER_H
#define __BSP_ENCODER_H

#include "main.h"

#define Encoder_LT_TIM htim4
#define Encoder_RT_TIM htim2
#define Encoder_LB_TIM htim5
#define Encoder_RB_TIM htim3

#define EncoderMultiples 4	/* 编码器倍频数，取决于编码器初始化设置 */
#define ENCODER_ACCURACY 13 /* 霍尔编码器13 */
#define GEARRATIO 27		/* 电机减速比 */

#define WHEEL_DIAMETER 0.127f /* 麦轮轮胎直径 */
#ifndef PI
#define PI 3.14159265358979f
#endif

#define CONTROL_FREQUENCY 100											/* 编码器数据读取频率 */
#define WHEEL_PERIMETER WHEEL_DIAMETER *PI /* 主动轮周长 */				// 0.4m
#define ENCODER_PRECISION EncoderMultiples *ENCODER_ACCURACY *GEARRATIO /* 电机(车轮)转1圈对应的编码器数值 1404 */

#define CALC_VELO 0.02846f /* CONTROL_FREQUENCY * WHEEL_PERIMETER / ENCODER_PRECISION */

#define VelocityMin 600
#define VelocityMax 1000

typedef enum
{
	eEncoderEnable,
	eEncoderDisable,
} eEncoderState;

typedef enum
{
	Encoder_LT,
	Encoder_RT,
	Encoder_LB,
	Encoder_RB,
} eEncoder;

typedef enum
{
	eEncoderForeward,
	eEncoderBackward,
	//	eEncoderStop,
} eEncoderDir;

void encoder_set_enable(eEncoder encoder, eEncoderState state);
short encoder_get_counter(eEncoder encoder);
float encoder_get_velocity(eEncoder encoder, short counter);
eEncoderDir encoder_get_dir(eEncoder encoder);

#endif /* __BSP_ENCODER_H */
