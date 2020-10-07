#ifndef HTTPREQ_H
#define HTTPREQ_H

#include <iostream>
#include <string>

using namespace std;

// ! REQUISIÇÃO HTTP
// ! GET / HTTP/1.1\r\n
// ! Host: developer.mozilla.org\r\n
// ! \r\n

// ! lembrando que o path '/' significa '/index.html'

// ! para enviar a mensagem: converter a string em bytecode, uma vez que os sockets operam com buffers em bytes
// ! receber a mensagem: parse dos bytes recebidos e um método construtor para preencher as informações internas do request

class HTTPReq {
  public:
    string message;
    string host;
    string path;

    HTTPReq();
    
    void parseClientURL(string clientURL);
    void buildMessage(string method, string path, string host);
};

HTTPReq::HTTPReq() {
  message = "";
  host = "";
  path = "";
}

void HTTPReq::parseClientURL(string clientURL) {
  // ! separar a URL em host e path
  // ! verificar se o path é / -> gerar path /index.html
  cout << "parseClientURL: " << clientURL << endl;
}

void HTTPReq::buildMessage(string method, string path, string host) {
  string requestLine = method + " " + path + " HTTP/1.1\r\n";
  string headerLine = "Host: " + host + "\r\n";
  message = requestLine + headerLine + "\r\n";
}

#endif 