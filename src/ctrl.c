#define F_CPU 1000000UL

#include "../inc/ctrl.h"
#include "../inc/adc.h"
#include <util/delay.h>
#include <avr/interrupt.h>

#define SOIL_SENSOR_OFFSET_UPPER 520
#define SOIL_SENSOR_OFFSET_LOWER 180

#define MAX_MOISTURE_PRCNT 90
#define MIN_MOISTURE_PRCNT 10
#define MOISTURE_PRCNT_GAP 30

volatile uint8_t upper_limit = 90;
volatile uint8_t lower_limit = 50;
volatile bool pumpIsRunning = false;
const uint8_t pumpCycles = 10;
const uint8_t pumpOnCycles = 5;
volatile uint8_t pumpRunCounter = 0;

void ctrl_init() 
{
    _delay_ms(500);
    adc_init();
    // turn off timer 1 and USI to save power
    PRR |= (1 << PRTIM1) | (1 << PRUSI);
    // enable watchdog timer prescaler set for 4 second intervals 
    WDTCSR |= (1 << WDIE) | (1 << WDE) | (1<< WDP3);
    // set outputs
    DDRA |= (1<< LED) | (1<< IR_LED) | (1 << PUMP);
    DDRB |= (1 << SOIL_SENSOR_PWR);
    // flash LED twice to signal power on
    PORTA |= (1 << LED);
    _delay_ms(100);
    PORTA &= ~(1 << LED);
    _delay_ms(100);
    PORTA |= (1 << LED);
    _delay_ms(100);
    PORTA &= ~(1 << LED);

    sei();
}

bool hasMinWater() 
{
    // turn on IR led
    PORTA |= (1 << IR_LED);
    // delay to allow led to reach brightness
    _delay_ms(1);
    // take reading from phototransistor
    uint8_t ir_reading = adc_read_prcnt(IR_SENSOR);
    // turn off IR led
    PORTA &= ~(1 << IR_LED);

    return (ir_reading > 50);
}

void setLimits() 
{
    uint8_t potentiometer_reading = adc_read_prcnt(POTENTIOMETER);
    upper_limit = potentiometer_reading;
    if (upper_limit > MAX_MOISTURE_PRCNT) {
        upper_limit = MAX_MOISTURE_PRCNT;
    }
    lower_limit = potentiometer_reading - MOISTURE_PRCNT_GAP;
    if (lower_limit < MIN_MOISTURE_PRCNT) {
        lower_limit = MIN_MOISTURE_PRCNT;
    }
}

SoilState getSoilState() 
{
    // switch on soil sensor
    PORTB |= (1 << SOIL_SENSOR_PWR);
    // let it settle
    _delay_ms(200);
    // take reading mapped to readings taken in air and in water
    uint8_t soil_reading = mapped_adc_prcnt(SOIL_SENSOR, SOIL_SENSOR_OFFSET_UPPER, SOIL_SENSOR_OFFSET_LOWER);
    // switch off the sensor
    PORTB &= ~(1 << SOIL_SENSOR_PWR);
    // sensor reads higher in air so invert the result so it makes more sense
    uint8_t inverted_result = 100 - soil_reading;
    
    if (inverted_result > upper_limit) {
        return ABOVE_UPPER;
    }
    else if(inverted_result < lower_limit) {
        return BELOW_LOWER;
    }
    else {
        return MIDRANGE;
    }
}

void runPump(bool run) 
{
    pumpIsRunning = run;
    pumpRunCounter = 0;
}

void ctrlPump() 
{   
    // pump operation affects ADC so stay in this while loop while running     
    while(pumpIsRunning && pumpRunCounter < pumpOnCycles) {
        PORTA |= (1 << PUMP);
    }
    PORTA &= ~(1 << PUMP);
}

void flashLed() 
{
    PORTA |= (1 << LED);
    _delay_ms(200);
    PORTA &= ~(1 << LED);
}

void sleep() 
{
    // standby mode
    MCUCR |= (1U << SE) | (1U << SM1) | (1U << SM0) | (1U << BODS) | (1U << BODSE);
    __asm__ __volatile__ ( "sleep" "\n\t" :: );
    MCUCR &= ~((1U << SE)| (1U << SM1) | (1U << SM0) | (1U << BODS) | (1U << BODSE));
}

ISR(WDT_vect) 
{
    if (++pumpRunCounter >= pumpCycles) {
        pumpRunCounter = 0;
    }
    WDTCSR |= (1 << WDIE);
}
