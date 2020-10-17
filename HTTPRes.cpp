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

    string getObjectStatus();
    string getObjectContent();
    void parseMessage(string serverMessage);

  private:
    string object_status;
    string object_length;
    string object_content;
    string getTime();
};

HTTPRes::HTTPRes() {
  message = "";
  status = "";

  object_status = "";
  object_length = "";
  object_content = "";
}

void HTTPRes::setStatus(string value) {
  status = value;
}

void HTTPRes::buildMessage(string content, int file_size) {
  string responseHeader = "HTTP/1.1 " + status + " \r\n";
  responseHeader += "Date: " + getTime();
  if(content != "" && file_size != 0) {
    responseHeader += "Content-Length: " + to_string(file_size) + " \r\n";
    message = responseHeader + "\r\n" + content;
  } else {
    message = responseHeader + "\r\n";
  }
}



string HTTPRes::getObjectStatus() {
    return object_status;
}

string HTTPRes::getObjectContent() {
    return object_content;
}

void HTTPRes::parseMessage(string serverMessage) {
  object_status = serverMessage.substr(serverMessage.find(" ")+1, serverMessage.find("\r\n")-serverMessage.find(" ")-2);
  //object_name = serverMessage.substr(serverMessage.find("Host: ")+6, serverMessage.find("\r\n")-4);
  object_length = serverMessage.substr(serverMessage.find("Content-Length: ")+16, serverMessage.find("\r\n\r\n")-serverMessage.find("Content-Length: ")-16);
  object_content = serverMessage.substr(serverMessage.find("\r\n\r\n")+4, stoi(object_length)-1);


  //if(object_path == "/") object_path = "/index.html";

  //string completeAddress = clientMessage.substr(clientMessage.find("Host: ")+6, message.find("\r\n")-4);
  //hostname = completeAddress.substr(0, completeAddress.find(":"));
}

string HTTPRes::getTime() {
  time_t rawtime;
  struct tm* timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  return asctime(timeinfo);
}

#endif
