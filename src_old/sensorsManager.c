#include "sensorsManager.h"

int soundVal = 0;
int lumenVal = 0;

double humidity = 0.0;
double temperature = 20.0;

int dht11_dat[5] = {0,0,0,0,0};

static pthread_mutex_t mutexSensors = PTHREAD_MUTEX_INITIALIZER;

pthread_t tid[3];

void* sensorsManager(void* arg) {
    int err;

    err = pthread_create(tid+0, NULL, &humitureManager, NULL);
    err = pthread_create(tid+1, NULL, &lumenManager, NULL);

    pthread_join(tid[1],NULL);
}

int getSoundVal() {
    int v;
    pthread_mutex_lock(&mutexSensors);
    v = soundVal;
    pthread_mutex_unlock(&mutexSensors);
    return v;
}

int getLumenVal() {
    int v;
    pthread_mutex_lock(&mutexSensors);
    v = lumenVal;
    pthread_mutex_unlock(&mutexSensors);
    return v;
}

double getHumidity() {
    double v;
    pthread_mutex_lock(&mutexSensors);
    v = humidity;
    pthread_mutex_unlock(&mutexSensors);
    return v;
}

double getTemperature() {
    double v;
    pthread_mutex_lock(&mutexSensors);
    v = temperature;
    pthread_mutex_unlock(&mutexSensors);
    return v;
}

void* lumenManager(void* arg) {
    while(1) {
        pthread_mutex_lock(&mutexSensors);
        lumenVal = analogRead(LUMEN_PIN);
        pthread_mutex_unlock(&mutexSensors);

        //printf("lumen=%d\n", lumenVal);

        delay(DELAY_LUMEN);
    }
}

void* humitureManager(void* arg) {
    while(1) {
        read_dht11_dat();
        //printf("temp= %2.1f\n", getTemperature());
        //printf("humidity=%2.1f\%\n", getHumidity());
        delay(DELAY_HUMITURE);
    }
}


void read_dht11_dat()
{
	uint8_t laststate = HIGH;
	uint8_t counter = 0;
	uint8_t j = 0, i;
	float f; // fahrenheit

	dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

        pthread_mutex_lock(&mutexSensors);

	// pull pin down for 18 milliseconds
	pinMode(HumiturePin, OUTPUT);
	digitalWrite(HumiturePin, LOW);
	delay(18);
	// then pull it up for 40 microseconds
	digitalWrite(HumiturePin, HIGH);
	delayMicroseconds(40);
	// prepare to read the pin
	pinMode(HumiturePin, INPUT);

	// detect change and read data
	for ( i=0; i< MAXTIMINGS; i++) {
		counter = 0;
		while (digitalRead(HumiturePin) == laststate) {
			counter++;
			delayMicroseconds(1);
			if (counter == 255) {
				break;
			}
		}
		laststate = digitalRead(HumiturePin);

		if (counter == 255) break;

		// ignore first 3 transitions
		if ((i >= 4) && (i%2 == 0)) {
			// shove each bit into the storage bytes
			dht11_dat[j/8] <<= 1;
			if (counter > 16)
				dht11_dat[j/8] |= 1;
			j++;
		}
	}

	// check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
	// print it out if data is good
	if ((j >= 40) &&
			(dht11_dat[4] == ((dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF)) ) {
            //f = dht11_dat[2] * 9. / 5. + 32;
            //printf("Humidity = %d.%d %% Temperature = %d.%d *C (%.1f *F)\n", dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f);

            temperature = dht11_dat[3];
            temperature /= 100;
            temperature += dht11_dat[2]; // not sure

            humidity = dht11_dat[1];
            humidity /= 100;
            humidity += dht11_dat[0];

	}
        pthread_mutex_unlock(&mutexSensors);
}
