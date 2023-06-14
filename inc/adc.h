#ifndef ADC_H
#define ADC_H

#include <stdint.h>

typedef enum {
    ADC0 = 0b0000,
    ADC1 = 0b0001,
    ADC2 = 0b0010,
    ADC3 = 0b0011
} ADC_Input;

void adc_init();
uint8_t adc_read_prcnt(ADC_Input);
void adc_sleep();

#endif