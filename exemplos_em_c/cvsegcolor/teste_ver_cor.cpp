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
 
	// pixel 
	cv::Vec3b p; 
	cv::Point pAlvo(300,400); 
    //set camera params
    Camera.set( CV_CAP_PROP_FORMAT, CV_8UC3 ); // 8U -> 8 bits -> uchar e C3 -> três canais 
    //Open camera
    cout<<"Opening Camera..."<<endl;
    if (!Camera.open()) {cerr<<"Error opening the camera"<<endl;return -1;}
    //Start capture
    cout<<"Capturing frames ...."<<endl;
    cv::namedWindow("Video Capture", cv::WINDOW_AUTOSIZE);
    while((char)cv::waitKey(1)!='q'){
        Camera.grab();
        Camera.retrieve ( frame);
        circle(frame, pAlvo, 20, cv::Scalar(0,255,0),2); 
        cv::imshow("Video Capture",frame);
        p = frame.at<cv::Vec3b>(pAlvo); 
        cout << p << endl;
        //cout << int(frame.at<uchar>(300,300)) << endl; // para tons de cinza 
    }
    cout<<"Stop camera..."<<endl;
    Camera.release();
     
}
