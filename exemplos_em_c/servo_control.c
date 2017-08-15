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

int vel; // velocidade do motor 
    
int setupRobot() {
	if(wiringPiSetup() == -1) {  
			printf("Setup do wiringPi falhou!\n");
			return -1;
	}
    minValue = 25; 
 	pwmRange= 125;
	maxValue = pwmRange; 
 
	vel = 20;
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
	int leftZero = 82; 
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
	
	turnOnServos(vel,vel); 
}

void stop() {
	
	turnOnServos(0,0); 
}


void goBackward() {
	turnOnServos(-vel,-vel); 
}

void turnLeft(){
	turnOnServos(vel,-vel); 
}
 
void turnRight(){
	turnOnServos(-vel,vel); 
}
 

void execComand(char *command) {
		if ( strcmp(command, "b") == 0 ) {
			goBackward(); 
		}
		else if ( strcmp(command, "f") == 0 ) {
			goForward();
		}
		else if ( strcmp(command, "l") == 0 ) {
			turnLeft();
		}
		else if ( strcmp(command, "r") == 0 ) {
			turnRight();
		}
		
	
}
 
int main(int argc, char *argv[]) {

    if(setupRobot() != -1) {  	
		printf(" > %s\n",argv[1]);
		execComand(argv[1]);
		delay(300);
		stop(); 
	}
 
}
