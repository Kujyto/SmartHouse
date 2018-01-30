#include "clap.h"

#define DELAY_SENSORS 20

// TODO: be sure
void* detectClap(void* arg) {
    int soundVal;
    int i;
    int count;
    /*
    while(1) {
        soundVal = analogRead(SOUND_PIN);
        if(soundVal < SOUND_LIMIT) {
            delay(DELAY_DOUBLE);

            for(i=0; i < MAX_TIMINGS; ++i) {
                soundVal = analogRead(SOUND_PIN);
                if(soundVal < SOUND_LIMIT) {
                    printf("Clap! %d\n",soundVal);
                    changeColor();
                    break;
                }
            }

            delay(2000);
        }
    }
    */

    while(1) {
        soundVal = getSoundVal();
        if(soundVal < SOUND_LIMIT) {
            printf("DEBUG clap 1\n");
            delay(2*DELAY_SENSORS);
            soundVal = getSoundVal();
            if(soundVal < SOUND_LIMIT) {
                printf("DEBUG clap 2\n");
                delay(DELAY_DOUBLE);

                count = 0;
                for(i=0; i < MAX_COUNT; ++i) {
                    soundVal = getSoundVal();
                    if(soundVal < SOUND_LIMIT) {
                        printf("DEBUG clap 3\n");
                        count++;
                    }

                    if(count >= MIN_COUNT) {
                        changeColor();
                        break;
                    }

                    delay(DELAY_SENSORS);
                }
            }
        }
    }
}
