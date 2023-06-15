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
#define SOIL_SENSOR_PWR PB2

typedef enum {
    ABOVE_UPPER,
    MIDRANGE,
    BELOW_LOWER
} SoilState;

volatile extern uint8_t upper_limit;
volatile extern uint8_t lower_limit;
volatile extern bool pumpIsRunning;

/**
 * Initialises the ADC and configures registers for controlling peripherals
*/
void ctrl_init();

/**
 * Reads the IR break beam sensor to check the water level is above minimum
 * Returns true if sufficient water
*/
bool hasMinWater();

/**
 * Reads the Potentiometer and sets the upper and lower moisture limits based off the reading
*/
void setLimits();

/**
 * Reads the soil moisture sensor and compares the result with the upper and lower moisture limits
 * Returns the state of the soil based on this comparison
*/
SoilState getSoilState();

/**
 * Puts the MCU into standby sleep mode
*/
void sleep();

/**
 * Flashes the indication LED once
*/
void flashLed();

/**
 * Sets a flag to allow running the water pump and resets the pump timer 
*/
void runPump(bool);

/**
 * Runs the pump based on the running flag and current timer/counter value
*/
void ctrlPump();

#endif