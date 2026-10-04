#define CORE_CM7
#include "stm32h7xx.h"
#include "uart.h"

#define GPIODEN          (1U<<3)
#define UART3EN          (1U<<18)

#define CR1_TE           (1U<<3)
#define CR1_RE           (1U<<2)

#define CR1_UE           (1U<<0)
#define ISR_TXFNF        (1U<<7)
#define ISR_RXFNE        (1U<<5)



#define SYS_FREQ         64000000U
#define APB1_CLK         SYS_FREQ
#define UART_BAUDRATE    115200U

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint32_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);

void uart3_write(int ch);


int __io_putchar(int ch){
	uart3_write(ch);
	return ch;
}

void uart3_rxtx_init(void){
	/********** Configure uart gpiod **********/

	/* Enable clock access to gpiod */
	RCC->AHB4ENR |= GPIODEN;

	/* Set PD8 mode to alternate function mode */
	GPIOD->MODER &= ~(1U<<16);
	GPIOD->MODER |=  (1U<<17);

	/* Set PD8 alternate function type to UART_TX (AF7) */
	GPIOD->AFR[1] |=  (1U<<0);
	GPIOD->AFR[1] |=  (1U<<1);
	GPIOD->AFR[1] |=  (1U<<2);
	GPIOD->AFR[1] &= ~(1U<<3);

	/* Set PD9 mode to alternate function mode */
	GPIOD->MODER &= ~(1U<<18);
	GPIOD->MODER |=  (1U<<19);

	/* Set PD9 alternate function type to UART_RX (AF7) */
	GPIOD->AFR[1] |=  (1U<<4);
    GPIOD->AFR[1] |=  (1U<<5);
	GPIOD->AFR[1] |=  (1U<<6);
	GPIOD->AFR[1] &= ~(1U<<7);



	/********** Configure uart module **********/

	/* Enable clock access to uart3 */
	RCC->APB1LENR |= UART3EN;

	/* Configure baudrate */
	uart_set_baudrate(USART3, APB1_CLK, UART_BAUDRATE);

	/*Configure transfer direction*/
	USART3->CR1 = (CR1_TE | CR1_RE);  //Everything here becomes 0 and only TE bitno.3 becomes 1
	// Even here I haven't used CR1's & CR2's other bits and thatswhy the data frame would only contain default frame{1-start,8-data,n-stop bits(n stop bits can be defined in CR2)}

	/*Enable uart module */
	USART3->CR1 |= CR1_UE;


}


void uart3_tx_init(void){
	/********** Configure uart gpiod **********/

	/* Enable clock access to gpiod */
	RCC->AHB4ENR |= GPIODEN;

	/* Set PD8 mode to alternate function mode */
	GPIOD->MODER &= ~(1U<<16);
	GPIOD->MODER |=  (1U<<17);

	/* Set PD8 alternate function type to UART_TX (AF7) */
	GPIOD->AFR[1] |=  (1U<<0);
	GPIOD->AFR[1] |=  (1U<<1);
	GPIOD->AFR[1] |=  (1U<<2);
	GPIOD->AFR[1] &= ~(1U<<3);

	/********** Configure uart module **********/

	/* Enable clock access to uart3 */
	RCC->APB1LENR |= UART3EN;

	/* Configure baudrate */
	uart_set_baudrate(USART3, APB1_CLK, UART_BAUDRATE);

	/*Configure transfer direction*/
	USART3->CR1 = CR1_TE;  //Everything here becomes 0 and only TE bitno.3 becomes 1
	// Even here I haven't used CR1's & CR2's other bits and thatswhy the data frame would only contain default frame{1-start,8-data,n-stop bits(n stop bits can be defined in CR2)}

	/*Enable uart module */
	USART3->CR1 |= CR1_UE;


}

char uart3_read(void){
	/*Make sure receive data register is not empty, when USART RXDR is not empty interrupt is generated*/
	while(!(USART3->ISR & ISR_RXFNE)){
		__NOP();

	}
	/*Read data*/
	return USART3->RDR;

}


void uart3_write(int ch){
	/*Make sure transmit data register is empty, when USART TXDR is empty interrupt is generated */
	while(!(USART3->ISR & ISR_TXFNF)){
		__NOP();
	}

	/*Write to transmit data register*/
	USART3->TDR = (ch & 0xFF);
}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate){
	USARTx->BRR = compute_uart_bd(PeriphClk, BaudRate);
}


static uint32_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate){
	return((PeriphClk + (BaudRate/2U))/BaudRate);

}





