# STM32 Bare-Metal Projects

A collection of bare-metal embedded projects developed using **Embedded C** and **direct STM32 peripheral register programming**.

These projects focus on understanding STM32 peripherals at the hardware-register level instead of depending on high-level HAL drivers. The repository currently contains projects involving **ADC, UART, I2C, interrupts, GPIO, NVIC, and sensor interfacing**.

---

## Projects

| Project | MCU | Main Concepts |
|---|---|---|
| ADC Data Logger | STM32H755ZI-Q | ADC, Interrupts, UART, GPIO, Registers |
| MPU6050 I2C Interface | STM32G474RE | I2C, Sensor Interfacing, UART, Registers |

---

# 1. ADC Data Logger

## Overview

A bare-metal ADC data acquisition project developed using the **STM32H755ZI-Q**.

The project reads an analog signal connected to **PC0**, which is configured as **ADC1 Channel 1**. The ADC is configured directly through STM32 registers and the conversion completion is detected using the **End-of-Conversion (EOC) interrupt**.

Once the conversion is completed, the ADC result is read from the ADC data register and transmitted to a PC through **USART3**. `printf()` is redirected to UART to make debugging and monitoring easier.

This project demonstrates the complete path from an analog input to digital data and finally to a PC terminal.

## Main Objectives

- Understand STM32 ADC peripheral configuration
- Configure GPIO for analog input
- Configure ADC channels using registers
- Handle ADC conversion using interrupts
- Configure NVIC for ADC interrupt handling
- Configure USART3 without HAL
- Redirect `printf()` output to UART
- Understand register-level peripheral programming

## Hardware

- STM32H755ZI-Q Nucleo
- Analog input / potentiometer
- USB connection to PC

## Peripheral Configuration

### ADC

- Peripheral: **ADC1**
- Channel: **Channel 1**
- Analog Input: **PC0**
- Interrupt: **EOC**
- Conversion result: **ADC1->DR**

### UART

- Peripheral: **USART3**
- TX Pin: **PD8**
- Alternate Function: **AF7**
- Baud Rate: **115200**

## Working Principle

