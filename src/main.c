#define F_CPU 1000000UL
#include "../inc/ctrl.h"


int main (void) 
{
    ctrl_init();
    
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
        ctrlPump();
        sleep();
    }
    
    return 0;
}

