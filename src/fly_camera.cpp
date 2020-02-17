#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <opencv2/opencv.hpp>
#include "fly_camera/FlyCap2CV.h"
int main(int argc,char *argv[]){

  ros::init (argc, argv, "fly_camera");
  ros::NodeHandle nh("~");

  image_transport::ImageTransport it(nh);
  image_transport::Publisher image_pub = it.advertise("image_raw", 10);
 
  cv::Mat image;
  FlyCap2CVWrapper cam(0);
  ros::Rate looprate (10); // capture image at 10Hz

  while(ros::ok()) {
    image = cam.readImage();
    sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", image).toImageMsg();
    image_pub.publish(msg);
    ros::spinOnce();
    looprate.sleep();
  }
  return 0;
}
