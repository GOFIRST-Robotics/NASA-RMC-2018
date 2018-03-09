#include "Telecomm.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define STDIN 0

int sockfd;
struct addrinfo hints, *dstinfo = NULL, *srcinfo = NULL, *p = NULL;
int rv = -1, ret = -1, len = -1,  numbytes = 0;
struct timeval tv;
tv.tv_sec = 0;
tv.tv_usec = 0;
char buffer[256] = {0};
fd_set readfds;
FD_ZERO(&readfds);

void deleteWithException(bool throwit){
  if(dstinfo)
    freeaddrinfo(dstinfo);
  if(srcinfo)
    freeaddrinfo(srcinfo);
  close(sockfd);
  if(throwit)
    throw("COMM_FAIL");
}

Telecomm::Telecomm(string dst_addr, int dst_port, int src_port){
  
  // Set dst info
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  if((rv = getaddrinfo(dst_addr, dst_port, &hints, &dstinfo)) != 0){
    fprintf(stderr, "getaddrinfo for dst address %s\n", gai_strerror(rv));
    ret = 1;
    goto LBL_RET;
  }

  // Get socket for dst info
  for(p = dstinfo; p != NULL; p = p->ai_next){
    if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
      perror("socket");
      continue;
    }
    break;
  }

  // Either failed to get socket to all entries
  if(p == NULL){
    fprintf(stderr, "%s: Failed to get socket\n");
    ret = 2;
    goto LBL_RET;
  }

  // Or connect socket to dst
 if((rv= connect(sockfd, p->ai_addr, p->ai_addrlen)) != 0) {
    fprintf(stderr, "connect: %s\n", gai_strerror(rv));
    ret = 3;
    goto LBL_RET;
 }

  // Set src info
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  // Getting src_addr, could do by hand, should be 192.168.1.50; try replace
  if((rv = getaddrinfo(NULL, src_port, &hints, &srcinfo)) != 0){
    fprintf(stderr, "getaddrinfo for src address %s\n", gai_strerror(rv));
    ret = 4;
    goto LBL_RET;
  }

  // Bind socket src_addr info */
  if((rv = bind(sockfd, srcinfo->ai_addr, srcinfo->ai_addrlen)) != 0) {
    fprintf(stderr, "bind: %s\n", gai_strerror(rv));
    ret = 5;
    goto LBL_RET;
  }

  // End init(?)
  ret = 0;
  
  LBL_RET:
    if(ret == 0){
      return;
    }else{
      deleteWithException(true);
  }

}

Telecomm::~Telecomm(){
  ret = 0;
  deleteWithException(false);
}

int Telecomm::getErrno(){ return errno; }

int Telecomm::status(){ return ret; }

int Telecomm::update(){
  // To be called at the beginning of every loop
  FD_ZERO(&readfds);
  FD_SET(sockfd, &readfds);
  FD_SET(STDIN, &readfds); // Replace or add with other sparse io options
  // If multiple ios, need master list, and select arg0 is max + 1 of list

  if(select(sockfd + 1, &readfds, NULL, NULL, &tv) == -1){
    perror("select");
    ret = 6;
    deleteWithException(true);
  }
  return ret;
}

// Need method to add io to master list, and by accessible name


int sendall(int s, char *buf, int *len) {
  int total = 0;        // how many bytes we've sent
  int bytesleft = *len; // how many we have left to send
  int n;

  while(total < *len) {
    n = send(s, buf+total, bytesleft, 0);
    //fprintf(stdout,"Sendall: %s\n",buf+total);
    if (n == -1) { break; }
    tota += n;
    bytesleft -= n;
  }

  *len = total; // return number actually sent here

  return n==-1?-1:0; // return -1 on failure, 0 on success
}

bool Telecomm::stdioReadAvail(){
  return FD_ISSET(STDIN, &readfds);
}

string Telecomm::stdioRead(){
  memset(buffer, 0, sizeof(buffer));
  len = 0;
  if(0 >= (len = read(STDIN, buffer, sizeof(buffer)))){
    perror("read STDIN");
    ret = 7;
    deleteWithException(true);
  }
  return string(buffer, len); 
}

int Telecomm::send(string msg){
  memset(buffer, 0, sizeof(buffer));
  std::strcpy(buffer, msg.c_str());
  len = length(msg);
  if(sendall(sockfd, buffer, &len) == -1) {
    perror("sendall");
    fprintf(stderr, "We only sent %d bytes b/c of error\n", len);
    ret = 8;
    deleteWithException(true);
  }
  return ret;
}

bool Telecomm::recvAvail(){
  return FD_ISSET(sockfd, &readfds);
}

string Telecomm::recv(){
  memeset(buffer 0, sizeof(buffer));
  numbytes = recv(sockfd, buffer, sizeof(buffer), 0);
  if(0 == numbytes){
    printf("Destination closed");
    ret = 9;
    deleteWithException(true);
    return "";
  }else if(-1 == numbytes){
    perror("recv");
    printf("Receive error check firewall settings\n");
    ret = 10;
    deleteWithException(true);
    return "";
  }
  return string(buffer, numbytes);
}

string Telecomm::simpleStatus(int i){
  switch(i){
    case -1: return "INIT_ERR";
    case 0: return "OK";
    case 1: return "GETADDRINFO_DST";
    case 2: return "SOCK_ERR";
    case 3: return "CONNECT_DST_ERR";
    case 4: return "GETADDRINFO_SRC";
    case 5: return "BIND_SRC_ERR";
    case 6: return "SELECT_ERR";
    case 7: return "STDIN_READ_ERR";
    case 8: return "SEND_ERR";
    case 9: return "DST_CLOSED";
    case 10: return "RECV_ERR";
    default: return "SIMPLESTATUS_ERR";
  }
}

string Telecomm::simpleStatus(){
  return simpleStatus(ret);
}

string Telecomm::verboseStatus(int i){
  switch(i){
    case -1: return "INIT_ERR";
    case 0: return "OK";
    case 1: return ("GETADDRINFO_DST: " + gai_strerror(rv));
    case 2: return "SOCK_ERR: failed to get socket, no addrinfo for dst";
    case 3: return ("CONNECT_DST_ERR: " + gai_strerror(rv));
    case 4: return ("GETADDRINFO_SRC: " + gai_strerror(rv));
    case 5: return ("BIND_SRC_ERR: " + gai_strerror(rv));
    case 6: return "SELECT_ERR";
    case 7: return "STDIN_READ_ERR";
    case 8: return "SEND_ERR";
    case 9: return "DST_CLOSED";
    case 10: return "RECV_ERR: check firewall?";
    default: return ("SIMPLESTATUS_ERR: UNKNOWN CODE " + std::to_string(i));
  }
}

string Telecomm::verboseStatus(){
  return verboseStatus(ret);
}
// END
