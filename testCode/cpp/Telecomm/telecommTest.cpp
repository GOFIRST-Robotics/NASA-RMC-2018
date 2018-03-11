#include <stdio.h>
#include <string.h>

#include "Telecomm.h"

#define ERR_CHECK \
  do { if (comm.update() != 0){ \
    fprintf(stdout, "Error: %s\n", comm.verboseStatus().c_str()); \
    return comm.status(); \
  } } while(0)

int main(int argc, char *argv[]){
  if (argc != 4){
    fprintf(stderr, "usage: ./programname dst-hostname dst-udpport src udpport\n");
    exit(1);
  }

  Telecomm comm(argv[1], atoi(argv[2]), atoi(argv[3]));
  comm.setFailureAction(false);
  ERR_CHECK;

  while(1){
    comm.update();
    ERR_CHECK;

    if(comm.stdioReadAvail()){
      std::string msg = comm.stdioRead();
      ERR_CHECK;

      if(!msg.compare("EOM\n")){
        printf("Received EOM closing\n");
        break;
      }

      comm.send(msg);
      ERR_CHECK;
    }

    if(comm.recvAvail()){
      std::string msg = comm.recv();
      ERR_CHECK;

      fprintf(stdout, "Received message: %s\n", msg.c_str());
    }

    printf(".\n");

  }
}
