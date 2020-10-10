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

struct URL {
  string protocol;
  string hostname;
  string port;
  string object;
  bool valid;
};

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

URL URLparse(string url) {
    string token;
    URL url_parse;
    url_parse.valid = true;

    stringstream ss_url(url);
    while (getline(ss_url, token, ':')) {
        if (token == "http" || token == "https")
            url_parse.protocol = token;
        else if (token[0] == '/' && token[1] == '/') {
            if (token.size() > 1) {
                token.erase(0, 1);
                token.erase(0, 1);
            }
            url_parse.hostname = token;
        }
        else {
            url_parse.port = token.substr(0, url.find('/') - 1);
            if (token.size() > url_parse.port.size())
                url_parse.object = token.substr(url.find('/'), token.size() - url_parse.port.size() - 1);
        }
    }

    if (url_parse.protocol == "" || url_parse.hostname == "" || url_parse.port == "")
        url_parse.valid = false;
    if (url_parse.object == "")
        url_parse.object = "index.html";

    //cout << url_parse.protocol << endl;
    //cout << url_parse.hostname << endl;
    //cout << url_parse.port << endl;
    //cout << url_parse.object << endl;
    //cout << url_parse.valid << endl;
    return url_parse;
}

char* string2charStar(string input) {
    char *output = new char[input.size()];

    for (unsigned int i = 0; i < input.size(); i++) {
        output[i] = input[i];
    }

    return output;
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

  URL url_parse;
  url_parse = URLparse(url);
  if (!url_parse.valid) {
    cerr << "WRONG URL" << endl;
    cerr << "please, give <protocol>://<hostname>:<port>/<object> ...'" << endl;
    return -1;
  }

  //! ------------------------------------------------
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(stoi(url_parse.port)); //! estática??
  serverAddr.sin_addr.s_addr = inet_addr(showIP(string2charStar(url_parse.hostname))); //! estática??
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
