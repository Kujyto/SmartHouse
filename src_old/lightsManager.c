#include "lightsManager.h"

int preLightPercent = 0;
void* lightsManager(void* arg) {
    while(1) {
        int lumenVal = getLumenVal();
        int lightPercent = 100 * (lumenVal-LIGHT_MIN) / (LIGHT_MAX - LIGHT_MIN);

        if(lightPercent < 1) {
            lightPercent = 1;
        }
        else if(lightPercent > 100) {
            lightPercent = 100;
        }

        setLightToPercent(lightPercent);

        delay(DELAY_LIGHTS);
    }
}


void setLightToPercent(int percent) {
    while(preLightPercent != percent) {
            if(preLightPercent > percent)
                preLightPercent--;
            else
                preLightPercent++;

            setLightLevel((100-preLightPercent) / 100.0);
            delay(LIGHT_TRANSITION);
        }
}
