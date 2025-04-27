#ifndef __MLX90614_H__
#define __MLX90614_H__

#include "stm32f4xx_hal.h"
#define mlx90614_read_temp  mlx90614_read_object_temp

// Alamat default MLX90614 (jika tidak diubah)
#define MLX90614_I2CADDR      (0x5A << 1) // HARUS shift kiri 1 bit untuk HAL (7-bit address)

#define MLX90614_TA           0x06  // Ambient Temperature Register
#define MLX90614_TOBJ1        0x07  // Object 1 Temperature Register

void mlx90614_init(I2C_HandleTypeDef *hi2c);
float mlx90614_read_object_temp(I2C_HandleTypeDef *hi2c);

#endif
