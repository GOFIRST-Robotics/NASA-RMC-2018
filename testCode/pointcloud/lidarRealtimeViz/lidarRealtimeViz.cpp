#include <iostream>

#include <boost/thread/thread.hpp>
#include <pcl/common/common_headers.h>
#include <pcl/visualization/pcl_visualizer.h>

#include <opencv2/core.hpp>
#include <LIDAR.h>

boost::shared_ptr<pcl::visualization::PCLVisualizer> simpleVis (pcl::PointCloud<pcl::PointXYZ>::ConstPtr cloud) {
  // -----Open 3D viewer and add point cloud-----
  // --------------------------------------------
  boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer (new pcl::visualization::PCLVisualizer ("3D Viewer"));
  viewer->setBackgroundColor (0, 0, 0);
  viewer->addPointCloud<pcl::PointXYZ> (cloud, "LIDAR cloud");
  viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "LIDAR cloud");
  viewer->addCoordinateSystem (1.0);
  viewer->initCameraParameters ();
  return (viewer);
}

int main(){
  // Init pointcloud "cloud"
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloudPtr (new pcl::PointCloud<pcl::PointXYZ>);
  cloudPtr->points.push_back({0.0,0.0,0.0});
  cloudPtr->width = (int) cloudPtr->points.size();
  cloudPtr->height = 1;
  
  // Init viewer
  boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;
  viewer = simpleVis(cloudPtr);

  // Lidar
  LIDAR lidar = LIDAR("","");
  
  while(!viewer->wasStopped()){
    // Update point cloud
    std::vector<lidarPoint> scan = lidar.scan(); 
    for(std::vector<lidarPoint>::iterator it = scan.begin(); it!=scan.end(); ++it)
        cloudPtr->points.push_back(

    // Update viewer
    viewer->updatePointCloud(cloudPtr, "LIDAR cloud");
    viewer->spinOnce(100);
    boost::this_thread::sleep(boost::posix_time::microseconds(100000));
  }
  
  return 0;
}
