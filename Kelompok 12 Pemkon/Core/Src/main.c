/* Includes */
#include "main.h"
#include "i2c.h"
#include "gpio.h"
#include "usart.h"
#include "max30102.h"
#include "mlx90614.h"
#include <stdio.h>

/* Private variables */
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;

/* Private function prototypes */
void SystemClock_Config(void);

int main(void)
{
    max30102_init(&hi2c1);
    mlx90614_init(&hi2c2);  
	/* MCU Configuration */
    HAL_Init();
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_I2C2_Init();
    MX_USART2_UART_Init();

    /* Initialize sensors */


    /* Infinite loop */
    while (1)
    {
        uint32_t red, ir;
        float temperature;

        // Read from MAX30102
        max30102_read_fifo(&hi2c1, &red, &ir);

        // Read from MLX90614
       temperature = mlx90614_read_object_temp(&hi2c2);


        // Print results (assumes printf is properly retargeted to USART2)
        printf("Red: %u, IR: %u, Temp: %.2f C\r\n", red, ir, temperature);

        HAL_Delay(1000); // Delay 1 second
    }
}

/* System Clock Configuration */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Configure the main internal regulator output voltage */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Initializes the RCC Oscillators according to the specified parameters */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 84;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    /* Initializes the CPU, AHB and APB buses clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
}

/* Error Handler */
void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
        // Stay here
    }
}
