/*
 * adc.h
 *
 *  Created on: Sep 24, 2025
 *      Author: soham
 */

#ifndef ADC_H_
#define ADC_H_


#include <stdint.h>
void pc0_adc_init(void);
uint32_t adc_read(void);
void start_conversion(void);
void pc0_adc_interrupt_init(void);

#define ISR_EOC        (1U<<2)




#endif /* ADC_H_ */
