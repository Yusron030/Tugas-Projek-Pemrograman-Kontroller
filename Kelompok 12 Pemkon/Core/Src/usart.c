#include "usart.h"
#include "main.h"    // ? tambahkan ini agar Error_Handler() dikenali

// � kode MX_USART2_UART_Init() �

UART_HandleTypeDef huart2;

/**
 * @brief USART2 Initialization Function
 * @retval None
 */
void MX_USART2_UART_Init(void)
{
  huart2.Instance          = USART2;
  huart2.Init.BaudRate     = 115200;
  huart2.Init.WordLength   = UART_WORDLENGTH_8B;
  huart2.Init.StopBits     = UART_STOPBITS_1;
  huart2.Init.Parity       = UART_PARITY_NONE;
  huart2.Init.Mode         = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}
