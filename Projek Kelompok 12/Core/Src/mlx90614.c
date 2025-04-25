#include "mlx90614.h"

HAL_StatusTypeDef MLX90614_Init(const I2C_HandleTypeDef *hi2c)
{
    uint8_t rx[3];
    // Baca sekali suhu ambien (CMD_TA) hanya untuk cek komunikasi
    HAL_StatusTypeDef st = HAL_I2C_Mem_Read((I2C_HandleTypeDef *)hi2c,
                                            (uint16_t)(MLX90614_I2C_ADDR << 1U),
                                            MLX90614_CMD_TA,
                                            I2C_MEMADD_SIZE_8BIT,
                                            rx,
                                            3U,
                                            1000U);
    return st;
}

HAL_StatusTypeDef MLX90614_ReadTemp(const I2C_HandleTypeDef *hi2c,
                                    uint8_t cmd,
                                    float *temperature)
{
    uint8_t rx[3];
    HAL_StatusTypeDef st = HAL_I2C_Mem_Read((I2C_HandleTypeDef *)hi2c,
                                            (uint16_t)(MLX90614_I2C_ADDR << 1U),
                                            cmd,
                                            I2C_MEMADD_SIZE_8BIT,
                                            rx,
                                            3U,
                                            1000U);
    if (st != HAL_OK) {
        return st;
    }
    uint16_t raw = (uint16_t)rx[0] | ((uint16_t)rx[1] << 8U);
    *temperature = ((float)raw * 0.02F) - 273.15F;
    return HAL_OK;
}
