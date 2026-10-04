#include <stdio.h>
#include <stdint.h>
#include "stm32g4xx.h"
#include "mpu6050.h"
#include "uart.h"


// Global variables
int16_t x, y, z;
float xg, yg, zg;

// External data buffers
extern char data;
extern uint8_t data_rec[6];

int main(void) {

    uart2_rxtx_init();

    // Initialize MPU6050 (includes I2C1_init internally)
    mpu6050_init();

    // WHO_AM_I test (device ID read)
    mpu6050_read_address(DEVICE_R);
    printf("WHO_AM_I = 0x%02X\r\n", data);


    printf("Loop running...\r\n");
    for (volatile int i = 0; i < 100000; i++);

    // Freeze here so you can inspect values in debugger/SWV
    while (1) {

    	printf("Reading sensor...\r\n");
    	mpu6050_read_values(DATA_START_ADDR);  //If you only see "Reading sensor..." but not "Read done" — I2C is stuck.
    	printf("Read done\r\n");

        x = (data_rec[1] << 8) | data_rec[0];
        for (volatile int i = 0; i < 100000; i++);

        y = (data_rec[3] << 8) | data_rec[2];
        for (volatile int i = 0; i < 100000; i++);

        z = (data_rec[5] << 8) | data_rec[4];
        for (volatile int i = 0; i < 100000; i++);

        xg = x / 8192.0;
        yg = y / 8192.0;
        zg = z / 8192.0;

        // Print readings to SWV
        printf("Accel (g): X=%.2f, Y=%.2f, Z=%.2f\n", xg, yg, zg);

        for (volatile int i = 0; i < 100000; i++);  // crude delay
    }
}
