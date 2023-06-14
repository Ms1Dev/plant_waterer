#ifndef CTRL_H
#define CTRL_H

#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/iotn84.h>
#include "adc.h"

#define POTENTIOMETER ADC1
#define SOIL_SENSOR ADC2
#define IR_SENSOR ADC3
#define IR_LED PA5
#define LED PA6
#define PUMP PA7

typedef enum {
    ABOVE_UPPER,
    MIDRANGE,
    BELOW_LOWER
} SoilState;

volatile extern uint8_t upper_limit;
volatile extern uint8_t lower_limit;
volatile extern bool pumpIsRunning;

void ctrl_init();
bool hasMinWater();
void setLimits();
SoilState getSoilState();
void sleep();
void flashLed();
void runPump(bool);

#endif