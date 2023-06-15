#define F_CPU 1000000UL

#include "../inc/adc.h"
#include <avr/io.h>
#include <avr/iotn84.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void adc_init() 
{
    // prescale clock by div factor 8 and enable adc
    ADCSRA |= (1<<ADPS1) | (1<<ADPS0) | (1<<ADEN);
    sei();
}

int _adc_read(ADC_Input adc_input) 
{
    // clear channel select
    ADMUX &= 0xF0;
    // set the adc channel
    ADMUX |= adc_input;
    // start conversion
    ADCSRA |=  (1 << ADIE);
    // while conversion not complete
    adc_sleep();
    // store low register first
    int result = ADCL;
    // or the result with the high register shifted 8 bits
    result |= (ADCH << 8);

    return result;
}

uint8_t mapped_adc_prcnt(ADC_Input adc_input, int upper, int lower) 
{
    int adc_reading = _adc_read(adc_input);
    double range = (upper - lower) * 1.0;
    double ratio = (adc_reading - lower) / range;
    double as_prcnt = ratio * 100;
    as_prcnt += 0.5;
    return (uint8_t) as_prcnt;
}

uint8_t adc_read_prcnt(ADC_Input adc_input) 
{
    int adc_reading = _adc_read(adc_input);
    double as_prcnt = 100.0 / 1024;
    as_prcnt *= adc_reading;
    as_prcnt += 0.5;
    return (uint8_t) as_prcnt;
}

void adc_sleep() 
{
    MCUCR |= (1U << SE)| (1U << SM0);
    __asm__ __volatile__ ( "sleep" "\n\t" :: );
    MCUCR &= ~((1U << SE)| (1U << SM0));
    _delay_ms(1);
}

ISR(ADC_vect) {}