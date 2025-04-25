#include "mlx90614.h"

HAL_StatusTypeDef MLX90614_ReadTemp(const I2C_HandleTypeDef * const hi2c,
                                    uint8_t cmd,
                                    float * const temperature)
{
    uint8_t rx[3U];
    HAL_StatusTypeDef st;

    st = HAL_I2C_Mem_Read((I2C_HandleTypeDef *)hi2c,
                          (uint16_t)(MLX90614_I2C_ADDR << 1U),
                          cmd,
                          I2C_MEMADD_SIZE_8BIT,
                          rx,
                          (uint16_t)3U,
                          (uint32_t)1000U);
    if (st != HAL_OK)
    {
        return st;
    }

    {
        uint16_t raw = (uint16_t)rx[0U] |
                       ((uint16_t)rx[1U] << 8U);
        *temperature = ((float)raw * 0.02F) - 273.15F;
    }
    return HAL_OK;
}