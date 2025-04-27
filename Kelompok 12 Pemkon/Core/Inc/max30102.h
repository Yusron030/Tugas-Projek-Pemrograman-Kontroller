#ifndef __MAX30102_H
#define __MAX30102_H

#include "stm32f4xx_hal.h"

#define MAX30102_ADDRESS           (0x57 << 1)
#define MAX30102_REG_INTR_STATUS_1 0x00
#define MAX30102_REG_INTR_ENABLE_1 0x02
#define MAX30102_REG_FIFO_WR_PTR   0x04
#define MAX30102_REG_FIFO_RD_PTR   0x06
#define MAX30102_REG_FIFO_DATA     0x07
#define MAX30102_REG_MODE_CONFIG   0x09
#define MAX30102_REG_SPO2_CONFIG   0x0A
#define MAX30102_REG_LED1_PA       0x0C
#define MAX30102_REG_LED2_PA       0x0D

typedef enum { MAX_OK = 0, MAX_ERR } max_status_t;

/**
 * @brief  Initialize MAX30102 sensor over I2C
 * @param  hi2c: pointer to HAL I2C handle
 * @retval MAX_OK on success, MAX_ERR on failure
 */
max_status_t max30102_init(I2C_HandleTypeDef *hi2c);

/**
 * @brief  Read one sample from FIFO (red & IR)
 * @param  hi2c: pointer to HAL I2C handle
 * @param  red: pointer to store Red LED data (18-bit)
 * @param  ir:  pointer to store IR LED data (18-bit)
 * @retval MAX_OK on success, MAX_ERR on failure
 */
max_status_t max30102_read_fifo(I2C_HandleTypeDef *hi2c, uint32_t *red, uint32_t *ir);

#endif /* __MAX30102_H */