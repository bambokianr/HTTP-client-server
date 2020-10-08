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
    const uint8_t *bytecode;
    // vector<uint8_t> *bytecode;
    string host;
    string path;

    HTTPReq();
    
    void parseMessage(string clientMessage);
    void parseURL(string url);
    void buildMessage(string method, string url);
    void encodeMessage(string message);
    void decodeMessage(const uint8_t *bytecode);
};

HTTPReq::HTTPReq() {
  message = "";
  host = "";
  path = "";
}

void HTTPReq::parseMessage(string clientMessage) {
  path = clientMessage.substr(clientMessage.find(" ")+1, clientMessage.find(" HTTP")-4);
  if(path == "/") path = "/index.html";

  string completeAddress = clientMessage.substr(clientMessage.find("Host: ")+6, message.find("\r\n")-4);
  host = completeAddress.substr(0, completeAddress.find(":"));
}

void HTTPReq::parseURL(string url) {
  host = url.substr(0, url.find("/"));
  path = url.substr(url.find("/"));
}

void HTTPReq::buildMessage(string method, string url) {
  parseURL(url);
  string requestLine = method + " " + path + " HTTP/1.1\r\n";
  string headerLine = "Host: " + host + "\r\n";
  message = requestLine + headerLine + "\r\n";
}

void HTTPReq::encodeMessage(string message) {
  bytecode = reinterpret_cast<const uint8_t*>(message.c_str());
  cout << "bytecode " << bytecode << endl;
}

void HTTPReq::decodeMessage(const uint8_t *bytecode) {
  message.assign(bytecode, bytecode + sizeof(bytecode));
  cout << "message " << message << endl;
}

#endif 