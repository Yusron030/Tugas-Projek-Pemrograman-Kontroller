#ifndef _MLX90614CONFIG_H
#define _MLX90614CONFIG_H

// Gunakan handle hi2c2, bukan hi2c1
#define _MLX90614_I2C         hi2c2
#define _MLX90614_I2C_ADDRESS 0xB4

// Matikan FreeRTOS supaya tidak #include "cmsis_os.h"
#define _MLX90614_FREERTOS    0

#endif
