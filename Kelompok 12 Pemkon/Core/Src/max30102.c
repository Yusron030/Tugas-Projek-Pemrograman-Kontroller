#include "max30102.h"

static I2C_HandleTypeDef *hmax;

max_status_t max30102_init(I2C_HandleTypeDef *hi2c)
{
    hmax = hi2c;
    uint8_t cfg;

    // Reset device
    cfg = 0x40;
    if (HAL_I2C_Mem_Write(hmax, MAX30102_ADDRESS, MAX30102_REG_MODE_CONFIG,
                          I2C_MEMADD_SIZE_8BIT, &cfg, 1, HAL_MAX_DELAY) != HAL_OK) {
        return MAX_ERR;
    }
    HAL_Delay(100);

    // Set SpO2 config: 100 Hz sample, 411 us pulse width
    cfg = 0x27; // SPO2_CFG: SPO2_ADC range 4096nA, sample rate 100Hz, PW 411us
    if (HAL_I2C_Mem_Write(hmax, MAX30102_ADDRESS, MAX30102_REG_SPO2_CONFIG,
                          I2C_MEMADD_SIZE_8BIT, &cfg, 1, HAL_MAX_DELAY) != HAL_OK) {
        return MAX_ERR;
    }

    // Enable interrupt on new FIFO data
    cfg = 0x02;
    if (HAL_I2C_Mem_Write(hmax, MAX30102_ADDRESS, MAX30102_REG_INTR_ENABLE_1,
                          I2C_MEMADD_SIZE_8BIT, &cfg, 1, HAL_MAX_DELAY) != HAL_OK) {
        return MAX_ERR;
    }

    // Set LED pulse amplitudes
    cfg = 0x24; // LED1 (Red) ~7.6mA
    HAL_I2C_Mem_Write(hmax, MAX30102_ADDRESS, MAX30102_REG_LED1_PA,
                      I2C_MEMADD_SIZE_8BIT, &cfg, 1, HAL_MAX_DELAY);
    cfg = 0x24; // LED2 (IR) ~7.6mA
    HAL_I2C_Mem_Write(hmax, MAX30102_ADDRESS, MAX30102_REG_LED2_PA,
                      I2C_MEMADD_SIZE_8BIT, &cfg, 1, HAL_MAX_DELAY);

    return MAX_OK;
}

max_status_t max30102_read_fifo(I2C_HandleTypeDef *hi2c, uint32_t *red, uint32_t *ir)
{
    uint8_t raw[6];
    if (HAL_I2C_Mem_Read(hmax, MAX30102_ADDRESS, MAX30102_REG_FIFO_DATA,
                         I2C_MEMADD_SIZE_8BIT, raw, 6, HAL_MAX_DELAY) != HAL_OK) {
        return MAX_ERR;
    }
    // 18-bit values: MSB[2] + mid[3] + LSB[3]
    *red = ((uint32_t)raw[0] << 16) | ((uint32_t)raw[1] << 8) | raw[2];
    *ir  = ((uint32_t)raw[3] << 16) | ((uint32_t)raw[4] << 8) | raw[5];
    return MAX_OK;
}