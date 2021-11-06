/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f1xx_hal.h"

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
#define RED_Pin GPIO_PIN_3
#define RED_GPIO_Port GPIOA
#define YELLOW_Pin GPIO_PIN_4
#define YELLOW_GPIO_Port GPIOA
#define GREEN_Pin GPIO_PIN_5
#define GREEN_GPIO_Port GPIOA
#define LED1_A_Pin GPIO_PIN_0
#define LED1_A_GPIO_Port GPIOB
#define LED1_B_Pin GPIO_PIN_1
#define LED1_B_GPIO_Port GPIOB
#define LED1_C_Pin GPIO_PIN_2
#define LED1_C_GPIO_Port GPIOB
#define LED2_D_Pin GPIO_PIN_10
#define LED2_D_GPIO_Port GPIOB
#define LED2_E_Pin GPIO_PIN_11
#define LED2_E_GPIO_Port GPIOB
#define LED2_F_Pin GPIO_PIN_12
#define LED2_F_GPIO_Port GPIOB
#define LED2_G_Pin GPIO_PIN_13
#define LED2_G_GPIO_Port GPIOB
#define LED1_D_Pin GPIO_PIN_3
#define LED1_D_GPIO_Port GPIOB
#define LED1_E_Pin GPIO_PIN_4
#define LED1_E_GPIO_Port GPIOB
#define LED1_F_Pin GPIO_PIN_5
#define LED1_F_GPIO_Port GPIOB
#define LED1_G_Pin GPIO_PIN_6
#define LED1_G_GPIO_Port GPIOB
#define LED2_A_Pin GPIO_PIN_7
#define LED2_A_GPIO_Port GPIOB
#define LED2_B_Pin GPIO_PIN_8
#define LED2_B_GPIO_Port GPIOB
#define LED2_C_Pin GPIO_PIN_9
#define LED2_C_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

// Comment this line out if error reporting is NOT required
//#define SCH_REPORT_ERRORS
//Where error reporting is required ,the port on which error codes will be displayed
// is also determined via main.h:
#ifdef SCH_REPORT_ERRORS
// The port on which error codes will be displayed
// ONLY USED IF ERRORS ARE REPORTED
#define Error_port PORTA
#endif

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
