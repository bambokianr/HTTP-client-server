#ifndef HTTPREQ_H
#define HTTPREQ_H

#include <iostream>
#include <string>
#include <sstream>

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

    HTTPReq();

    //void parseMessage(string clientMessage);
    void parseURL(string url);
    void buildMessage(string method);
    void encodeMessage(string message);
    void decodeMessage(const uint8_t *bytecode);

    string getProtocol();
    string getHostname();
    string getPort();
    string getObjectPath();
    bool isValid();

  private:
    string protocol;
    string hostname;
    string port;
    string object_path;
    bool valid;
};

HTTPReq::HTTPReq() {
  message = "";
  protocol = "";
  hostname = "";
  port = "";
  object_path = "";
  valid = true;
}

//void HTTPReq::parseMessage(string clientMessage) {
//  object_path = clientMessage.substr(clientMessage.find(" ")+1, clientMessage.find(" HTTP")-4);
//  if(object_path == "/") object_path = "/index.html";

//  string completeAddress = clientMessage.substr(clientMessage.find("Host: ")+6, message.find("\r\n")-4);
//  hostname = completeAddress.substr(0, completeAddress.find(":"));
//}

void HTTPReq::parseURL(string url) {
    string token;
    //URL url_parse;

    stringstream ss_url(url);
    while (getline(ss_url, token, ':')) {
        if (token == "http" || token == "https")
            protocol = token;
        else if (token[0] == '/' && token[1] == '/') {
            if (token.size() > 1) {
                token.erase(0, 1);
                token.erase(0, 1);
            }
            hostname = token;
        }
        else {
            port = token.substr(0, token.find('/'));
            if (token.size() > port.size())
                object_path = token.substr(token.find('/'), token.size() - port.size());
        }
    }

    if (protocol == "" || hostname == "" || port == "")
        valid = false;
    if (object_path == "/")
        object_path = "/index.html";
}

void HTTPReq::buildMessage(string method) {
  //parseURL(url);
  string requestLine = method + " " + object_path + " HTTP/1.1\r\n";
  string headerLine = "Host: " + hostname + "\r\n";
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

string HTTPReq::getProtocol() {
    return protocol;
}

string HTTPReq::getHostname() {
    return hostname;
}

string HTTPReq::getPort() {
    return port;
}

string HTTPReq::getObjectPath() {
    return object_path;
}

bool HTTPReq::isValid() {
    return valid;
}

#endif
