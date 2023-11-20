/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MRB_EN2_Pin GPIO_PIN_0
#define MRB_EN2_GPIO_Port GPIOC
#define MRT_EN1_Pin GPIO_PIN_12
#define MRT_EN1_GPIO_Port GPIOB
#define MRT_EN2_Pin GPIO_PIN_13
#define MRT_EN2_GPIO_Port GPIOB
#define MRB_EN1_Pin GPIO_PIN_14
#define MRB_EN1_GPIO_Port GPIOB
#define MLB_EN1_Pin GPIO_PIN_10
#define MLB_EN1_GPIO_Port GPIOD
#define BEEP_Pin GPIO_PIN_11
#define BEEP_GPIO_Port GPIOD
#define MLB_EN2_Pin GPIO_PIN_12
#define MLB_EN2_GPIO_Port GPIOD
#define LED0_Pin GPIO_PIN_13
#define LED0_GPIO_Port GPIOD
#define LED1_Pin GPIO_PIN_14
#define LED1_GPIO_Port GPIOD
#define LED2_Pin GPIO_PIN_15
#define LED2_GPIO_Port GPIOD
#define MLT_EN2_Pin GPIO_PIN_8
#define MLT_EN2_GPIO_Port GPIOA
#define MLT_EN1_Pin GPIO_PIN_12
#define MLT_EN1_GPIO_Port GPIOC
#define OLED_DC_Pin GPIO_PIN_4
#define OLED_DC_GPIO_Port GPIOD
#define OLED_RST_Pin GPIO_PIN_5
#define OLED_RST_GPIO_Port GPIOD
#define OLED_SDA_Pin GPIO_PIN_6
#define OLED_SDA_GPIO_Port GPIOD
#define OLED_SCL_Pin GPIO_PIN_7
#define OLED_SCL_GPIO_Port GPIOD
#define IIC_SDA_Pin GPIO_PIN_8
#define IIC_SDA_GPIO_Port GPIOB
#define IIC_SCL_Pin GPIO_PIN_9
#define IIC_SCL_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
