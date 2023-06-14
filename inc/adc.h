#ifndef ADC_H
#define ADC_H

#include <stdint.h>

typedef enum {
    ADC0 = 0b000000,
    ADC1 = 0b000001,
    ADC2 = 0b000010,
    ADC3 = 0b000011,
    ADC4 = 0b000100
} ADC_Input;

void adc_init();
uint8_t adc_read_prcnt(ADC_Input);
void adc_sleep();

#endif