MAIN_DIR=~/Workspace/workspace/NASA-RMC-2018
g++ -std=c++11 -o LaptopJoystickWifi.out $MAIN_DIR/testCode/Teleop/LaptopJoystickWifi.cpp -I$MAIN_DIR/include $MAIN_DIR/src/util/Telecomm.cpp $MAIN_DIR/src/util/Formatter.cpp $MAIN_DIR/src/util/joystick.cc

