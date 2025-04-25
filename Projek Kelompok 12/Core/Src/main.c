/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
#include "main.h"
#include "gpio.h"
#include "i2c.h"
#include "usart.h"
#include "mlx90614.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* USER CODE END PV */

int main(void)
{
    HAL_StatusTypeDef ret;
    float temperature;
    char rxBuf[16];
    uint8_t idx;
    bool valid;
    char msg[32];

    /* Initialize HAL and system clock */
    HAL_Init();
    SystemClock_Config();

    /* Initialize peripherals */
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_USART2_UART_Init();

    /* Main loop */
    for (;;)
    {
        /* --- Number Check via UART --- */
        const char prompt[] = "Enter number: ";
        HAL_UART_Transmit(&huart2, (uint8_t*)prompt, strlen(prompt), HAL_MAX_DELAY);

        /* Receive until carriage return or buffer full */
        idx = 0U;
        do {
            HAL_UART_Receive(&huart2, (uint8_t*)&rxBuf[idx], 1U, HAL_MAX_DELAY);
        } while ((rxBuf[idx++] != '\r') && (idx < sizeof(rxBuf) - 1U));
        rxBuf[idx - 1U] = '\0';  /* Null-terminate */

        /* Validate digits */
        valid = (idx > 1U);
        for (uint8_t i = 0U; i < idx - 1U; i++) {
            if ((rxBuf[i] < '0') || (rxBuf[i] > '9')) {
                valid = false;
                break;
            }
        }
        if (valid) {
            snprintf(msg, sizeof(msg), "Valid number: %s\r\n", rxBuf);
        } else {
            snprintf(msg, sizeof(msg), "Invalid format\r\n");
        }
        HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

        /* --- MLX90614 Sensor Reading --- */
        ret = MLX90614_ReadTemp(&hi2c1, MLX90614_CMD_OBJ, &temperature);
        if (ret == HAL_OK) {
            snprintf(msg, sizeof(msg), "Temp: %.2f C\r\n", temperature);
            HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
        } else {
            Error_Handler();
        }

        HAL_Delay(1000U);
    }
}

/**
  * @brief System Clock Configuration
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Enable power control clock and configure voltage scaling */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Configure HSE oscillator and PLL */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8U;
    RCC_OscInitStruct.PLL.PLLN = 336U;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7U;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /* Select PLL as system clock source and configure bus prescalers */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 |
                                  RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        Error_Handler();
    }
}

/**
  * @brief This function handles error occurrences.
  */
void Error_Handler(void)
{
    /* User may add code to report the error state */
    __disable_irq();
    while (1) {
    }
}