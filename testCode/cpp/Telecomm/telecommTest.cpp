#include <stdio.h>
#include <string.h>

#include "Telecomm.h"

int main(int argc, char *argv[]){
  if (argc != 4){
    fprintf(stderr, "usage: %s dst-hostname dst-udpport src udpport\n");
    exit(1);
  }

  Telecomm comm = new Telecomm(argv[1], argv[2], argv[3]);
  if (comm.status() != 0){
    fprintf(stdout, "Error: %s\n", comm.verboseStatus());
    exit(2);
  }

  while(1){
    if (comm.update() != 0){

    }

    // copy over all structure from twoWayUDP_tst1.cpp

  }
