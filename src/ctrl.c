#define F_CPU 1000000L

#include "../inc/ctrl.h"
#include "../inc/adc.h"
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t upper_limit = 90;
volatile uint8_t lower_limit = 50;
volatile bool pumpIsRunning = false;
const uint8_t pumpRunRatio = 3; // ratio of time pump is running e.g. 3 = 1:3 or 4 seconds on and 12 seconds off 
volatile uint8_t pumpRunCounter = 0;

void ctrl_init() {
    // turn off timer 1 and USI to save power
    PRR |= (1 << PRTIM1) | (1 << PRUSI);
    // enable watchdog timer prescaler set for 4 second intervals 
    WDTCSR |= (1 << WDIE) | (1 << WDE) | (1<< WDP3);
    // set leds and pump as outputs
    DDRA |= (1<< LED) | (1<< IR_LED) | (1 << PUMP);
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

void runPump(bool run) {
    pumpIsRunning = run;
}

void _runPump() {
    if (!pumpIsRunning) {
        PORTA &= ~(1 << PUMP);
        return;
    }
    if (++pumpRunCounter >= pumpRunRatio) {
        pumpRunCounter = 0;
        PORTA |= (1 << PUMP);
    }
    else {
        PORTA &= ~(1 << PUMP);
    }
}

void flashLed() {
    PORTA |= (1 << LED);
    _delay_ms(200);
    PORTA &= ~(1 << LED);
}

void sleep() {
    MCUCR |= (1U << SE) | (1U << SM1) | (1U << SM0) | (1U << BODS) | (1U << BODSE);
    __asm__ __volatile__ ( "sleep" "\n\t" :: );
    MCUCR &= ~((1U << SE)| (1U << SM1) | (1U << SM0)  | (1U << BODS) | (1U << BODSE));
}

ISR(WDT_vect) {
    _runPump();
    WDTCSR |= (1 << WDIE);
}
