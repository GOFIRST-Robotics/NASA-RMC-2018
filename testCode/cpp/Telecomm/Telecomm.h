#ifndef TELECOMM_H
#define TELECOMM_H

#include <string>

/* 
  Telecomm defines macros and functions to send and recieve data between two 
  computers performing intensive operations in parallel. If in a live stream
  control scenario, or the adjustment of state space, mode handling and state
  handling is not handled here. 

  The handling of state is best done over another file, Protocal.h, which
  should have macros defining the content of the message and thus how to 
  handle the message. Ex. post to user, update motors, etc. 
*/

class Telecomm {
  public: 
    // Initialize communications: Telecomm comm = new Telecomm("127.0.0.1", 5005, 5006);
    Telecomm(std::string dst_addr, int dst_port, int src_port);
    ~Telecomm();

    // In loop, call update; if != 0, error
    int update();

    // Can & only read from STDIO if available
    bool stdioReadAvail();
    std::string stdioRead();

    // Can send data at any time, no wait; if !=0, error
    int send(std::string msg);

    // Can recv data if available, check status
    bool recvAvail();
    std::string recv();

    // getErrno() returns the errno set by any socket functions
    int getErrno();
    // status() returns the internal state of the status, status() !=0 is error
    int status();
    // simpleStatus: a string code detailing the problem indicated by/of status
    std::string simpleStatus(int status);
    std::string simpleStatus();
    // verboseStatus: a more verbose version of simpleStatus if available
    std::string verboseStatus(int status);
    std::string verboseStatus();

    // Set failure to throw error, true/default, or not/false
    void setFailureAction(bool throwError);
};

#endif
