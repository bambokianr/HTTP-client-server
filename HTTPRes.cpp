#ifndef HTTPRES_H
#define HTTPRES_H

#include <iostream>
#include <string>
#include <time.h>

using namespace std;

class HTTPRes {
  public:
    string message;
    string status;

    HTTPRes();

    void setStatus(string value);
    void buildMessage(string content, int file_size);

  private:
    string getTime();
};

HTTPRes::HTTPRes() {
  message = "";
  status = "";
}

void HTTPRes::setStatus(string value) {
  status = value;
}

void HTTPRes::buildMessage(string content, int file_size) {
  // ! without content yet
  string responseHeader = "HTTP/1.1 " + status + " \r\n";
  responseHeader += "Date: " + getTime() + " \r";
  responseHeader += "Content-Length: " + to_string(file_size) + " \r\n";
  string responseBody = content + " \r\n";
  message = responseHeader + "\r\n" + responseBody;
}

string HTTPRes::getTime() {
  time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  //printf ( "Data atual do sistema é: %s", asctime (timeinfo) )

  return asctime (timeinfo);
}

#endif
