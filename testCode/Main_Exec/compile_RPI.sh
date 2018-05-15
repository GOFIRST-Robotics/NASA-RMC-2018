MAIN_DIR=~/Desktop/NASA-RMC-2018
g++ -o RPI_Main.out -std=c++11 $MAIN_DIR/testCode/Main_Exec/RPI_Main.cpp -I$MAIN_DIR/include $MAIN_DIR/src/util/Telecomm.cpp $MAIN_DIR/src/util/Formatter.cpp -lserial -lopencv_core -lopencv_highgui -lpthread
