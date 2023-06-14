#define F_CPU 8000000L

#include <util/delay.h>
#include <avr/io.h>
#include <avr/iotn85.h>
#include <avr/interrupt.h>
#include "../inc/adc.h"
#include "../inc/ctrl.h"


int main (void) {
    ctrl_init();
    adc_init();

    while(1) {
        // analyse water level float sensor
        // if water above level
            // analyse variable resistor pin for setting
            // convert reading into moisture percentage limits
            // analyse soil moisture level
            // if below lower limit && watering false
                // set watering to true
            // else if above upper limit && watering true
                // set watering to false
            // if watering true
                // run pump
            // else
                // stop pump
        // else
            // flash LED

        
        SoilState soilState = getSoilState();

        if (soilState == ABOVE_UPPER) {
            PORTB |= (1<<LED);
        }
        else {
            PORTB &= ~(1<<LED);
        }

        sleep();
    }
    
    return 0;
}

