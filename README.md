# STM32 Bare-Metal Projects

> A collection of **bare-metal embedded projects** developed using **Embedded C** and **direct STM32 peripheral register programming**.

These projects focus on understanding STM32 peripherals at the **hardware-register level**, including ADC, UART, I2C, GPIO, interrupts, NVIC, and sensor interfacing.

The goal is to build practical experience in developing embedded firmware **without depending on high-level HAL APIs** for the core peripheral operations.

---

## 📌 Projects

| Project | MCU | Main Concepts |
|---|---|---|
| **ADC Data Logger** | STM32H755ZI-Q | ADC, Interrupts, UART, GPIO, Registers |
| **MPU6050 I2C Interface** | STM32G474RE | I2C, Sensor Interfacing, UART, Registers |

---

# 🔹 1. ADC Data Logger

## Overview

A bare-metal ADC data acquisition project developed using the **STM32H755ZI-Q**.

An analog signal is connected to **PC0**, configured as **ADC1 Channel 1**. The ADC is configured directly through STM32 peripheral registers and the conversion is handled using the **End-of-Conversion (EOC) interrupt**.

After the conversion is completed, the ADC result is read from the ADC data register and transmitted to a PC through **USART3**.

`printf()` is redirected to UART for displaying the acquired values on a serial terminal.

This project demonstrates the complete process of:

- Configuring an ADC
- Handling an interrupt
- Reading conversion data
- Transmitting the result through UART

---

## ✨ Key Features

- ADC1 Channel 1 configured on PC0
- Analog GPIO configuration
- ADC conversion using EOC interrupt
- NVIC interrupt configuration
- Direct ADC register programming
- USART3 communication at 115200 baud
- `printf()` redirected to UART
- Modular ADC and UART drivers

---

## 🛠 Hardware

- STM32H755ZI-Q Nucleo
- Analog input / potentiometer
- USB connection to PC

---

## ⚙️ Peripheral Configuration

### ADC

| Parameter | Configuration |
|---|---|
| Peripheral | ADC1 |
| Channel | Channel 1 |
| Input Pin | PC0 |
| Interrupt | EOC |
| Data Register | `ADC1->DR` |

### UART

| Parameter | Configuration |
|---|---|
| Peripheral | USART3 |
| TX Pin | PD8 |
| Alternate Function | AF7 |
| Baud Rate | 115200 |

---

## 🔄 Working Flow

