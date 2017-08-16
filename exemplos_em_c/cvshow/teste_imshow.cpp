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
 
    //set camera params
    Camera.set( CV_CAP_PROP_FORMAT, CV_8UC1 );
    //Open camera
    cout<<"Opening Camera..."<<endl;
    if (!Camera.open()) {cerr<<"Error opening the camera"<<endl;return -1;}
    //Start capture
    cout<<"Capturing frames ...."<<endl;
    cv::namedWindow("Video Capture", cv::WINDOW_AUTOSIZE);
    while((char)cv::waitKey(1)!='q'){
        Camera.grab();
        Camera.retrieve ( frame);
        cv::imshow("Video Capture",frame);
    }
    cout<<"Stop camera..."<<endl;
    Camera.release();
     
}
