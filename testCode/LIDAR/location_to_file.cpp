// To compile:
// g++ -o l_to_file -std=c++11 location_to_file.cpp -lsweep

//  To run: (5 = number of heights) (testdata.dat = file to save it to)
//  ./l_to_file /dev/ttyUSB0 5 datatest1.dat

//use the python program to see it

//argv[2] = 5 argv[3]=filename

#include <cstdlib>
#include <iostream>
#include <sweep/sweep.hpp>
#include <fstream>

int main(int argc, char* argv[]) try {
  if (argc != 4) {
    std::cerr << "Usage: ./example-c++ device numheights filename\n";
    return EXIT_FAILURE;
  }
  
  //std::cout << argv[2] << std::endl;
  std::cout << "Constructing sweep device..." << std::endl;
  sweep::sweep device{argv[1]};

  std::cout << "Motor Speed Setting: " << device.get_motor_speed() << " Hz" << std::endl;
  std::cout << "Sample Rate Setting: " << device.get_sample_rate() << " Hz" << std::endl;

  std::cout << "Beginning data acquisition as soon as motor speed stabilizes..." << std::endl;
  device.start_scanning();
  //Setting up file
  std::ofstream output;
  double currentheight;
  output.open(argv[3]);
  int numheights = atoi(argv[2]); 
  std::cout<<"Number of heights:   "<<numheights<<"\n";
  
  for(int count=0; count<numheights; count++){
	  std::cout<<"Enter Current Height\n";
	  std::cin>>currentheight;
	  for (auto n = 0; n < 2; ++n) {//2 is the number of scans per height
		const sweep::scan scan = device.get_scan();

		std::cout << "Scan #" << n << ":" << std::endl;
		for (const sweep::sample& sample : scan.samples) {
		  std::cout << "angle " << sample.angle << " distance " << sample.distance << " strength " << sample.signal_strength << " height "<<currentheight<<"\n";
		//File io 
		output<< sample.angle << " "<< sample.distance << " "<<sample.signal_strength <<" "<<currentheight<<"\n";
		}
		
	  }
	}
  output.close();

  device.stop_scanning();
} catch (const sweep::device_error& e) {
  std::cerr << "Error: " << e.what() << std::endl;
}
