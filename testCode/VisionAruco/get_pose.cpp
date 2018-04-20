//CURRENT ASSUMPTION:
//webcamcalibration.yml and aruco_calibration_grid_board_a4.yml are located 
//one directory above where this program is compiled

#include "aruco.h"
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <stdexcept>

using namespace cv;
using namespace std;
using namespace aruco;
class CmdLineParser{int argc;char** argv;public:CmdLineParser(int _argc, char** _argv): argc(_argc), argv(_argv){}   bool operator[](string param)    {int idx = -1;  for (int i = 0; i < argc && idx == -1; i++)if (string(argv[i]) == param)idx = i;return (idx != -1);}    string operator()(string param, string defvalue = "-1")    {int idx = -1;for (int i = 0; i < argc && idx == -1; i++)if (string(argv[i]) == param)idx = i;if (idx == -1)return defvalue;else return (argv[idx + 1]);}};

int main(int argc, char** argv)
{
    try
    {
        CmdLineParser cml(argc, argv);
        if (argc == 1 || cml["-h"])
        {
            cerr << "Usage: (in_image|video.avi) [-c cameraParams.yml] [-m markerMap.h]"   << endl;
            cerr << "\tDictionaries: ";
            for (auto dict : aruco::Dictionary::getDicTypes())
                cerr << dict << " ";
            cerr << endl;
            cerr << "\t Instead of these, you can directly indicate the path to a file with your own generated "
                    "dictionary"
                 << endl;
            cout << "Example to work with apriltags dictionary : video.avi -d TAG36h11" << endl << endl;
            return 0;
        }

        aruco::CameraParameters CamParam;

        // read the input image
        cv::Mat InImage;
        // Open input and read image
        VideoCapture vreader(argv[1]);
        if (vreader.isOpened()) vreader >> InImage;
        else throw std::runtime_error("Could not open input");

        // read camera parameters
		if (cml["-c"])
            CamParam.readFromXMLFile(cml("-c"));
        else CamParam.readFromXMLFile("../webcamcalibration.yml");

        // read marker size if specified (default value -1)
        float markerSize = 0.03614;
        // Create the tracker
        aruco::MarkerMap mmap;
		if (cml["-m"])
            mmap.readFromFile(cml("-m"));
		else mmap.readFromFile("../aruco_calibration_grid_board_a4.yml");

		aruco::MarkerMapPoseTracker poseTracker;
		poseTracker.setParams(CamParam,mmap,markerSize);
		
		MarkerDetector MDetector;
		vector<Marker> markers;
		markers = MDetector.detect(InImage);

		if (poseTracker.estimatePose(markers)){
			cv::Mat rVec = poseTracker.getRvec();
			cv::Mat tVec = poseTracker.getTvec();
			
			cout << "Rotation vector: " << endl;
			cout << rVec << endl;
			cout << "Translation vector: " << endl;
			cout << tVec << endl;
			
			return 0;;
		}
		

		
		

    }
    catch (std::exception& ex)

    {
        cout << "Exception :" << ex.what() << endl;
    }
}