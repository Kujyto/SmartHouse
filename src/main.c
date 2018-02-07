#include "definition.h"

#include "sensorsManager.h"
#include "networkInterface.h"

pthread_t tid[4];

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Usage: %s <ip of server> [debug]\n", argv[0]);
        return 1;
    }

    int err;

    if(wiringPiSetup() == -1){ //when initialize wiring failed, print message to screen
        printf("setup wiringPi failed !");
        return 1;
    }
    pcf8591Setup(PCF, 0x48);

    setServerAddr(argv[1]);

    ledInit();

    err = pthread_create(tid+0, NULL, &sensorsManager, NULL);
//    err = pthread_create(tid+1, NULL, &updateActuators, NULL);
    err = pthread_create(tid+3, NULL, &networkManager, NULL);


    if(argc > 2) {
        while(1) {
            printf("SoundVal: %d\n", getSoundVal());
            printf("LumenVal: %d\n", getLumenVal());
            printf("Humidity: %f\n", getHumidity());
            printf("Temperature: %f\n", getTemperature());
            printf("\n");

            delay(2000);
        }
    }

    pthread_join(tid[0],NULL);

    return 0;
}
