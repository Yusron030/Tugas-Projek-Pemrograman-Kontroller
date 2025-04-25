#ifndef MLX90614_H
#define MLX90614_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

#define MLX90614_I2C_ADDR    ((uint16_t)0x5AU)
#define MLX90614_CMD_OBJ     ((uint8_t)0x07U)

HAL_StatusTypeDef
MLX90614_ReadTemp(const I2C_HandleTypeDef * const hi2c,
                  uint8_t cmd,
                  float * const temperature);

#endif /* MLX90614_H */