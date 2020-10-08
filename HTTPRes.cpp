#ifndef HTTPRES_H
#define HTTPRES_H

#include <iostream>
#include <string>

using namespace std;

class HTTPRes {
  public:
    string message;
    string status;

    HTTPRes();
    
    void setStatus(string value);
    void buildMessage(string content);
};

HTTPRes::HTTPRes() {
  message = "";
  status = "";
}

void HTTPRes::setStatus(string value) {
  status = value;
}

void HTTPRes::buildMessage(string content) {
  // ! without content yet
  string responseLine = "HTTP/1.1 " + status + " \r\n";
  message = responseLine + "\r\n";
}

#endif 