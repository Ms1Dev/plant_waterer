#ifndef CTRL_H
#define CTRL_H

#include <stdbool.h>
#include <stdint.h>

#define POTENTIOMETER ADC1
#define SOIL_SENSOR ADC2
#define IR_SENSOR ADC3
#define IR_LED PB0
#define LED PB1

typedef enum {
    ABOVE_UPPER,
    MIDRANGE,
    BELOW_LOWER
} SoilState;

volatile extern uint8_t upper_limit;
volatile extern uint8_t lower_limit;

void ctrl_init();
bool hasMinWater();
void setLimits();
SoilState getSoilState();
void sleep();

#endif