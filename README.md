# STM32 Bare-Metal Projects

A collection of bare-metal embedded projects developed using **Embedded C** and **direct STM32 peripheral register programming**.

The projects focus on understanding how STM32 peripherals work at the hardware-register level, including ADC, UART, I2C, interrupts, GPIO, and sensor interfacing.

## Projects

| Project | MCU | Main Concepts |
|---|---|---|
| ADC Data Logger | STM32H755ZI-Q | ADC, Interrupts, UART, Register Programming |
| MPU6050 I2C Interface | STM32G474RE | I2C, Sensor Interface, UART, Register Programming |

---

# 1. ADC Data Logger

## Overview

A bare-metal ADC data acquisition project developed using the **STM32H755ZI-Q**.

An analog signal is applied to **PC0 (ADC1 Channel 1)**. The ADC conversion is handled using the **End-of-Conversion (EOC) interrupt**, and the converted value is transmitted to a PC through **USART3**.

The project demonstrates how ADC, GPIO, NVIC, and UART can be configured directly through STM32 peripheral registers without relying on HAL drivers.

## Key Features

- ADC1 Channel 1 configured on PC0
- ADC conversion using EOC interrupt
- Direct register-level ADC configuration
- USART3 communication at 115200 baud
- `printf()` redirected to UART
- Modular ADC and UART drivers

## Working Flow

```text
Analog Signal
      ↓
     PC0
      ↓
    ADC1
      ↓
ADC Conversion
      ↓
   EOC Interrupt
      ↓
Read ADC1->DR
      ↓
    printf()
      ↓
   USART3 TX
      ↓
   PC Terminal
