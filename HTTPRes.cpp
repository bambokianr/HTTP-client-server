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
    
    void setStatus(int statusCode);
};

HTTPRes::HTTPRes() {
  message = "";
  status = "";
}

void HTTPRes::setStatus(int statusCode) {
  cout << "statusCode " << statusCode << endl;
}

#endif 