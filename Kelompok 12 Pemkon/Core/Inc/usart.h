#ifndef __USART_H
#define __USART_H

#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart2;

/**
 * @brief USART2 Initialization Function
 * @retval None
 */
void MX_USART2_UART_Init(void);

#endif /* __USART_H */