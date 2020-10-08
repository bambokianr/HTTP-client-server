#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sstream>

#include "HTTPReq.cpp"
#include "HTTPRes.cpp"

using namespace std; 

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "WRONG USAGE" << endl;
    cerr << "please, run './web-client [URL] [URL] ...'" << endl;
    return -1;
  } 

  // for (int i = 1; i < argc; ++i) 
  //   cout << argv[i] << endl; 
  string url = argv[1];

  //! ------------------------------------------------
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(3000); //! estática??
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //! estática??
  memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

  if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
    perror("connect");
    return -1;
  }

  struct sockaddr_in clientAddr;
  socklen_t clientAddrLen = sizeof(clientAddr);
  if (getsockname(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen) == -1) {
    perror("getsockname");
    return -1;
  }

  char ipstr[INET_ADDRSTRLEN] = {'\0'};
  inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, ipstr, sizeof(ipstr));
  std::cout << "Configurar uma conexão a partir de " << ipstr << ":" << ntohs(clientAddr.sin_port) << std::endl;


  HTTPReq request;
  HTTPRes response;

  char buf[1024] = {0};
  memset(buf, '\0', sizeof(buf));
  request.buildMessage("GET", url);
  
  if (send(sockfd, request.message.c_str(), request.message.size(), 0) == -1) {
    perror("send");
    return -1;
  }

  if (recv(sockfd, buf, 1024, 0) == -1) {
    perror("recv");
    return -1;
  }

  cout << "RESPOSTA DO SERVIDOR: " << buf << endl;
  close(sockfd);

  return 0;
}