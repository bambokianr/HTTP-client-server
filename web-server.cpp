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

#include "HTTPRes.cpp"

string convertURLtoIP(char* host) {
  struct addrinfo hints;
  struct addrinfo* res;

  memset(&hints, 0, sizeof(hints)); // limpa a struct
  hints.ai_family = AF_INET; // IPv4
  hints.ai_socktype = SOCK_STREAM; // TCP

  int status = 0;
  if ((status = getaddrinfo(host, "80", &hints, &res)) != 0) {
    cerr << "getaddrinfo: " << gai_strerror(status) << endl;
    return "error";
  } 

  char ipstr[INET_ADDRSTRLEN] = {'\0'};
  for(struct addrinfo* p = res; p != 0; p = p->ai_next) {
    struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
    inet_ntop(p->ai_family, &(ipv4->sin_addr), ipstr, sizeof(ipstr));
  }

  freeaddrinfo(res); // libera a memoria alocada dinamicamente para "res"

  return ipstr;
}

int main(int argc, char *argv[]) { 
  if (argc != 4) {
    cerr << "WRONG USAGE" << endl;
    cerr << "please, run './web-server [host] [port] [dir]'" << endl;
    return -1;
  }

  char* host = argv[1];
  int port = stoi(argv[2]);
  string dir = argv[3];

  string IPaddress = convertURLtoIP(host);

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  int yes = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    perror("setsockopt");
    return 1;
  }

  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);
  serverAddr.sin_addr.s_addr = inet_addr(IPaddress.c_str());
  memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

  if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
    perror("bind");
    return -1;
  }

  if (listen(sockfd, 1) == -1) {
    perror("listen");
    return -1;
  }

  //! ------------------------------------------------------------

  struct sockaddr_in clientAddr;
  socklen_t clientAddrSize = sizeof(clientAddr);
  int clientSockfd = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrSize);

  if (clientSockfd == -1) {
    perror("accept");
    return -1;
  }

  char ipstr[INET_ADDRSTRLEN] = {'\0'};
  inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, ipstr, sizeof(ipstr));
  cout << "Conexão iniciada com o cliente " << ipstr << ":" << ntohs(clientAddr.sin_port) << endl << endl;



  bool isEnd = false;
  char buf[1024] = {0};
  stringstream ss;

  while (!isEnd) {
    memset(buf, '\0', sizeof(buf));

    if (recv(clientSockfd, buf, 1024, 0) == -1) {
      perror("recv");
      return -1;
    }

    ss << buf << endl;
    cout << ss.str() << endl;
    // cout << buf << endl;

    //! envia a mensagem HTTP de resposta
    if (send(clientSockfd, buf, 1024, 0) == -1) {
      perror("send");
      return -1;
    }

    ss.str("");
  }

  close(clientSockfd);


  // HTTPRes response;
  // response.setStatus(200);
  // cout << response.status << endl;

  return 0;
}