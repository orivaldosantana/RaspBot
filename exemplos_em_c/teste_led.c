
#include <wiringPi.h>
#include <stdio.h>

#define LedPin 0 // Pin 11 - WiringPi 0 - BCM 17 - GPIO 17
 
int main(void) {
        if(wiringPiSetup() == -1) { //Quando a inicialização do wiringPi falha 
                // uma mensagem é apresentada
                printf("Setup do wiringPi falhou!\n");
                return -1;
        }
 
        pinMode(LedPin, OUTPUT);
        while(1) {
                digitalWrite(LedPin, LOW);   //led off
                printf("led off\n");
                delay(1000);                 // espera  1 sec
                digitalWrite(LedPin, HIGH);  //led on
                printf("led on\n");
                delay(1000);                 // espera 1 sec
        }
        return 0;
}
