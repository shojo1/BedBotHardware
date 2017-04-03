#include <stdio.h>    // Used for printf() statements
#include <wiringPi.h> // Include WiringPi library!
#include <curl/curl.h>

// Pin number declarations. We're using the Broadcom chip pin numbers.
const int pwmPin = 18; // PWM LED - Broadcom pin 18, P1 pin 12
const int ledPin = 23; // Regular LED - Broadcom pin 23, P1 pin 16
const int butPin = 17; // Active-low button - Broadcom pin 17, P1 pin 11

const int pwmValue = 75; // Use this to set an LED brightness

int main(void)
{
    // Setup stuff:
    wiringPiSetupGpio(); // Initialize wiringPi -- using Broadcom pin numbers

    pinMode(butPin, INPUT);      // Set button as INPUT
    //pullUpDnControl(butPin, PUD_UP); // Enable pull-up resistor on button

    printf("gpio is running! Press CTRL+C to quit.\n");

    // Loop (while(1)):
    while(1)
    {
        digitalRead(butPin);
    }

    return 0;
}
