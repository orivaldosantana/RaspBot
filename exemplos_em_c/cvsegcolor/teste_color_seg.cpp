/**
 * http://docs.opencv.org/2.4/doc/tutorials/imgproc/shapedescriptors/moments/moments.html 
*/

#include <ctime>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <raspicam/raspicam_cv.h>

#include <opencv2/opencv.hpp>

cv::RNG rng(12345);

using namespace std; 
 
int main ( int argc,char **argv ) {
   
 
    raspicam::RaspiCam_Cv Camera;
    cv::Mat frame;
    cv::Mat frame_threshold;
    
    vector<vector<cv::Point> > contours;
    vector<cv::Vec4i> hierarchy;
 
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
    cv::namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
    while((char)cv::waitKey(1)!='q'){
        Camera.grab();
        Camera.retrieve ( frame);
        cv::inRange(frame, cv::Scalar(0,0,40), cv::Scalar(10,15,80),frame_threshold);
        
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
			}
			
		}
		cout << endl; 
		
		/// Show in a window
		
		imshow( "Contours", drawing );

        circle(frame, pAlvo, 20, cv::Scalar(0,255,0),2); 
 
        cv::imshow("Video Capture",frame);
        
        p = frame.at<cv::Vec3b>(pAlvo); 
        cout << p << endl;
         
    }
    cout<<"Stop camera..."<<endl;
    Camera.release();
     
}
