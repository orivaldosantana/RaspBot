/**
*/

#include <ctime>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <raspicam/raspicam_cv.h>

#include <opencv2/opencv.hpp>

using namespace std; 
 
int main ( int argc,char **argv ) {
   
 
    raspicam::RaspiCam_Cv Camera;
    cv::Mat frame;
    cv::Mat frame_threshold;
 
	// pixel 
	cv::Vec3b p; 
	int imgHeight = 480; 
	int imgWidth = 640; 
	cv::Point pAlvo(imgWidth/2,imgHeight/2); 
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
    while((char)cv::waitKey(1)!='q'){
        Camera.grab();
        Camera.retrieve ( frame);
        cv::inRange(frame, cv::Scalar(0,0,40), cv::Scalar(10,15,80),frame_threshold);
        circle(frame, pAlvo, 20, cv::Scalar(0,255,0),2); 
 
        cv::imshow("Video Capture",frame);
        cv::imshow("Video Threshold",frame_threshold);
        p = frame.at<cv::Vec3b>(pAlvo); 
        cout << p << endl;
         
    }
    cout<<"Stop camera..."<<endl;
    Camera.release();
     
}
