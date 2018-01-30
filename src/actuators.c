#include "actuators.h"

static pthread_mutex_t mutexActuators = PTHREAD_MUTEX_INITIALIZER;

Color color = RED;
uchar lumen = 0;

void changeColor() {
    pthread_mutex_lock(&mutexActuators);
    switch(color) {
    case RED:
        color = GREEN;
        break;

    case GREEN:
        color = BLUE;
        break;

    case BLUE:
        color = RED;
        break;

    default:
        printf("changeColor: Color error\n");
    }
    pthread_mutex_unlock(&mutexActuators);
}

void setLightLevel(double level) {
    if(level < 0 || level > 1) {
        fprintf(stderr, "Unacceptable light level value: %f\n", level);
    }

    uchar newVal = (uchar)(100 * level);

    int changed = 0;

    pthread_mutex_lock(&mutexActuators);
    if(lumen-newVal < -1e-2 || lumen-newVal > 1e-2) {
        lumen = newVal;
        changed=1;
    }
    pthread_mutex_unlock(&mutexActuators);

    if(changed) {
        setColor(color);
    }
}

void setColor(Color c) {
    pthread_mutex_lock(&mutexActuators);
    uchar val = lumen;
    pthread_mutex_unlock(&mutexActuators);

    switch(c) {
    case RED:
        ledColorSet(val,100,100);
        break;

    case GREEN:
        ledColorSet(100,val,100);
        break;

    case BLUE:
        ledColorSet(100,100,val);
        break;

    default:
        break;
    }
}

void* updateActuators(void* arg) {
    ledInit();

    Color curColor = color;
    int changed = 1;
    while(1) {
        pthread_mutex_lock(&mutexActuators);
        if(color != curColor) {
            curColor = color;
            changed = 1;
        }
        pthread_mutex_unlock(&mutexActuators);

        if(changed) {
            setColor(curColor);
            changed = 0;
        }

        delay(100);
    }
}

void ledInit(void)
{
    softPwmCreate(LedPinRed,  0, 100);
    softPwmCreate(LedPinGreen,0, 100);
    softPwmCreate(LedPinBlue, 0, 100);
}

void ledColorSet(uchar r_val, uchar g_val, uchar b_val)
{
    softPwmWrite(LedPinRed,   r_val);
    softPwmWrite(LedPinGreen, g_val);
    softPwmWrite(LedPinBlue,  b_val);
}
