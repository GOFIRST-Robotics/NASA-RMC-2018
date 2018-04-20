#include <iostream>

#include <boost/thread/thread.hpp>
#include <pcl/common/common_headers.h>
#include <pcl/visualization/pcl_visualizer.h>

#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>

#include <opencv2/core.hpp>
#include "Lidar.h"

bool dengo;

void mouseEventOccurred (const pcl::visualization::MouseEvent &event, void* viewer_void) {
  //pcl::visualization::PCLVisualizer *viewer = static_cast<pcl::visualization::PCLVisualizer *> (viewer_void);
  if (event.getButton () == pcl::visualization::MouseEvent::LeftButton &&
      event.getType () == pcl::visualization::MouseEvent::MouseButtonRelease) {
    dengo = true;
  }
}

boost::shared_ptr<pcl::visualization::PCLVisualizer> simpleVis (pcl::PointCloud<pcl::PointXYZ>::ConstPtr cloud) {
  // -----Open 3D viewer and add point cloud-----
  // --------------------------------------------
  boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer (new pcl::visualization::PCLVisualizer ("3D Viewer"));
  viewer->setBackgroundColor (0, 0, 0);
  pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> single_color(cloud, 0, 255, 0);
  viewer->addPointCloud<pcl::PointXYZ> (cloud, single_color, "LIDAR cloud");
  viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "LIDAR cloud");
  viewer->addCoordinateSystem (0.1);
  viewer->initCameraParameters ();
  viewer->registerMouseCallback (mouseEventOccurred, (void*)&viewer);
  return (viewer);
}

int main(int argc, char* argv[]){
  if(argc != 5)
    std::cout << "Usage should be ./program /dev/ttyUSB0 /dev/ttyUSB1 ../leftStandLidar.yml ../rightStandLidar.yml" << std::endl;

  dengo = false;

  // Init pointcloud "cloud"
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloudPtr (new pcl::PointCloud<pcl::PointXYZ>);
  cloudPtr->points.push_back({0.0,0.0,0.0});
  cloudPtr->width = (int) cloudPtr->points.size();
  cloudPtr->height = 1;
  
  // Init viewer
  boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;
  viewer = simpleVis(cloudPtr);

  // Lidar
  // Currently set for Jude's computer, changes can easily be made. Left is closes to screen
  //Lidar lidarLeft = Lidar("/dev/ttyUSB0","leftStandLidar.yml");
  //Lidar lidarRight = Lidar("/dev/ttyUSB1","rightStandLidar.yml");
  Lidar lidarLeft = Lidar(argv[1],argv[3]);
  Lidar lidarRight = Lidar(argv[2],argv[4]);

  //lidarLeft.setMotorSpeed(5);
  //lidarRight.setMotorSpeed(5);
  //lidarLeft.setSampleRate(1000);
  //lidarRight.setSampleRate(1000);
  
  float limL = -45;
  float limR = 45;

  while(!viewer->wasStopped()){
    // Delete old points
    if(dengo || cloudPtr->points.size() > 1500){
      cloudPtr->points.erase(cloudPtr->points.begin(),cloudPtr->points.end()-1);
      cloudPtr->points.shrink_to_fit();
      dengo = false;
    }

    // Update point cloud
    if(1){ // lidarLeft.scanAvail()){
      //std::vector<lidarPoint> scan = lidar.scan(); 
      for(auto & v : lidarLeft.scan()){ // v : lidarPoint
        if(v.str > 0.2 && v.ang > limL && v.ang < limR && v.Z > 0.1 && v.Z < 1.5)
          cloudPtr->points.push_back({v.X,v.Y,v.Z});
      }
    }
    if(1){
      for(auto & v : lidarRight.scan()){
        if(v.str > 0.2 && v.ang > limL && v.ang < limR && v.Z > 0.1 && v.Z < 1.5)
          cloudPtr->points.push_back({v.X,v.Y,v.Z});
      }
    }

    // Update viewer
    cloudPtr->width = (int) cloudPtr->points.size();
    viewer->updatePointCloud(cloudPtr, "LIDAR cloud");
    viewer->spinOnce(100);
    boost::this_thread::sleep(boost::posix_time::microseconds(100000));
  }
  
  //pcl::io::savePCDFileASCII("tst.pcd", *cloudPtr);

  return 0;
}
