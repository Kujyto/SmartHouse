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

void setLightTransition(double level) {
    pthread_mutex_lock(&mutexActuators);
    int curLumen = lumen;
    pthread_mutex_unlock(&mutexActuators);

    int target = 100 * level;
    while(curLumen != target) {
        if(curLumen < target)
            curLumen++;
        else
            curLumen--;

        setLightLevel(curLumen / 100.0);
        delay(LIGHT_TRANSITION);
    }
}

void setLightLevel(double level) {
    if(level < 0 || level > 1) {
        fprintf(stderr, "Unacceptable light level value: %f\n", level);
    }

    uchar newVal = (uchar)(100 * level);

    int changed = 0;

    pthread_mutex_lock(&mutexActuators);
    lumen = newVal;
    pthread_mutex_unlock(&mutexActuators);

    setColor(color,newVal);
}

void setColor(Color c, uchar l) {
    switch(c) {
    case RED:
        ledColorSet(l,100,100);
        break;

    case GREEN:
        ledColorSet(100,l,100);
        break;

    case BLUE:
        ledColorSet(100,100,l);
        break;

    default:
        break;
    }
}

void dualSetColor(int i) {
    if(i < 0) { // green
	softPwmWrite(DualGreenPin, 100);
        softPwmWrite(DualRedPin, 0);
    }
    else if(i > 0) { // red
        softPwmWrite(DualGreenPin, 0);
        softPwmWrite(DualRedPin, 100);
    }
    else { // no color
        softPwmWrite(DualGreenPin, 0);
        softPwmWrite(DualRedPin, 0);
    }
}

void* updateActuators(void* arg) {
    ledInit();

    Color curColor = color;
    uchar curLumen = lumen;
    int changed = 1;
    while(1) {
        pthread_mutex_lock(&mutexActuators);
        if(color != curColor) {
            curColor = color;
            curLumen = lumen;
            changed = 1;
        }
        pthread_mutex_unlock(&mutexActuators);

        if(changed) {
            setColor(curColor,curLumen);
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

    softPwmCreate(DualRedPin, 0, 100);
    softPwmCreate(DualGreenPin, 0, 100);
}

void ledColorSet(uchar r_val, uchar g_val, uchar b_val)
{
    softPwmWrite(LedPinRed,   r_val);
    softPwmWrite(LedPinGreen, g_val);
    softPwmWrite(LedPinBlue,  b_val);
}
