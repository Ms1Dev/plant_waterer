#define F_CPU 1000000L

#include <util/delay.h>
#include <avr/io.h>
#include <avr/iotn84.h>
#include <avr/interrupt.h>
#include "../inc/adc.h"
#include "../inc/ctrl.h"


int main (void) 
{
    ctrl_init();
    adc_init();

    while(1) {
        if (hasMinWater()) {
            setLimits();
            SoilState soilState = getSoilState();

            if (soilState == BELOW_LOWER && !pumpIsRunning) {
                runPump(true);
            }
            else if (soilState == ABOVE_UPPER && pumpIsRunning) {
                runPump(false);
            }
        }
        else {
            runPump(false);
            flashLed();
        }
        sleep();
    }
    
    return 0;
}

