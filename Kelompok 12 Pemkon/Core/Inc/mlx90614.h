#ifndef __MLX90614_H
#define __MLX90614_H

#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

#define MLX90614_I2C_ADDR   (0x5A << 1)
#define MLX90614_REG_TA     0x06  // Ambient temperature register
#define MLX90614_REG_TOBJ1  0x07  // Object temperature register

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  Initialize MLX90614 sensor (check device readiness)
 * @param  hi2c Pointer to HAL I2C handle (e.g., &hi2c2)
 * @retval true if sensor ACKs, false otherwise
 */
bool mlx90614_init(I2C_HandleTypeDef *hi2c);

/**
 * @brief  Read ambient temperature
 * @param  hi2c    Pointer to HAL I2C handle
 * @param  temp_c  Pointer to float to receive temperature (°C)
 * @retval true on success, false on CRC or I2C error
 */
bool mlx90614_read_ambient_temp(I2C_HandleTypeDef *hi2c, float *temp_c);

/**
 * @brief  Read object temperature
 * @param  hi2c    Pointer to HAL I2C handle
 * @param  temp_c  Pointer to float to receive temperature (°C)
 * @retval true on success, false on CRC or I2C error
 */
bool mlx90614_read_object_temp(I2C_HandleTypeDef *hi2c, float *temp_c);

/**
 * @brief  Read and average multiple object temperature samples to reduce noise
 * @param  hi2c     Pointer to HAL I2C handle
 * @param  samples  Number of samples to average
 * @return Averaged temperature (°C), or NaN if all reads fail
 */
float mlx90614_read_object_temp_avg(I2C_HandleTypeDef *hi2c, uint8_t samples);

#ifdef __cplusplus
}
#endif

#endif // __MLX90614_H