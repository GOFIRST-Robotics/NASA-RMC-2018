# testCode

TestCode contains the rough, very rough, and possibly working pieces of code, before they go into the include/ or src/ folders. Any further development on the file should be done in the testCode folder, so that the ones in include/ or src/ are always stable, and can be appropriately targetted by CMAKE etc. 
Each production-worthy file either has it's first line as a comment of its name (.cpp), or first two a pair of appropriate defs (.h), followed by a one line `// VERSION 1.0.0` or such. Bump major version for significant changes, minor version for the changing of any function, and bump it a 0.0.1 if the changes are merely a comment, an interior bug fix that doesn't change interface, or something small. 
Always update both .h/.cpp files for any change to either one. Always make sure to use the latest version. And don't make changes without changing version. 
All c++ files should have extensions .h/.cpp. All executables should have .out or .exe extention. Any c++ code written for Arduino, but meant for compiling on computer, has extentions .hh/.cc. In the case a c++ header file uses advanced c++ functionality, or that it's necessary to have a version which uses more "vanilla" c++, name the more advanced header .hpp, and the lesser .h. Also applies to any cases where we have c code: c extentions .h/.c with respective c++ extentions .hpp/.cpp. 
Each directory must have a README.md explaining what is in it, and a DevLog.txt that records ALL the stuff that happens. The master DevLog.txt in the main directory should be updated in overview, but always with a summary of the work done inside. 

This folder contains the following. 
* Sensors: Interface classes to utilize the functionality of sensors, may just be wrappers
* Vision: Specifically CV related things
* Communications: Has communications, and some utils
* Fusion: This is where larger components get built together out of smaller base classes, but are not executables
* Main_Exec: This is where executables go
