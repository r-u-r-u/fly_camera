#include <ros/ros.h>
#include <ros/package.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <camera_info_manager/camera_info_manager.h>
#include <opencv2/opencv.hpp>
#include "fly_camera/FlyCap2CV.h"
int main(int argc,char *argv[]){

  ros::init (argc, argv, "fly_camera");
  ros::NodeHandle nh("~");
  
  int cam_number=0;
  std::string side="left";
  nh.getParam("number",cam_number);
  nh.getParam("side",side);

  image_transport::ImageTransport it(nh);
  image_transport::Publisher image_pub = it.advertise("image_raw", 10);
  ros::Publisher info_pub = nh.advertise<sensor_msgs::CameraInfo>("camera_info",5);
 
  cv::Mat image;
  FlyCap2CVWrapper cam(cam_number);
  ros::Rate loop_rate (30); // capture image at 10Hz

  std::string caminfo_url = "file://"+ros::package::getPath("fly_camera")+"/data/"+side+".yaml";
  camera_info_manager::CameraInfoManager caminfo(nh,side,caminfo_url);
  sensor_msgs::CameraInfo ci;
  while(ros::ok()) {

    image = cam.readImage();
    sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", image).toImageMsg();
    image_pub.publish(msg);

    ci.header.stamp = ros::Time::now();
    ci =caminfo.getCameraInfo();
    info_pub.publish(ci);

    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
