

#define CORE_CM7
#include "stm32h7xx.h"
#include "adc.h"


#define ADC1EN         (1U<<5)
#define GPIOCEN        (1U<<2)
#define ADC_CH1        (1U<<6)
#define ADC_SEQ_LEN_1    0x00
#define CR_ADEN        (1U<<0)
#define CR_ADSTART     (1U<<2)
#define ISR_EOC        (1U<<2)

#define IER_EOCIE      (1U<<2)



void pc0_adc_interrupt_init(void){

    /* Enable clock access to GPIOC */
    RCC->AHB4ENR |= GPIOCEN;

    /* Set PC0 to analog mode */
    GPIOC->MODER &= ~(3U<<0);   // clear bits
    GPIOC->MODER |=  (3U<<0);   // analog mode (11)

    /* Enable ADC clock */
    RCC->AHB1ENR |= ADC1EN;

    /* Enable ADC end-of-conversion interrupt*/
    ADC1->IER |= IER_EOCIE;

    /*Enable ADC interrupt in NVIC */
    NVIC_EnableIRQ(ADC_IRQn);

    /* Conversion sequence: 1 conversion, channel 1 (PC0) */
    ADC1->SQR1 = (0U<<00) | (1U << 6);

    /* Enable ADC */
    ADC1->CR |= CR_ADEN;
}




void pc0_adc_init(void){

    /* Enable clock access to GPIOC */
    RCC->AHB4ENR |= GPIOCEN;

    /* Set PC0 to analog mode */
    GPIOC->MODER &= ~(3U<<0);   // clear bits
    GPIOC->MODER |=  (3U<<0);   // analog mode (11)

    /* Enable ADC clock */
    RCC->AHB1ENR |= ADC1EN;

    /* Conversion sequence: 1 conversion, channel 1 (PC0) */
    ADC1->SQR1 = (0U<<00) | (1U << 6);

    /* Enable ADC */
    ADC1->CR |= CR_ADEN;
}


void start_conversion(void){
	/*Start ADC conversion */
	ADC1->CR |= CR_ADSTART;

}

uint32_t adc_read(void){

	/* Wait for the concersion to be complete {We check flag here "EOC flag" } */
	while(!(ADC1->ISR & ISR_EOC)){
		// Wait until conversion is complete {Loop will execute when data is not converted}
	}

	/* Read the converted result */
	return (ADC1->DR); //Converted data is stored in Data register


}