```text
Analog Signal
      │
      ▼
     PC0
      │
      ▼
    ADC1
      │
      ▼
 ADC Conversion
      │
      ▼
 EOC Interrupt
      │
      ▼
Read ADC1->DR
      │
      ▼
   printf()
      │
      ▼
 USART3 TX
      │
      ▼
 PC Terminal

💻 ADC Implementation
The ADC conversion is started by setting the ADSTART bit in the ADC control register:
ADC1->CR |= CR_ADSTART;

When the conversion is completed, the EOC flag is checked inside the ADC interrupt handler:
void ADC_IRQHandler(void)
{
    if((ADC1->ISR & ISR_EOC) != 0)
    {
        ADC1->ISR &= ~ISR_EOC;
        adc_callback();
    }
}

The converted ADC value is read from the ADC data register:
sensor_value = ADC1->DR;

The value is then sent to the PC through UART:
printf("Sensor value is: %lu\r\n",
       (unsigned long)sensor_value);

📁 Firmware Structure
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
- Handling ADC interrupt processing
- Reading the ADC conversion result
- Sending the result through UART
adc.c
Responsible for:
- Enabling GPIOC clock
- Configuring PC0 as analog input
- Enabling ADC1 clock
- Selecting ADC Channel 1
- Configuring ADC EOC interrupt
- Enabling ADC interrupt through NVIC
- Enabling ADC1
- Starting ADC conversions
uart.c
Responsible for:
- Enabling GPIOD clock
- Configuring PD8 for USART3
- Selecting Alternate Function 7
- Configuring USART3
- Setting the baud rate
- Transmitting characters
- Redirecting printf() to UART
🔹 2. MPU6050 I2C Interface
Overview
A bare-metal I2C sensor interfacing project developed using the STM32G474RE and an MPU6050 IMU sensor.
The STM32 communicates with the MPU6050 using I2C1.
The I2C peripheral is configured directly through STM32 registers and a custom low-level I2C driver is used for sensor communication.
The project verifies the connected MPU6050 using the WHO_AM_I register, performs sensor initialization, resets and wakes the device, configures the accelerometer, and reads the accelerometer data using an I2C burst-read operation.
The six bytes of accelerometer data are combined into signed 16-bit X, Y, and Z values and converted into acceleration in g.
The results are displayed on a PC through USART2.
✨ Key Features
- Bare-metal I2C1 driver
- Direct STM32 I2C register programming
- MPU6050 register read/write operations
- WHO_AM_I sensor verification
- MPU6050 reset and wake-up
- Accelerometer configuration
- Six-byte burst data acquisition
- Raw accelerometer data processing
- Conversion of raw data into g
- USART2 debug output
- Modular I2C, MPU6050, and UART drivers
🛠 Hardware
- STM32G474RE
- MPU6050 IMU sensor
- USB connection to PC
- I2C pull-up resistors
⚙️ Peripheral Configuration
I2C
Parameter	Configuration
Peripheral	I2C1
SCL	PB8
SDA	PB9
Alternate Function	AF4
Speed	100 kHz Standard Mode


UART
Parameter	Configuration
Peripheral	USART2
TX Pin	PA2
RX Pin	PA3
Alternate Function	AF7
Baud Rate	115200


🔄 Working Flow
              STM32G474RE
                   │
                   │ I2C1
                   │
             ┌─────┴─────┐
             │  MPU6050  │
             └─────┬─────┘
                   │
                   ▼
               WHO_AM_I
                   │
                   ▼
             Sensor Reset
                   │
                   ▼
              Wake Sensor
                   │
                   ▼
       Configure Accelerometer
                   │
                   ▼
          Burst Read 6 Bytes
                   │
                   ▼
             X / Y / Z Raw
                   │
                   ▼
             Convert to g
                   │
                   ▼
               USART2
                   │
                   ▼
              PC Terminal

🔧 MPU6050 Initialization
The MPU6050 is initialized through I2C register communication.
The initialization sequence is:
Start
  │
  ▼
Initialize USART2
  │
  ▼
Initialize I2C1
  │
  ▼
Read WHO_AM_I
  │
  ▼
Reset MPU6050
  │
  ▼
Wake MPU6050
  │
  ▼
Configure Accelerometer
  │
  ▼
Start Sensor Reading

The firmware reads the WHO_AM_I register to verify communication with the sensor.
Example:
WHO_AM_I = 0x68

The sensor is then reset and taken out of sleep mode before configuring the accelerometer.
🔌 I2C Driver
The project contains a low-level I2C driver that directly controls the STM32 I2C peripheral.
Main I2C Functions
I2C1_byteRead();
I2C1_burstRead();
I2C1_burstWrite();

These functions are used for communication with the MPU6050 registers.
The driver handles the major parts of an I2C transaction:
START
  │
  ▼
Slave Address
  │
  ▼
Register Address
  │
  ▼
Read / Write
  │
  ▼
Data Transfer
  │
  ▼
Transfer Complete
  │
  ▼
STOP

The implementation checks I2C status conditions such as:
- BUSY
- TXIS
- TC
- RXNE
- STOPF
This gives direct control over the I2C communication process at the peripheral-register level.
📡 MPU6050 Register Communication
The project performs both read and write operations on MPU6050 registers.
The firmware uses register access to:
- Verify the sensor identity
- Reset the sensor
- Wake the sensor
- Configure the accelerometer
- Read accelerometer measurements
📊 Accelerometer Data Acquisition
Accelerometer data is read as a burst of six consecutive bytes:
X High Byte
X Low Byte

Y High Byte
Y Low Byte

Z High Byte
Z Low Byte

📐 Accelerometer Data Processing
The received high and low bytes are combined to form signed 16-bit acceleration values:
x = (data_rec[1] << 8) | data_rec[0];

y = (data_rec[3] << 8) | data_rec[2];

z = (data_rec[5] << 8) | data_rec[4];

The accelerometer is configured for the ±4g range.
For this range, the sensitivity used by the project is:
8192 LSB/g

The raw values are converted into acceleration in g:
xg = x / 8192.0;
yg = y / 8192.0;
zg = z / 8192.0;

🖥 Example UART Output
WHO_AM_I = 0x68

Reading sensor...
Read done

Accel (g): X=0.12, Y=-0.03, Z=0.98

The UART output provides a simple way to verify sensor communication and observe the accelerometer measurements.
📁 Firmware Structure
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
- Initializing USART2
- Initializing the MPU6050
- Reading and displaying WHO_AM_I
- Requesting accelerometer data
- Combining high and low bytes
- Converting raw data into g
- Printing X, Y and Z acceleration
i2c.c
Contains the low-level I2C1 driver.
Responsible for:
- Configuring GPIO pins
- Enabling I2C1 clock
- Configuring I2C timing
- Generating START conditions
- Generating STOP conditions
- Handling transmit operations
- Handling receive operations
- Performing byte reads
- Performing burst reads
- Performing burst writes
mpu6050.c
Contains MPU6050-specific functions for:
- Reading sensor registers
- Writing sensor registers
- Reading accelerometer data
- Initializing the MPU6050
- Configuring the accelerometer
uart.c
Responsible for:
- Configuring USART2
- Configuring PA2 and PA3
- Setting the baud rate
- UART transmission
- Redirecting printf() to USART2
🧠 Technical Skills Demonstrated
Embedded C
- Direct peripheral register manipulation
- Bitwise operations
- Register-level programming
- Interrupt service routines
- Modular firmware development
- Sensor data processing
- printf() UART retargeting
STM32 Peripherals
- GPIO
- ADC
- I2C
- USART
- NVIC
- RCC / peripheral clock configuration
- Alternate Function configuration
Communication Protocols
- UART
- I2C
Embedded Concepts
- Bare-metal firmware development
- Interrupt-driven ADC acquisition
- Polling-based I2C communication
- Sensor register communication
- Peripheral initialization
- Hardware-level debugging
- Modular driver architecture
🏗 Development Approach
The projects are implemented at the register level to understand how STM32 peripherals operate internally.
Instead of depending on high-level HAL functions, the firmware directly configures peripheral registers for:
- ADC
- I2C
- UART
- GPIO
- Interrupt handling
The general firmware flow is:
Enable Peripheral Clock
        │
        ▼
Configure GPIO
        │
        ▼
Configure Peripheral Registers
        │
        ▼
Enable Peripheral
        │
        ▼
Start Conversion / Communication
        │
        ▼
Acquire Data
        │
        ▼
Process Data
        │
        ▼
UART Debug Output

This approach provides practical experience with the relationship between:
Microcontroller Hardware
        ↓
Peripheral Registers
        ↓
Communication Protocols
        ↓
Interrupts
        ↓
Embedded Application Firmware

🧰 Development Environment
Category	Details
IDE	STM32CubeIDE
Language	Embedded C
MCUs	STM32H755ZI-Q, STM32G474RE
Debug Interface	ST-LINK
Communication	UART / I2C
Programming Approach	Bare-Metal / Register-Level


🚀 Future Improvements
Possible extensions include:
- ADC DMA-based data acquisition
- Timer-triggered ADC sampling
- I2C timeout and error handling
- MPU6050 gyroscope integration
- Complete IMU data acquisition
- External data logging
- Sensor fusion
- Real-time sensor visualization
- FreeRTOS-based implementation
📂 Repository Structure
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

👨‍💻 Author
Soham Zaware
B.Tech Electronics & Telecommunication Engineering
