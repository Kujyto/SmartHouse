#include "definition.h"

#include "clap.h"
#include "actuators.h"
#include "lightsManager.h"
#include "networkInterface.h"

pthread_t tid[4];

int main() {
    int err;

    if(wiringPiSetup() == -1){ //when initialize wiring failed, print message to screen
        printf("setup wiringPi failed !");
        return 1;
    }
    pcf8591Setup(PCF, 0x48);


    err = pthread_create(tid+0, NULL, &sensorsManager, NULL);
    err = pthread_create(tid+1, NULL, &updateActuators, NULL);
    err = pthread_create(tid+2, NULL, &lightsManager, NULL);
    err = pthread_create(tid+3, NULL, &networkManager, NULL);
    //err = pthread_create(tid+..., NULL, &detectClap, NULL); // does not work

    int v;
    while(1) {
        printf("Value: ");
        scanf("%d", &v);

        dualSetColor(v);

        if(v < -1)
            break;
    }

    return 0;
}
