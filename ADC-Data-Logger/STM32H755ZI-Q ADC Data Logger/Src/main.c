/* Here I used ADC1 channel 1 (PC0) to sample analog values and send it to console pc via uart */
#define CORE_CM7

#include <stdio.h>
#include <stdint.h>
#include "stm32h7xx.h"
#include "uart.h"
#include "adc.h"

static void adc_callback(void);

//#include "core_cm7.h"  // Usually included via stm32h7xx.h


uint32_t sensor_value;

int main(void){

	uart3_tx_init();
	pc0_adc_interrupt_init();

	while(1){

	}

}

static void adc_callback(void){
	start_conversion();
	sensor_value = ADC1->DR;
	printf("Sensor value is: %lu\r\n", (unsigned long)sensor_value);
	for(volatile int i=0; i<10000; i++); // delay

}

void ADC_IRQHandler(void){
	//Check EOC in ISR reg.
	if((ADC1->ISR & ISR_EOC) != 0){  //When bit is set by EOC then enter loop

		/* Clear EOC after interrupt */
		ADC1->ISR &= ~ISR_EOC;
		// Now do something
		adc_callback();


	}

}
