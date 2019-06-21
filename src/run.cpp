#include <ros/ros.h>
#include <iostream>
#include <vector>

#include "sensor_msgs/image_encodings.h"
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>

class save_training_data{
private:
  int file_num=0;
protected:
  std::string pc_name;
  std::string workspace_name;

public:
  void load_param(ros::NodeHandle nh){
    //load parameters with default values of "usr" and "catkin_ws"
    nh.param<std::string>("pc_name", pc_name, "usr");
    nh.param<std::string>("workspace_name", workspace_name, "catkin_ws");
    //display path for easyer debug
    ROS_INFO("Path to workspace: /home/%s/%s/",pc_name.c_str(),workspace_name.c_str());

  }
  std::string get_file_path(){

    std::string home="/home/";
    std::string slash="/";
    std::string rest="/src/train_data/data/train/";
  	std::string file_name="train_data_";
    int actual_file_number= file_num/10;
  	std::string file_number=std::to_string(actual_file_number);
  	std::string file_type=".png";
  	std::string full_path=home+pc_name+slash+workspace_name+rest+file_name+file_number+file_type;
    return full_path;
  }
  void imageCallback(const sensor_msgs::CompressedImageConstPtr& msg)
  {
  	cv::Mat input;
    try
    {
      input = cv::imdecode(cv::Mat(msg->data),1);//convert compressed image data to cv::Mat

    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("Could not convert to image!");
    }

    //save the image
    if (file_num%10==0) {
      std::string file_path=get_file_path();
      std::vector<int> compression_params;
      compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
      compression_params.push_back(9);
      try {
          cv::imwrite(file_path, input, compression_params);
          ROS_INFO("Saved image:%s",file_path.c_str());
      }
      catch (cv::Exception& e) {
          ROS_ERROR("Error converting image to PNG format");
      }
    }
    file_num++;
  }

};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "data_trainer");
	ros::NodeHandle nh;
  //NodeHandle for parameters
  ros::NodeHandle param_nh("~");
  //create class instance
  save_training_data begin;
  //Load the parameters into the class
  begin.load_param(param_nh);
  cv::startWindowThread();
	ros::Subscriber sub = nh.subscribe("/main_cam/image_raw/compressed", 1, &save_training_data::imageCallback,&begin);
	ros::spin();
}
