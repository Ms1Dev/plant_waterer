#include "../inc/ctrl.h"
#include "../inc/adc.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/iotn85.h>
#include <avr/interrupt.h>

volatile uint8_t upper_limit = 90;
volatile uint8_t lower_limit = 50;

void ctrl_init() {
    // turn off timer 1 to save power
    PRR |= (1U << PRTIM1);
    // enable watchdog timer prescaler set for 4 second intervals 
    WDTCR |= (1 << WDIE) | (1 << WDE) | (1<< WDP3);
    
    DDRB |= (1<<PB0) | (1<<PB1);
}

bool hasMinWater() {
    int ir_reading = adc_read_prcnt(IR_SENSOR);
    return (ir_reading > 50);
}

void setLimits() {
    int potentiometer_reading = adc_read_prcnt(POTENTIOMETER);
}

SoilState getSoilState() {
    int soil_reading = adc_read_prcnt(SOIL_SENSOR);
    
    if (soil_reading > upper_limit) {
        return ABOVE_UPPER;
    }
    else if(soil_reading < lower_limit) {
        return BELOW_LOWER;
    }
    else {
        return MIDRANGE;
    }
}

void sleep() {
    MCUCR |= (1U << SE)| (1U << SM1) | (1U << BODS) | (1U << BODSE);
    __asm__ __volatile__ ( "sleep" "\n\t" :: );
    MCUCR &= ~((1U << SE)| (1U << SM1) | (1U << BODS) | (1U << BODSE));
}

ISR(WDT_vect) {
    WDTCR |= (1 << WDIE);
}
