
#include"uart.h"



#define GPIOAEN       (1U<<0)
#define UART2EN       (1U<<17)

#define CR1_TE        (1U<<3)
#define CR1_RE        (1U<<2)

#define CR1_UE        (1U<<0)
#define ISR_TXE       (1U<<7)
#define ISR_RXNE      (1U<<5)


#define SYS_FREQ      16000000
#define APB1_CLK      SYS_FREQ

#define UART_BAUDRATE     115200

static void uart_set_baudrate(USART_TypeDef *USARTx,uint32_t PeriphClk,  uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);

void usart2_write(int ch);

int __io_putchar(int ch){
	usart2_write(ch);
	return ch;
}



void uart2_rxtx_init(void){
	/**************** Configure UART GPIO pin ****************/
	/*Enable clock access to gpioa */
	RCC->AHB2ENR |= GPIOAEN;

	/*Set PA2 mode to Alternate Function mode */
	GPIOA->MODER &= ~(1U<<4);
	GPIOA->MODER |=  (1U<<5);

	/*Set PA2 Alternate function type to UART_TX (AF7)*/
	GPIOA->AFR[0]  |=  (1U<<8);
	GPIOA->AFR[0]  |=  (1U<<9);
	GPIOA->AFR[0]  |=  (1U<<10);
	GPIOA->AFR[0]  &= ~(1U<<11);

	/*Set PA3 mode to Alternate Function mode */
	GPIOA->MODER &= ~(1U<<6);
	GPIOA->MODER |=  (1U<<7);

	/*Set PA3 Alternate function type to UART_RX (AF7)*/
	GPIOA->AFR[0]  |=  (1U<<12);
	GPIOA->AFR[0]  |=  (1U<<13);
	GPIOA->AFR[0]  |=  (1U<<14);
	GPIOA->AFR[0]  &= ~(1U<<15);





	/**************** Configure UART module ****************/
	/*Enable clock access to UART2*/
	RCC->APB1ENR1 |= UART2EN;

	/*Configure baudrate*/
	uart_set_baudrate(USART2,APB1_CLK,UART_BAUDRATE);

	/*Configure the transfer direction*/
	USART2->CR1 = (CR1_TE | CR1_RE);

	/*Enable uart module*/
	USART2->CR1 |= CR1_UE;
}

char uart2_read(void){
	/*Make sure receive data register is not empty*/
	while(!(USART2->ISR & ISR_RXNE)){
			__NOP();

		}

	/*Read Data*/
    return (char)(USART2->RDR & 0xFF);

}


void usart2_write(int ch){
	/*Make sure transmit data register is empty*/
	while(!(USART2->ISR & ISR_TXE)){
		__NOP();

	}

	/*Write to transmit data register*/
	USART2->TDR = (ch & 0xFF);

}



static void uart_set_baudrate(USART_TypeDef *USARTx,uint32_t PeriphClk,  uint32_t BaudRate){
	USARTx->BRR = compute_uart_bd(PeriphClk,BaudRate );
}


static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate){
	return ((PeriphClk + (BaudRate/2U)) / BaudRate);
}

