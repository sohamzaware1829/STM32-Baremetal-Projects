
#ifndef UART_H_
#define UART_H_
#include <stdint.h>
#include "stm32h7xx.h"


void uart3_tx_init(void);
void uart3_write(int ch);
void uart3_rxtx_init(void);
char uart3_read(void);




#endif /* UART_H_ */
