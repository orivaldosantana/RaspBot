// http://www.admfactory.com/rgb-led-on-raspberry-pi-using-c/
// https://pinout.xyz/pinout/wiringpi 


#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>

#define ServoPin 1 // GPIO 18

 
int main(void) {
        if(wiringPiSetup() == -1) { //Quando a inicialização do wiringPi falha 
                // uma mensagem é apresentada
                printf("Setup do wiringPi falhou!\n");
                return -1;
        }
		int angle = 0; 
		int maxValue = 800; 
        softPwmCreate(ServoPin,0,maxValue);
        while(1) {
				angle += 1; 
                if (angle > maxValue) {
					angle = 0; 
				}			
                softPwmWrite(ServoPin, angle);    
                printf("Angle: %i\n",angle);
                delay(3000);  

                      
        }
        return 0;
}
