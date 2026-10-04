#include "mpu6050.h"

char data;
uint8_t data_rec[6];

void mpu6050_read_address(uint8_t reg){
    I2C1_byteRead(MPU6050_ADDR, reg, &data);
    printf("Read 0x%02X from register 0x%02X\r\n", data, reg);
}

void mpu6050_write(uint8_t reg, char value){
    char buffer[1];
    buffer[0] = value;
    I2C1_burstWrite(MPU6050_ADDR, reg, 1, buffer);
}

void mpu6050_read_values(uint8_t reg){
    I2C1_burstRead(MPU6050_ADDR, reg, 6, (char*) data_rec);
}

void mpu6050_init(void){

	I2C1_init();

    // 1. (Optional) Read WHO_AM_I (should return 0x68)
    mpu6050_read_address(DEVICE_R);
    printf("WHO_AM_I: 0x%02X\r\n", data);  // Expect 0x68

    // 2. Reset the device
    mpu6050_write(POWER_MGMT, RESET);

    // 3. Wait for device to reset
    for (volatile int i = 0; i < 100000; i++);  // crude delay

    // 4. Wake up MPU6050
    mpu6050_write(POWER_MGMT, 0x00);  // SET_MEASUREMENT_B (wake)

    // 5. Set accelerometer to ±4g
    mpu6050_write(MPU6050_ACCEL_CONFIG, FOUR_G);
}
