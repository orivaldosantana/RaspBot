/**
 * http://docs.opencv.org/2.4/doc/tutorials/imgproc/shapedescriptors/moments/moments.html 
*/

#include <ctime>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <raspicam/raspicam_cv.h>

#include <opencv2/opencv.hpp>


using namespace std; 

/*************************************************************************/  

#include <wiringPi.h>
#include <softPwm.h>
#include <string>
#include <iostream>


unsigned int timerBeginCommand,timerEndCommand, timerStop, currentTime;
int contTimerState = 0; 
bool executingCommand = false; 
std::string currentCommand; 

#define servoRightPin 26 // BCM 13
#define servoLeftPin 23 // BCM 12

int minValue;
int pwmRange;
int maxValue; 

int vel; // velocidade do motor 
    
 /** 
 * Recebe valores entre um máximo e um mínimo 
 */
int turnOnServos(int _left, int _right ) {
	int rightZero = 74;
	int leftZero = 82; 
	pwmWrite(servoLeftPin, leftZero - _left); 
	
	pwmWrite(servoRightPin,rightZero +  _right); 
	
	return 0; 
}

    
int setupRobot() {
	if(wiringPiSetup() == -1) {  
			std::cout << "Setup do wiringPi falhou!" << std::endl; 
			return -1;
	}
    minValue = 25; 
 	pwmRange= 125;
	maxValue = pwmRange; 
 
	vel = 10;
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



void auxServoCalibrating(int _servoPin) {
	int testStep = 5;
	int timeDelay = 500; 
	int i;
	for (i=minValue; i < maxValue; i += testStep ) {
		pwmWrite(_servoPin, i);
		std::cout << "V " << i << std::endl; 
		delay(timeDelay); 
	}
	for (i = maxValue; i > minValue; i -= testStep) {
		pwmWrite(_servoPin, i);
		std::cout << "V " << i << std::endl; 
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
	turnOnServos(vel,0); 
}
 
void turnRight(){
	turnOnServos(0,vel); 
}
 

void execComand(std::string command) {
	
	currentTime = millis(); 
	if ( contTimerState == 0) { 
		timerBeginCommand = currentTime;
		currentCommand = command;  
		contTimerState++;
		cout<<"-------------- 0 ------------ "<< command << endl;
	} else if ( contTimerState == 1) {
		if (currentTime - timerBeginCommand > 50) {
			currentCommand = "s";
			contTimerState++; 
			
			cout<<"-------------- 1000 ------------ "<<endl;
		} 
	} else if (contTimerState == 2 ) {
		if (currentTime - timerBeginCommand > 100) {
			contTimerState = 0; 
			cout<<"-------------- 2000 ------------ "<<endl; 
		} 
	}
	
	// s -> stop   
	if ( currentCommand.compare("s") == 0 ) {
		stop();
	} // b -> back 
	else if ( currentCommand.compare("b") == 0 ) {
		goBackward(); 
	} // f -> forward 
	else if ( currentCommand.compare( "f") == 0 ) {
		goForward();
	}// l -> left 
	else if (currentCommand.compare( "l") == 0 ) {
		turnLeft();
	} // r -> right 
	else if ( currentCommand.compare( "r") == 0 ) {
		turnRight();
	}
}



/**
 * Códigos para criar um classe RaspCam CV de rastreamento de cor
 */ 
	cv::RNG rng(12345);
 
 
 
	cv::Mat frame_threshold;
	vector<vector<cv::Point> > contours;
    vector<cv::Vec4i> hierarchy;
    int findObjectByColor(cv::Mat &_frame) {
		
		// pixel 
		cv::Vec3b p;
		cv::Point pAlvo(0,0); 
		
		cv::inRange(_frame, cv::Scalar(0,60,95), cv::Scalar(5,120,130),frame_threshold);
        
        cv::imshow("Video Threshold",frame_threshold);
        
        /// Find contours
        cv::findContours( frame_threshold, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
        
        /// Get the moments
		vector<cv::Moments> mu(contours.size() );
		for( int i = 0; i < contours.size(); i++ ){
			 mu[i] = moments( contours[i], false ); }
			 
		///  Get the mass centers:
		vector<cv::Point2f> mc( contours.size() );
		for( int i = 0; i < contours.size(); i++ )
			{ mc[i] = cv::Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }	 
			
		/// Draw contours
		cv::Mat drawing = cv::Mat::zeros( frame_threshold.size(), CV_8UC3 );
		for( int i = 0; i< contours.size(); i++ )
		{
			if ( contours[i].size() > 100 ) {
				cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
				cv::drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, cv::Point() );
			
				circle( drawing, mc[i], 30, color, -1, 8, 0 );
				cout << " " << contours[i].size() ; 
				pAlvo = mc[i]; 
			}
			
		}
		cout << endl; 
		
		/// Show in a window
		
		imshow( "Contours", drawing );

        circle(_frame, pAlvo, 20, cv::Scalar(0,255,0),2); 
 
        cv::imshow("Video Capture",_frame);
        
        
        //p = _frame.at<cv::Vec3b>(pAlvo); 
        return pAlvo.x; 
		
	} 
	
/*************************************************************************/  
 
 
int main ( int argc,char **argv ) {
   
 
 
	if(setupRobot() != -1) { 
		
		
		raspicam::RaspiCam_Cv Camera;
		cv::Mat frame;

	 
		int imgHeight = 480; 
		int imgWidth = 640; 
		// left and right limit to control 
		int leftLimit = imgWidth/3; 
		int rightLimit = (2 * imgWidth)/3;
		//set camera params
		Camera.set( CV_CAP_PROP_FORMAT, CV_8UC3 ); // 8U -> 8 bits -> uchar e C3 -> três canais 
		Camera.set( CV_CAP_PROP_FRAME_WIDTH, 640); 
		Camera.set( CV_CAP_PROP_FRAME_HEIGHT, 480);
		//Open camera
		cout<<"Opening Camera..."<<endl;
		if (!Camera.open()) {cerr<<"Error opening the camera"<<endl;return -1;}
		//Start capture
		cout<<"Capturing frames ...."<<endl;
		cv::namedWindow("Video Capture", cv::WINDOW_AUTOSIZE);
		cv::namedWindow("Video Threshold", cv::WINDOW_AUTOSIZE);
		cv::namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
		while((char)cv::waitKey(1)!='q'){
			
			Camera.grab();
			Camera.retrieve ( frame);
			int pos = findObjectByColor(frame);
			//cout << pos << endl;
			
			if ( pos == 0 || pos > leftLimit && pos < rightLimit ) { 
				//stop(); 
				execComand("s");
			}
			else if ( pos < leftLimit ) {
				//turnRight(); 
				execComand("r");
			}
			else if ( pos > rightLimit ) {
				//turnLeft();
				execComand("l");
			}
			 
		}
		cout<<"Stop camera..."<<endl;
		Camera.release();
	}
     
}
