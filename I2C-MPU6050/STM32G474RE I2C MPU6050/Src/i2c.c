#include "stm32g4xx.h"   // ✅ correct for STM32G474RE
#include <stdint.h>
#include <stdio.h>

#define GPIOBEN             (1U<<1)
#define I2C1EN              (1U<<21)
// Use STM32's macro { I2C1->CR1 &= ~I2C_CR1_PE } directly instead of { #define I2C_CR1_PE          (1U<<0) }


#define ISR_ISR_BUSY        (1U<<15)
#define CR2_START           (1U<<13)

#define I2C_ISR_TXIS        (1U<<1)  //Even for these warnings we can use STM32's inbuild macros but not using due to confusion
#define I2C_ISR_TC          (1U<<6)

#define I2C_CR2_RD_WRN      (1U<<10)
#define I2C_CR2_STOP        (1U<<14)

/* I2C1_SCL => PA13 & I2C1_SDA => PA14 */

void I2C1_init(void){

	/*Enable clock access to GPIOB*/
	RCC->AHB2ENR |= GPIOBEN;

	/* Set PB8 & PB9 to alternate function */
	GPIOB->MODER &= ~(1U<<16);
	GPIOB->MODER |=  (1U<<17);

	GPIOB->MODER &= ~(1U<<18);
	GPIOB->MODER |=  (1U<<19);

	/*Set PB8 & PB9 output type to open drain */
	GPIOB->OTYPER |= (1U<<8);
	GPIOB->OTYPER |= (1U<<9);


	/* Enable pull ups to PB8 & PB9 */
	GPIOB->PUPDR |=  (1U<<16);
	GPIOB->PUPDR &= ~(1U<<17);

	GPIOB->PUPDR |=  (1U<<18);
	GPIOB->PUPDR &= ~(1U<<19);

	// Set AF4 (0100) for PB8
	GPIOB->AFR[1] &= ~(1U << 0);
	GPIOB->AFR[1] &= ~(1U << 1);
	GPIOB->AFR[1] |=  (1U << 2);
	GPIOB->AFR[1] &= ~(1U << 3);

	// Set AF4 (0100) for PB9
	GPIOB->AFR[1] &= ~(1U << 4);
	GPIOB->AFR[1] &= ~(1U << 5);
	GPIOB->AFR[1] |=  (1U << 6);
	GPIOB->AFR[1] &= ~(1U << 7);


	/*Enable clock access to I2C1*/
	RCC->APB1ENR1 |= I2C1EN;

	/* Enter reset mode */
	RCC->APB1RSTR1 |=  (1U<<21);

	/* Come out of reset mode */
	RCC->APB1RSTR1 &= ~(1U<<21);

	/* Set peripheral clock frequency to 16Mhz (But here no need)*/


	// Disable I2C before changing TIMINGR (Extra but necessary)
	I2C1->CR1 &= ~I2C_CR1_PE;

	/*Set I2C1 to standard mode 100KHz clock */
	I2C1->TIMINGR = 0x10420F13; // From STM32CubeMX timing calculator

	// Re-enable I2C
	I2C1->CR1 |= I2C_CR1_PE;

}


void I2C1_byteRead(char saddr, char maddr, char *data){

	/* 1. Check bus free */
	while(I2C1->ISR & I2C_ISR_BUSY){

	}

	/* Transmit slave address + write + No. of bytes */
	I2C1->CR2 = (saddr << 1) | (1 << 16);

	/* Generate START */
	I2C1->CR2 |= CR2_START;

	/* Wait until START is detected (BUSY=1) */
	while(!(I2C1->ISR & I2C_ISR_BUSY)){

	}


	/* Wait for TXIS (address acknowledged) */
    while(!(I2C1->ISR & I2C_ISR_TXIS)){

    }

	/* Send memory address */
	I2C1->TXDR = maddr;


	/* Generate RE-START */
	//I2C1->CR2 |= CR2_START;

	/* Wait until START is detected (BUSY=1) */
	//while(!(I2C1->ISR & I2C_ISR_BUSY)){

	//}

	/* Wait for TC (transfer complete) */
	while (!(I2C1->ISR & I2C_ISR_TC));

	/* Prepare READ (1 byte)*/
	I2C1->CR2 = (saddr << 1) | I2C_CR2_RD_WRN | (1 << 16);

	/*Generate repeated RE-START*/
	I2C1->CR2 |= I2C_CR2_START;

	/*Wait for data */
	while (!(I2C1->ISR & I2C_ISR_RXNE));

	/* Read and stop*/
	*data = I2C1->RXDR;
	I2C1->CR2 |= I2C_CR2_STOP;

}

void I2C1_burstRead(char saddr, char maddr, int n, char* data) {
    int i = 0;

    /* 1. Wait until bus is free */
    while (I2C1->ISR & I2C_ISR_BUSY);

    /* 2. Set up write to send memory address (maddr) */
    I2C1->CR2 = (saddr << 1)        // 7-bit slave address
              | (1 << 16)           // 1 byte to send (maddr)
              | (0 << 10);          // Write transfer

    /* 3. Generate START */
    I2C1->CR2 |= I2C_CR2_START;

    /* 4. Wait for TXIS = ready to transmit */
    while (!(I2C1->ISR & I2C_ISR_TXIS));

    /* 5. Send memory address */
    I2C1->TXDR = maddr;

    /* 6. Wait for transfer complete */
    while (!(I2C1->ISR & I2C_ISR_TC));

    /* 7. Setup for READ transfer of `n` bytes */
    I2C1->CR2 = (saddr << 1)        // Slave address
              | (n << 16)           // Number of bytes to read
              | I2C_CR2_RD_WRN;     // Read mode

    /* 8. Generate repeated START */
    I2C1->CR2 |= I2C_CR2_START;

    /* 9. Receive data bytes */
    for (i = 0; i < n; i++) {
        while (!(I2C1->ISR & I2C_ISR_RXNE)); // Wait for byte
        data[i] = I2C1->RXDR;
    }

    /* 10. Send STOP condition */
    I2C1->CR2 |= I2C_CR2_STOP;

    /* 11. Wait for STOP flag (optional) */
    while (!(I2C1->ISR & I2C_ISR_STOPF));

    /* Clear STOP flag by writing to it */
    I2C1->ICR |= I2C_ICR_STOPCF;
}
void I2C1_burstWrite(char saddr, char maddr, int n, char* data) {
    int i;

    /* 1. Wait until bus is free */
    while (I2C1->ISR & I2C_ISR_BUSY);

    /* 2. Set up write: slave address + number of bytes (n+1: maddr + data) */
    I2C1->CR2 = (saddr << 1)       // 7-bit address, write mode
              | ((n + 1) << 16);   // n data bytes + 1 memory/register byte

    /* 3. Generate START */
    I2C1->CR2 |= I2C_CR2_START;

    /* 4. Wait for TXIS (ready to transmit) */
    while (!(I2C1->ISR & I2C_ISR_TXIS));

    /* 5. Send memory address */
    I2C1->TXDR = maddr;

    /* 6. Transmit data bytes */
    for (i = 0; i < n; i++) {
        while (!(I2C1->ISR & I2C_ISR_TXIS)); // wait TX ready
        I2C1->TXDR = data[i];
    }

    /* 7. Wait for transfer complete */
    while (!(I2C1->ISR & I2C_ISR_TC));

    /* 8. Generate STOP */
    I2C1->CR2 |= I2C_CR2_STOP;

    /* 9. Wait for STOP and clear it */
    while (!(I2C1->ISR & I2C_ISR_STOPF));
    I2C1->ICR |= I2C_ICR_STOPCF;
}
