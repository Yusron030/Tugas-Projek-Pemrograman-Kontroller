#include "mlx90614.h"

void mlx90614_init(I2C_HandleTypeDef *hi2c)
{
    // Biasanya tidak perlu inisialisasi khusus untuk MLX90614
    // Sensor siap langsung dibaca setelah power-up
}

float mlx90614_read_object_temp(I2C_HandleTypeDef *hi2c)
{
    uint8_t reg = MLX90614_TOBJ1;
    uint8_t data[3]; // 2 data + 1 PEC
    uint16_t temp_raw;
    HAL_StatusTypeDef ret;

    ret = HAL_I2C_Mem_Read(hi2c, MLX90614_I2CADDR, reg, I2C_MEMADD_SIZE_8BIT, data, 3, 100);

    if (ret != HAL_OK)
    {
        return -273.15f; // Error indicator (absolute zero)
    }

    temp_raw = (uint16_t)data[1] << 8 | data[0];

    // Convert to Celsius
    float temp_celsius = (temp_raw * 0.02) - 273.15;

    return temp_celsius;
}
