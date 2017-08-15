// http://www.admfactory.com/rgb-led-on-raspberry-pi-using-c/
// https://pinout.xyz/pinout/wiringpi 
// https://stackoverflow.com/questions/20081286/controlling-a-servo-with-raspberry-pi-using-the-hardware-pwm-with-wiringpi
// https://projects.drogon.net/raspberry-pi/wiringpi/software-pwm-library/

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
    int minValue = 25; 
	int v = minValue;
 	int pwmRange= 125;
	int maxValue = pwmRange; 
    softPwmCreate(ServoPin,minValue,pwmRange);
	pinMode(ServoPin, PWM_OUTPUT);
	pwmSetMode(PWM_MODE_MS); 
	pwmSetClock(384); //clock at 50kHz (20us tick)
	 
    
	while(1) {
		if (v > maxValue) {
			v = minValue; 
		}			
		pwmWrite(ServoPin, v);    
		printf("V %i\n",v);
		delay(1000); 
		v += 5;  
	}
	return 0;
}
