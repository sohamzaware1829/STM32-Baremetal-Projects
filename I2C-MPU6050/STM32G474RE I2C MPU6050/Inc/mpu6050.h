

#ifndef MPU6050_H_
#define MPU6050_H_

#include <stdint.h>   // ✅ Add this line
#include "i2c.h"

// I2C device address (AD0 pin = GND)
#define MPU6050_ADDR             0x68

// Register addresses
#define DEVICE_R                 0x75  // WHO_AM_I register
#define POWER_MGMT               0x6B  // Power management register
#define MPU6050_ACCEL_CONFIG     0x1C  // Accelerometer config
#define MPU6050_GYRO_CONFIG      0x1B  // Gyroscope config
#define DATA_START_ADDR          0x3B  // Start of accel/temp/gyro data

// Config values
#define FOUR_G                   0x08  // ±4g range setting
#define RESET                    0x80  // Device reset command
#define MPU6050_WAKE             0x00  // Wake from sleep mode


// Function prototypes
void mpu6050_init(void);
void mpu6050_write(uint8_t reg, char value);
void mpu6050_read_address(uint8_t reg);
void mpu6050_read_values(uint8_t reg);

#endif /* MPU6050_H_ */