```text
Analog Signal
      ↓
     PC0
      ↓
    ADC1
      ↓
ADC Conversion
      ↓
   EOC Flag
      ↓
 ADC Interrupt
      ↓
Read ADC1->DR
      ↓
    printf()
      ↓
   USART3 TX
      ↓
   PC Terminal

ADC Configuration
The GPIO pin is configured in analog mode and ADC1 is enabled through the corresponding RCC registers.
The ADC channel is selected through the ADC sequence register.
The conversion is started using the ADC control register:
ADC1->CR |= CR_ADSTART;

When the conversion is complete, the ADC generates an EOC interrupt.
The interrupt handler checks the EOC flag:
void ADC_IRQHandler(void)
{
    if((ADC1->ISR & ISR_EOC) != 0)
    {
        ADC1->ISR &= ~ISR_EOC;
        adc_callback();
    }
}

The conversion result is then read from:
sensor_value = ADC1->DR;

The value is transmitted using:
printf("Sensor value is: %lu\r\n",
       (unsigned long)sensor_value);

UART Communication
USART3 is configured directly using registers.
The project uses PD8 as the UART transmit pin with Alternate Function 7.
printf() is redirected to USART3 using __io_putchar(), allowing ADC values to be printed directly to a serial terminal.
Example output:
Sensor value is: 1245
Sensor value is: 1321
Sensor value is: 1408
Sensor value is: 1512

Firmware Structure
ADC-Data-Logger/
└── STM32H755ZI-Q ADC Data Logger/
    ├── Inc/
    │   ├── adc.h
    │   └── uart.h
    │
    └── Src/
        ├── main.c
        ├── adc.c
        └── uart.c

main.c
Responsible for:
- Initializing USART3
- Initializing ADC interrupt configuration
- Running the main program
- Handling ADC callback processing
- Reading the ADC result
- Printing the converted value
adc.c
Responsible for:
- Enabling GPIOC clock
- Configuring PC0 as analog input
- Enabling ADC1 clock
- Selecting ADC channel
- Enabling ADC EOC interrupt
- Enabling the ADC interrupt in NVIC
- Enabling ADC1
- Starting ADC conversions
uart.c
Responsible for:
- Enabling GPIOD clock
- Configuring PD8 for USART3
- Selecting Alternate Function 7
- Configuring USART3
- Setting baud rate
- Transmitting characters
- Redirecting printf() to UART
2. MPU6050 I2C Interface
Overview
A bare-metal I2C sensor interfacing project developed using the STM32G474RE and an MPU6050 IMU sensor.
The STM32 communicates with the MPU6050 using I2C1. The project implements the low-level I2C communication directly through STM32 registers instead of using HAL I2C APIs.
The firmware verifies the connected sensor using the WHO_AM_I register, performs sensor initialization, configures the accelerometer, reads six bytes of accelerometer data, combines the received bytes into signed 16-bit values, and converts the raw readings into acceleration in g.
The processed sensor values are then displayed on a PC using USART2.
Main Objectives
- Understand I2C master communication
- Configure I2C using STM32 registers
- Interface an external sensor
- Implement register read and write operations
- Perform I2C burst transactions
- Verify sensor identity
- Configure MPU6050 registers
- Read accelerometer data
- Convert raw sensor data into physical units
- Display sensor data through UART
Hardware
- STM32G474RE
- MPU6050
- USB connection to PC
- I2C pull-up resistors
Peripheral Configuration
I2C
- Peripheral: I2C1
- SCL: PB8
- SDA: PB9
- Alternate Function: AF4
- Speed: 100 kHz Standard Mode
- Communication: Master
UART
- Peripheral: USART2
- TX: PA2
- RX: PA3
- Alternate Function: AF7
- Baud Rate: 115200
MPU6050 Initialization
The sensor initialization follows this sequence:
Start
  ↓
Initialize USART2
  ↓
Initialize I2C1
  ↓
Read WHO_AM_I
  ↓
Reset MPU6050
  ↓
Wake MPU6050
  ↓
Configure Accelerometer
  ↓
Start Sensor Reading

The WHO_AM_I register is used to verify that the STM32 is communicating with the expected MPU6050 device.
Example:
WHO_AM_I = 0x68

The project then configures the sensor's power management and accelerometer settings before starting data acquisition.
I2C Communication
The project contains a low-level I2C driver that directly controls the STM32 I2C peripheral.
The driver implements:
I2C1_byteRead();
I2C1_burstRead();
I2C1_burstWrite();

These functions handle the basic I2C transaction sequence including:
START
  ↓
Slave Address
  ↓
Register Address
  ↓
Read / Write
  ↓
Data Transfer
  ↓
Transfer Complete
  ↓
STOP

The driver checks peripheral status flags such as:
- BUSY
- TXIS
- TC
- RXNE
- STOPF
This provides direct experience with the STM32 I2C peripheral rather than hiding the communication process behind a library.
MPU6050 Register Communication
The firmware performs both register read and register write operations.
For example, sensor configuration is performed by writing values to MPU6050 registers.
The project also reads multiple consecutive registers using a burst read operation.
The accelerometer data consists of six bytes:
X High Byte
X Low Byte

Y High Byte
Y Low Byte

Z High Byte
Z Low Byte

These bytes are combined into signed 16-bit values.
x = (data_rec[1] << 8) | data_rec[0];

y = (data_rec[3] << 8) | data_rec[2];

z = (data_rec[5] << 8) | data_rec[4];

Accelerometer Data Conversion
The accelerometer is configured for the ±4g range.
For this range, the sensitivity used by the project is:
8192 LSB/g

Therefore, the raw accelerometer values are converted using:
xg = x / 8192.0;
yg = y / 8192.0;
zg = z / 8192.0;

This converts the raw signed ADC-like sensor output into acceleration values expressed in g.
Example output:
WHO_AM_I = 0x68

Reading sensor...
Read done

Accel (g): X=0.12, Y=-0.03, Z=0.98

UART Debug Output
USART2 is used to monitor the sensor communication and display the measured acceleration values.
The UART output is useful for:
- Verifying sensor communication
- Checking WHO_AM_I
- Monitoring I2C transactions
- Viewing accelerometer readings
- Debugging the firmware
printf() is redirected to USART2 so that messages can be written directly from the application code.
Firmware Structure
I2C-MPU6050/
└── STM32G474RE I2C MPU6050/
    ├── Inc/
    │   ├── i2c.h
    │   ├── mpu6050.h
    │   └── uart.h
    │
    └── Src/
        ├── main.c
        ├── i2c.c
        ├── mpu6050.c
        └── uart.c

main.c
Responsible for:
- Initializing UART
- Initializing the MPU6050
- Reading the sensor identity
- Requesting accelerometer data
- Combining raw bytes
- Converting raw values to g
- Printing X, Y and Z acceleration
i2c.c
Contains the low-level I2C1 driver.
Responsibilities include:
- GPIO configuration
- I2C clock configuration
- I2C timing configuration
- START generation
- STOP generation
- Read/write control
- Transmit handling
- Receive handling
- Byte transactions
- Burst transactions
mpu6050.c
Contains MPU6050-specific functions for:
- Reading sensor registers
- Writing sensor registers
- Reading accelerometer data
- Sensor initialization
- Accelerometer configuration
uart.c
Responsible for:
- USART2 configuration
- GPIO configuration
- Baud-rate configuration
- UART transmission
- printf() redirection
Technical Skills Demonstrated
Embedded C
- Direct register manipulation
- Bitwise operations
- Peripheral register access
- Interrupt service routines
- Modular driver development
- Data conversion
- printf() UART retargeting
STM32 Peripherals
- GPIO
- ADC
- I2C
- USART
- NVIC
- RCC / clock configuration
- Alternate Function configuration
Communication Protocols
- UART
- I2C
Embedded Concepts
- Interrupt-driven ADC acquisition
- Polling-based I2C communication
- Sensor register communication
- Peripheral initialization
- Hardware-level debugging
- Modular firmware organization
Development Approach
The projects are intentionally implemented at the register level to build a stronger understanding of how STM32 peripherals operate internally.
Instead of using high-level APIs such as:
HAL_ADC_Start();
HAL_I2C_Master_Transmit();

the projects directly configure and access STM32 peripheral registers.
The general firmware approach is:
Enable Peripheral Clock
        ↓
Configure GPIO
        ↓
Configure Peripheral Registers
        ↓
Enable Peripheral
        ↓
Start Communication / Conversion
        ↓
Handle Data
        ↓
Process Data
        ↓
UART Debug Output

This provides practical experience with the relationship between microcontroller hardware, registers, peripheral configuration, and application-level firmware.
Development Environment
- IDE: STM32CubeIDE
- Language: Embedded C
- MCUs: STM32H755ZI-Q, STM32G474RE
- Debugging: ST-LINK / UART terminal
- Programming Approach: Bare-Metal / Register-Level
Future Improvements
Possible extensions to these projects include:
- ADC DMA-based data acquisition
- Timer-triggered ADC sampling
- I2C timeout and error handling
- MPU6050 gyroscope integration
- Complete IMU data acquisition
- External data logging
- Sensor fusion
- Real-time sensor visualization
- FreeRTOS-based implementation
Repository Structure
STM32-BareMetal-Projects/
│
├── ADC-Data-Logger/
│   └── STM32H755ZI-Q ADC Data Logger/
│       ├── Inc/
│       │   ├── adc.h
│       │   └── uart.h
│       │
│       └── Src/
│           ├── main.c
│           ├── adc.c
│           └── uart.c
│
├── I2C-MPU6050/
│   └── STM32G474RE I2C MPU6050/
│       ├── Inc/
│       │   ├── i2c.h
│       │   ├── mpu6050.h
│       │   └── uart.h
│       │
│       └── Src/
│           ├── main.c
│           ├── i2c.c
│           ├── mpu6050.c
│           └── uart.c
│
└── README.md
