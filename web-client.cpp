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



char* showIP(char *url) {
  char* IP = new char[20];

  struct addrinfo hints;
  struct addrinfo* res;

  //if (argc != 2) {
  //  std::cerr << "usage: showip hostname" << std::endl;
  // return 1;
  //}

  // hints - modo de configurar o socket para o tipo  de transporte
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET; // IPv4
  hints.ai_socktype = SOCK_STREAM; // TCP

  // funcao de obtencao do endereco via DNS - getaddrinfo
  // funcao preenche o buffer "res" e obtem o codigo de resposta "status"
  int status = 0;
  if ((status = getaddrinfo(url, "80", &hints, &res)) != 0) {
    std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
    strcpy(IP, "error");
    return IP;
  }

  std::cout << "IP addresses for " << url << ": " << std::endl;

  for(struct addrinfo* p = res; p != 0; p = p->ai_next) {
    // a estrutura de dados eh generica e portanto precisa de type cast
    struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;

    // e depois eh preciso realizar a conversao do endereco IP para string
    char ipstr[INET_ADDRSTRLEN] = {'\0'};
    inet_ntop(p->ai_family, &(ipv4->sin_addr), ipstr, sizeof(ipstr));
    std::cout << "  " << ipstr << std::endl;

    freeaddrinfo(res); // libera a memoria alocada dinamicamente para "res"
    strcpy(IP, ipstr);
    return IP;
  }

  freeaddrinfo(res); // libera a memoria alocada dinamicamente para "res"
  strcpy(IP, "");
  return IP;
}

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
  serverAddr.sin_addr.s_addr = inet_addr(showIP(argv[1])); //! estática??
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
