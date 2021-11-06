/*
 * task.c
 *
 *  Created on: Nov 6, 2021
 *      Author: MSI-
 */
#include "main.h"
#include "task.h"

#define NUMBER_OF_DECIMAL_DIGITS	10

static uint8_t sevenSegmentLEDConversion[NUMBER_OF_DECIMAL_DIGITS] = {0x01, 0x4f, 0x12, 0x06, 0x4C, 0x24, 0x20, 0x0f, 0x00, 0x04};

static int num_1 = 0;
static int num_2 = 0;

void led_red(void) {
	HAL_GPIO_TogglePin(RED_GPIO_Port, RED_Pin);
}

void led_yellow(void) {
	HAL_GPIO_TogglePin(YELLOW_GPIO_Port, YELLOW_Pin);
}

void led_green(void) {
	HAL_GPIO_TogglePin(GREEN_GPIO_Port, GREEN_Pin);
}

void displayled1(void) {
	uint8_t the_number = sevenSegmentLEDConversion[num_1];
	HAL_GPIO_WritePin(LED1_G_GPIO_Port, LED1_G_Pin, the_number & 0x01);
	HAL_GPIO_WritePin(LED1_F_GPIO_Port, LED1_F_Pin, the_number & 0x02);
	HAL_GPIO_WritePin(LED1_E_GPIO_Port, LED1_E_Pin, the_number & 0x04);
	HAL_GPIO_WritePin(LED1_D_GPIO_Port, LED1_D_Pin, the_number & 0x08);
	HAL_GPIO_WritePin(LED1_C_GPIO_Port, LED1_C_Pin, the_number & 0x10);
	HAL_GPIO_WritePin(LED1_B_GPIO_Port, LED1_B_Pin, the_number & 0x20);
	HAL_GPIO_WritePin(LED1_A_GPIO_Port, LED1_A_Pin, the_number & 0x40);
	if(++num_1 == 10)
		num_1 = 0;
}

void displayled2(void) {
	uint8_t the_number = sevenSegmentLEDConversion[num_2];
	HAL_GPIO_WritePin(LED2_G_GPIO_Port, LED2_G_Pin, the_number & 0x01);
	HAL_GPIO_WritePin(LED2_F_GPIO_Port, LED2_F_Pin, the_number & 0x02);
	HAL_GPIO_WritePin(LED2_E_GPIO_Port, LED2_E_Pin, the_number & 0x04);
	HAL_GPIO_WritePin(LED2_D_GPIO_Port, LED2_D_Pin, the_number & 0x08);
	HAL_GPIO_WritePin(LED2_C_GPIO_Port, LED2_C_Pin, the_number & 0x10);
	HAL_GPIO_WritePin(LED2_B_GPIO_Port, LED2_B_Pin, the_number & 0x20);
	HAL_GPIO_WritePin(LED2_A_GPIO_Port, LED2_A_Pin, the_number & 0x40);
	if(++num_2 == 10)
		num_2 = 0;
}
