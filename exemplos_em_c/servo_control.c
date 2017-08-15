// http://www.admfactory.com/rgb-led-on-raspberry-pi-using-c/
// https://pinout.xyz/pinout/wiringpi 
// https://stackoverflow.com/questions/20081286/controlling-a-servo-with-raspberry-pi-using-the-hardware-pwm-with-wiringpi
// https://projects.drogon.net/raspberry-pi/wiringpi/software-pwm-library/

/*
 * Compilando: 
 * gcc servo_control.c -o servo -lwiringPi  -lpthread
 */

#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>

 
#define servoRightPin 26 // BCM 13
#define servoLeftPin 23 // BCM 12

int minValue;
int pwmRange;
int maxValue; 
    
int setupRobot() {
	if(wiringPiSetup() == -1) {  
			printf("Setup do wiringPi falhou!\n");
			return -1;
	}
    minValue = 25; 
 	pwmRange= 125;
	maxValue = pwmRange; 
 
	// Servo motor direito 
	softPwmCreate(servoRightPin,minValue,pwmRange);
	pinMode(servoRightPin, PWM_OUTPUT);
	// servo motor esquerdo 
	softPwmCreate(servoLeftPin,minValue,pwmRange);
	pinMode(servoLeftPin, PWM_OUTPUT);
	
	pwmSetMode(PWM_MODE_MS); 
	pwmSetClock(384);  
	return 0;
}

/** 
 * Recebe valores entre um máximo e um mínimo 
 */
int turnOnServos(int _left, int _right ) {
	int rightZero = 75;
	int leftZero = 83; 
	pwmWrite(servoLeftPin, leftZero - _left); 
	
	pwmWrite(servoRightPin,rightZero +  _right); 
	
	return 0; 
}

void auxServoCalibrating(int _servoPin) {
	int testStep = 5;
	int timeDelay = 500; 
	int i;
	for (i=minValue; i < maxValue; i += testStep ) {
		pwmWrite(_servoPin, i);
		printf("V %i\n",i);
		delay(timeDelay); 
	}
	for (i = maxValue; i > minValue; i -= testStep) {
		pwmWrite(_servoPin, i);
		printf("V %i\n",i);
		delay(timeDelay); 
	} 
}

void testeCalibration() {
	auxServoCalibrating(servoRightPin);
	pwmWrite(servoRightPin,75); 
				
	auxServoCalibrating(servoLeftPin);	 
	pwmWrite(servoLeftPin, 83);    

}

void goForward() {
	
	turnOnServos(10,10); 
}

void goBackward() {
	turnOnServos(-10,-10); 
}
 
int main(void) {

    if(setupRobot() != -1) {  
 
		 
		goForward(); 
	}
 
}
