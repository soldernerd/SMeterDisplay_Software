/* 
 * File:   i2c.h
 * Author: Luke
 *
 * Created on 4. September 2016, 09:26
 */

#ifndef I2C_H
#define	I2C_H

#include <stdint.h>
#include "os.h"

typedef enum
{
    I2C_ADC_RESOLUTION_12BIT,
    I2C_ADC_RESOLUTION_14BIT,
    I2C_ADC_RESOLUTION_16BIT, 
} i2cAdcResolution_t;

typedef enum
{
    I2C_ADC_GAIN_1,
    I2C_ADC_GAIN_2,
    I2C_ADC_GAIN_4,
    I2C_ADC_GAIN_8
    
} i2cAdcGain_t;

void i2c_init(void);

#define I2C_EEPROM_LCD_CONFIG_ADDRESS 0x0000
#define I2C_EEPROM_CALIBRATION_ADDRESS 0x0100


void i2c_eeprom_writeByte(uint16_t address, uint8_t data);
uint8_t i2c_eeprom_readByte(uint16_t address);
void i2c_eeprom_write(uint16_t address, uint8_t *data, uint8_t length);
void i2c_eeprom_read(uint16_t address, uint8_t *data, uint8_t length);
uint8_t i2c_eeprom_busy(void);

void i2c_eeprom_calibration_write(int32_t value, uint8_t index);
int32_t i2c_eeprom_calibration_read(uint8_t index);

void i2c_adc_start(i2cAdcResolution_t resolution, i2cAdcGain_t gain);
int16_t i2c_adc_read(void);


#endif	/* I2C_H */

