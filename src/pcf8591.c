#include <stdio.h>
#include <wiringPi.h>
#include <pcf8591.h>

#define PCF       120
#define SOUND_PIN PCF+1
#define LIGHT_PIN PCF+0

int main (void)
{
    int sound,lumen;
	wiringPiSetup ();
	// Setup pcf8591 on base pin 120, and address 0x48
	pcf8591Setup (PCF, 0x48);
	while(1) // loop forever
	{
		sound = analogRead  (SOUND_PIN);
                lumen = analogRead(LIGHT_PIN);
                //printf("%d\n", value);
		if (sound < 20){
                    printf("Lumen: %d (sound %d)\n", lumen, sound);
		}
	}
	return 0;
}
