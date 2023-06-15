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

/**
 * Configure ADC control register for operation
*/
void adc_init();

/**
 * Gets the digital result from the given ADC input and converts to a percentage
 * Returns result from ADC as a percentage
*/
uint8_t adc_read_prcnt(ADC_Input adc_input);

/**
 * Gets the digital result from the given ADC input and maps it to the upper and lower parameters
 * Returns the mapped result as a percentage
*/
uint8_t mapped_adc_prcnt(ADC_Input adc_input ,int upper, int lower);

/**
 * Puts the MCU into ADC noise reduction sleep mode
*/
void adc_sleep();


#endif //ADC_H