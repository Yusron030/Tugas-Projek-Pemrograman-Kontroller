/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include "stdio.h"
#include "string.h"
#include "MAX30102/MAX30102.h"

/* Private define ------------------------------------------------------------*/
#define UART_BUFFER_SIZE  (32U)

/* Private variables ---------------------------------------------------------*/
static char UartBuffer[UART_BUFFER_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void MX_NVIC_Init(void);
static void UART2_SendString(const char *s);

/* USER CODE BEGIN 0 */
static void UART2_SendString(const char *s)
{
  if (s != NULL)
  {
    (void)HAL_UART_Transmit(&huart2, (uint8_t *)s, (uint16_t)strlen(s), 1000U);
  }
}
/* USER CODE END 0 */

int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_NVIC_Init();

  UART2_SendString("START\n\r");
  Max30102_Init(&hi2c1);

  for (;;)
  {
    Max30102_Task();

    (void)sprintf(UartBuffer, "%c[2J%c[H", 27, 27);
    UART2_SendString(UartBuffer);

    (void)sprintf(UartBuffer, "HR: %d\n\rSpO2: %d\n\r", Max30102_GetHeartRate(), Max30102_GetSpO2Value());
    UART2_SendString(UartBuffer);
  }
}

static void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8U;
  RCC_OscInitStruct.PLL.PLLN = 84U;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4U;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_NVIC_Init(void)
{
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1U, 0U);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == INT_Pin)
  {
    Max30102_InterruptCallback();
  }
}

void Error_Handler(void)
{
  for (;;) {} /* Stay in loop if error occurs */
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  (void)file;
  (void)line;
  /* Optional: Add log or indicator for debug */
}
#endif
