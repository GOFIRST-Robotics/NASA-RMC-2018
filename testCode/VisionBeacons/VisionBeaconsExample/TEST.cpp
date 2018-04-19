#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <cmath>

const int FRAME_WIDTH = 680;
const int FRAME_HEIGHT = 480;
//White
//const cv::Scalar minHSV = cv::Scalar(0,0,254);
//const cv::Scalar maxHSV = cv::Scalar(1,1,255);
const cv::Scalar minHSV = cv::Scalar(0,0,254);
const cv::Scalar maxHSV = cv::Scalar(1,1,255);
//Green
//const cv::Scalar minHSV = cv::Scalar(50,245,245);
//const cv::Scalar maxHSV = cv::Scalar(70,255,255);
int main(){
  cv::Mat origin;
  cv::Mat frame;
  
  //Newly added.
  cv::SimpleBlobDetector::Params params;
  // Filter by Area.
  params.filterByArea = true;
  params.minArea = 20;
  params.maxArea = 1000000;
  // Filter by Circularity.
  params.filterByCircularity = true;
  params.minCircularity = 0.75;
  // Filter by Inertia.
  params.filterByInertia = true;
  params.minInertiaRatio = 0.50;
  cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);
  int i = 1;
  
  //video capture object to acquire webcam feed
  cv::VideoCapture capture;
  //open capture object at location zero (default location for webcam)
  capture.open(0);
  //set height and width of capture frame
  capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
  capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
  while(1){
    // Declare necessary objects.
    std::vector<cv::KeyPoint> keypoints;
    std::vector<cv::Point2f> point2f;
    cv::Mat im_with_keypoints;
    double distance = -1, d2 = 0, angle = -1;
    
    // Read from the camera.
    capture.read(origin);
    frame = origin;
    cv::cvtColor(frame, frame, cv::COLOR_BGR2HSV);
    cv::inRange(frame, minHSV, maxHSV, frame);
    cv::Mat black(frame.rows, frame.cols, CV_8UC3, cv::Scalar(0,0,0));
    // Detect blobs and draw on black board.
    cv::bitwise_not(frame,frame);
    detector->detect(frame, keypoints);
    
    // Convert type.
    cv::KeyPoint::convert(keypoints, point2f);
    
    // If there are more than 2 elements in the vector, print distance btw them.
    if(point2f.size() > 1){
      // Distance
      distance = cv::norm(cv::Mat(point2f[0]), cv::Mat(point2f[1]), 2);
      // Horizontal Angle
      if(point2f[0].y > point2f[1].y){
//        temp1.x = point2f[0].x;
//        temp1.y = point2f[1].y;
        cv::Point2f temp1(point2f[0].x,point2f[1].y);
        cv::KeyPoint temp2(temp1,2,-1,0,0,-1);
        point2f.push_back(temp1);
        keypoints.push_back(temp2);
      }
      else{
//        temp1.x = point2f[1].x;
//        temp1.y = point2f[0].y;
        cv::Point2f temp1(point2f[1].x,point2f[0].y);
        cv::KeyPoint temp2(temp1,2,-1,0,0,-1);
        point2f.push_back(temp1);
        keypoints.push_back(temp2);
      }
      d2 = cv::norm(cv::Mat(point2f[1]), cv::Mat(point2f[2]), 2);
      angle = std::acos(d2/distance)*(180/3.1415);
    }
    
    // Print.
    std::cout<<"Size : "<<point2f.size()<<std::endl;
    std::cout<<"Distance : "<<distance<<std::endl;
    std::cout<<"Angle : "<<angle<<std::endl;
    
    // Draw im_with_keypoints.
    cv::drawKeypoints(black, keypoints, im_with_keypoints, cv::Scalar(255,255,255), cv::DrawMatchesFlags::DEFAULT);
    cv::drawKeypoints(origin, keypoints, origin, cv::Scalar(255,255,255), cv::DrawMatchesFlags::DEFAULT);
    //For visualizing the results.
    cv::flip(im_with_keypoints,im_with_keypoints,1);
    imshow("Test1",im_with_keypoints);
    cv::flip(origin,origin,1);
    imshow("Test2",origin);
    cv::flip(origin,origin,1);
//    cv::cvtColor(origin, origin, cv::COLOR_HSV2BGR);
//    imshow("Test3",origin);
    
    cv::waitKey(10);
  }
}
