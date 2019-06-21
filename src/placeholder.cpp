#include <ros/ros.h>
#include <iostream>
#include <vector>

#include "sensor_msgs/image_encodings.h"
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>
using namespace cv;
using namespace std;
class save_training_data{
private:

protected:
  std::string pc_name;
  std::string workspace_name;

public:
  void imageCallback(const sensor_msgs::CompressedImageConstPtr& msg)
  {
  	cv::Mat src;
    try
    {
      src = cv::imdecode(cv::Mat(msg->data),1);//convert compressed image data to cv::Mat

    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("Could not convert to image!");
    }

    Mat src_gray;
    Mat grad;
    std::string window_name = "Human extractor";
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;

    // Load an image
    Mat refframe = imread("/home/peter/catkin_ws/src/train_data/data/refframe.png");
    Mat refframe_gray;
    // Convert it to gray
    cvtColor( refframe, refframe_gray, CV_BGR2GRAY );
    // Convert it to gray
    cvtColor( src, src_gray, CV_BGR2GRAY );
    subtract(src_gray,refframe_gray,src_gray);
    convertScaleAbs( src_gray, src_gray );

    imshow( window_name, src_gray );

    waitKey(0);
  }

};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "data_trainer");
	ros::NodeHandle nh;
  // Create window
  std::string window_name = "Human extractor";
  namedWindow( window_name, CV_WINDOW_AUTOSIZE );
  //create class instance
  save_training_data begin;
  //Load the parameters into the class
  cv::startWindowThread();
	ros::Subscriber sub = nh.subscribe("/main_cam/image_raw/compressed", 1, &save_training_data::imageCallback,&begin);
  while(ros::ok()){
    ros::spinOnce();

  }
  destroyAllWindows();
}
