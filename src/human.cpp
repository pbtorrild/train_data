#include <ros/ros.h>
#include <iostream>
#include <vector>
#include <math.h>

#include <sensor_msgs/image_encodings.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "data_trainer");
	ros::NodeHandle nh;
  Mat src, src_gray;
  Mat grad;
  std::string window_name = "Human extractor";
  int scale = 1;
  int delta = 0;
  int ddepth = CV_32F;



  // Load an image
  src = imread("/home/peter/catkin_ws/src/train_data/data/train/test1.jpg");
  //apply median filter
  medianBlur( src, src, 1);

  // Convert it to gray
  //cvtColor( src, src_gray, CV_BGR2GRAY );

  // Create window
  namedWindow( window_name, CV_WINDOW_AUTOSIZE );

  // Generate grad_x and grad_y
  Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;

  // Gradient X
  //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
  Sobel( src, grad_x, ddepth, 1, 0, 5, scale, delta, BORDER_DEFAULT );
  convertScaleAbs( grad_x, abs_grad_x );

  // Gradient Y
  //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
  Sobel( src, grad_y, ddepth, 0, 1, 5, scale, delta, BORDER_DEFAULT );
  convertScaleAbs( grad_y, abs_grad_y );
  addWeighted( abs_grad_x, 0.5, abs_grad_x, 0.5, 0, grad);

  Mat mag, angle;
  bool angle_in_degree=true;
  cartToPolar(grad_x,grad_y,mag,angle,angle_in_degree);
  //find the Histogram

  // Quantize the mag to 20 levels
  // and the angle to 20 levels
  int mag_bins = 20, angle_bins = 20;
  int histSize[] = {mag_bins, mag_bins};
  // mag varies from 0 to 32bit int
  float mag_ranges[] = { 0, 4294967296 };
  // anlge varies from 0 to
  // 360 degree
  float angle_ranges[] = { 0, 360 };
  const float* ranges[] = { mag_ranges, angle_ranges };
  MatND hist;
  // we compute the histogram from the 0-th and 1-st channels
  int channels[] = {0, 1};

  calcHist( &mag, 1, channels, Mat(), // do not use mask
           hist, 2, histSize, ranges,
           true, // the histogram is uniform
           false );
  double maxVal=0;
  minMaxLoc(hist, 0, &maxVal, 0, 0);

  int scale_hist = 10;
  Mat histImg = Mat::zeros(mag_bins*scale_hist, angle_bins*10, CV_8UC3);

  for( int h = 0; h < mag_bins; h++ )
  {
    for( int s = 0; s < angle_bins; s++ )
    {
      float binVal = hist.at<float>(h, s);
      int intensity = cvRound(binVal*255/maxVal);
      rectangle( histImg, Point(h*scale_hist, s*scale_hist),
                  Point( (h+1)*scale - 1, (s+1)*scale_hist - 1),
                  Scalar::all(intensity),
                  CV_FILLED );
    }
  }
  imshow( window_name, grad );
  namedWindow( "H-S Histogram", 1 );
  imshow( "H-S Histogram", histImg );

  waitKey(0);
  while(ros::ok()){
    ros::spinOnce();

  }
  destroyAllWindows();
}
