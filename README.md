# STM32 Bare-Metal Projects

A collection of low-level STM32 embedded firmware projects developed using **Embedded C and direct peripheral register programming**.

These projects focus on understanding how STM32 peripherals work at the hardware-register level rather than relying on high-level HAL APIs.

The repository currently contains two working projects:

- **ADC Data Logger — STM32H755ZI-Q**
- **MPU6050 I2C Interface — STM32G474RE**

---

## What This Repository Demonstrates

These projects demonstrate practical experience with:

- Bare-metal Embedded C
- STM32 memory-mapped peripheral registers
- GPIO configuration
- RCC/peripheral clock configuration
- ADC
- UART/USART
- I2C
- NVIC interrupts
- Interrupt Service Routines
- Sensor interfacing
- Register-level debugging
- Bit manipulation and masking
- UART/SWV based debugging and logging
- Modular peripheral-driver development

The main objective is to understand and control STM32 peripherals directly through their registers.

---

# Repository Structure

```text
STM32-Baremetal-Projects/
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
├── .gitignore
└── README.md
