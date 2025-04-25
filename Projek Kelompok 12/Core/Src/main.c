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
#include <stdlib.h>
#include <stdio.h>

/* Hanya helper baca satu char UART */
static char recv_char(void)
{
    char c;
    while (HAL_UART_Receive(&huart2, (uint8_t*)&c, 1, HAL_MAX_DELAY) != HAL_OK) { }
    return c;
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_USART2_UART_Init();

    for (;;)
    {
        /* Tampilkan menu */
        const char menu[] =
            "\r\n=== MENU DEBUG ===\r\n"
            "1) Input Number Test\r\n"
            "2) Read Temperature\r\n"
            "3) Init MLX90614\r\n"
            "4) Run All\r\n"
            "Select [1-4]: ";
        HAL_UART_Transmit(&huart2, (uint8_t*)menu, strlen(menu), HAL_MAX_DELAY);

        char choice = recv_char();
        HAL_UART_Transmit(&huart2, (uint8_t*)&choice, 1, HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, HAL_MAX_DELAY);

        switch (choice)
        {
            case '1':
            {
                /* --- Inline check_number() --- */
                char buf[16];
                uint8_t idx = 0;
                char c;
                const char prompt[] = "Enter number: ";
                HAL_UART_Transmit(&huart2, (uint8_t*)prompt, strlen(prompt), HAL_MAX_DELAY);

                while (1)
                {
                    HAL_UART_Receive(&huart2, (uint8_t*)&c, 1, HAL_MAX_DELAY);
                    if (c=='\r'||c=='\n'||idx>=sizeof(buf)-1) break;
                    if (c>='0'&&c<='9')
                    {
                        buf[idx++]=c;
                        HAL_UART_Transmit(&huart2,(uint8_t*)&c,1,HAL_MAX_DELAY);
                    }
                }
                buf[idx]='\0';
                uint8_t result_add = (uint8_t)atoi(buf);
                char msg1[32];
                snprintf(msg1,sizeof(msg1),"\r\nYou entered: %u\r\n", result_add);
                HAL_UART_Transmit(&huart2,(uint8_t*)msg1,strlen(msg1),HAL_MAX_DELAY);
            }
            break;

            case '2':
            {
                /* --- Inline measurement_result() --- */
                float temp;
                if (MLX90614_ReadTemp(&hi2c1, MLX90614_CMD_OBJ, &temp)==HAL_OK)
                {
                    char msg2[32];
                    snprintf(msg2,sizeof(msg2),"Temp: %.2f C\r\n", temp);
                    HAL_UART_Transmit(&huart2,(uint8_t*)msg2,strlen(msg2),HAL_MAX_DELAY);
                }
                else
                {
                    Error_Handler();
                }
            }
            break;

            case '3':
            {
                /* --- Inline mlx90614_init() --- */
                if (MLX90614_Init(&hi2c1)==HAL_OK)
                {
                    const char ok[]="MLX90614 OK\r\n";
                    HAL_UART_Transmit(&huart2,(uint8_t*)ok,sizeof(ok)-1,HAL_MAX_DELAY);
                }
                else
                {
                    const char err[]="MLX90614 ERROR\r\n";
                    HAL_UART_Transmit(&huart2,(uint8_t*)err,sizeof(err)-1,HAL_MAX_DELAY);
                    Error_Handler();
                }
            }
            break;

            case '4':
                /* Bisa panggil ulang setiap blok di atas, atau copy–paste inline */
                break;

            default:
                HAL_UART_Transmit(&huart2,(uint8_t*)"Invalid choice\r\n",15,HAL_MAX_DELAY);
        }
    }
}


/* --------------------------------------------------
   Fungsi: measurement_result
   Deskripsi: Baca suhu dari MLX90614 dan kirim via UART
   -------------------------------------------------- */
static void measurement_result(void)
{
    float temperature;

    if (MLX90614_ReadTemp(&hi2c1, MLX90614_CMD_OBJ, &temperature) == HAL_OK)
    {
        char msg[32];
        snprintf(msg, sizeof(msg), "Temp: %.2f C\r\n", temperature);
        HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    }
    else
    {
        Error_Handler();
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