/* ======================================================================= */
/*                                main.h                                    */
/* ======================================================================= */
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

/* Pin definitions (dari CubeMX) */
#define B1_Pin              GPIO_PIN_13
#define B1_GPIO_Port        GPIOC
#define USART_TX_Pin        GPIO_PIN_2
#define USART_TX_GPIO_Port  GPIOA
#define USART_RX_Pin        GPIO_PIN_3
#define USART_RX_GPIO_Port  GPIOA
#define LD2_Pin             GPIO_PIN_5
#define LD2_GPIO_Port       GPIOA

/* Peripheral handles yg didefinisikan di gpio.c, i2c.c, usart.c */
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;
extern UART_HandleTypeDef huart2;

/* System & Error handlers */
void SystemClock_Config(void);
void Error_Handler(void);

/* Prototipe inisialisasi CubeMX-generated */
void MX_GPIO_Init(void);
void MX_I2C1_Init(void);
void MX_I2C2_Init(void);
void MX_USART2_UART_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
