MAIN_DIR=~/Workspace/workspace/NASA-RMC-2018
g++ -std=c++11 -o PiWifiMotors.out $MAIN_DIR/testCode/Teleop/PiWifiMotorsSerial.cpp -I$MAIN_DIR/include $MAIN_DIR/src/util/Telecomm.cpp -lserial
