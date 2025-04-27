#include "mlx90614.h"
#include <math.h>  // for NAN

static I2C_HandleTypeDef *mlx_i2c;

// CRC8 for PEC (polynomial x^8 + x^2 + x + 1)
static uint8_t mlx_crc8(const uint8_t *data, uint8_t len)
{
    uint8_t crc = 0;
    while (len--) {
        uint8_t in = *data++;
        for (uint8_t i = 0; i < 8; i++) {
            uint8_t mix = (crc ^ in) & 0x80;
            crc <<= 1;
            if (mix) crc ^= 0x07;
            in <<= 1;
        }
    }
    return crc;
}

// Read 16-bit register with PEC check
static bool mlx_read_reg(uint8_t reg, uint16_t *raw)
{
    uint8_t buf[3];
    if (HAL_I2C_Mem_Read(mlx_i2c, MLX90614_I2C_ADDR, reg,
                         I2C_MEMADD_SIZE_8BIT, buf, 3, HAL_MAX_DELAY)
        != HAL_OK) {
        return false;
    }
    // verify PEC: [addr<<1, reg, addr<<1|1, data_l, data_h]
    uint8_t seq[5] = { (uint8_t)(MLX90614_I2C_ADDR), reg,
                       (uint8_t)(MLX90614_I2C_ADDR | 1), buf[0], buf[1] };
    if (mlx_crc8(seq, 5) != buf[2]) {
        return false;
    }
    *raw = (uint16_t)buf[0] | ((uint16_t)buf[1] << 8);
    return true;
}

bool mlx90614_init(I2C_HandleTypeDef *hi2c)
{
    mlx_i2c = hi2c;
    return (HAL_I2C_IsDeviceReady(mlx_i2c, MLX90614_I2C_ADDR, 3, HAL_MAX_DELAY) == HAL_OK);
}

bool mlx90614_read_ambient_temp(I2C_HandleTypeDef *hi2c, float *temp_c)
{
    (void)hi2c; // use stored handle
    uint16_t raw;
    if (!mlx_read_reg(MLX90614_REG_TA, &raw))
        return false;
    *temp_c = (float)raw * 0.02f - 273.15f;
    return true;
}

bool mlx90614_read_object_temp(I2C_HandleTypeDef *hi2c, float *temp_c)
{
    (void)hi2c;
    uint16_t raw;
    if (!mlx_read_reg(MLX90614_REG_TOBJ1, &raw))
        return false;
    *temp_c = (float)raw * 0.02f - 273.15f;
    return true;
}

float mlx90614_read_object_temp_avg(I2C_HandleTypeDef *hi2c, uint8_t samples)
{
    (void)hi2c;
    if (samples == 0) return NAN;
    float sum = 0.0f;
    uint8_t count = 0;
    for (uint8_t i = 0; i < samples; i++) {
        float t;
        if (mlx90614_read_object_temp(mlx_i2c, &t)) {
            sum += t;
            count++;
        }
        HAL_Delay(50);
    }
    return count ? (sum / count) : NAN;
}
