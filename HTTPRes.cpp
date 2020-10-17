#ifndef HTTPRES_H
#define HTTPRES_H

#include <iostream>
#include <string>
#include <time.h>

using namespace std;

class HTTPRes {
  public:
    unsigned char *message;
    unsigned char message_array[1048576];
    string status;

    HTTPRes();

    void setStatus(string value);
    void buildMessage(unsigned char* content, int file_size);
    void parseMessage(unsigned char* serverMessage);
    string getObjectStatus();
    string getObjectLength();
    unsigned char* getObjectContent();
    unsigned char object_content_array[1048576];

  private:
    string object_status;
    string object_length;
    unsigned char *object_content;

    string getTime();
};

HTTPRes::HTTPRes() {
  message = NULL;
  status = "";

  object_status = "";
  object_length = "";
  object_content = NULL;
}

void HTTPRes::setStatus(string value) {
  status = value;
}

void HTTPRes::buildMessage(unsigned char* content, int file_size) {
  unsigned char responseHeader[2048];

  string headerTemp = "HTTP/1.1 " + status + " \r\n" + "Date: " + getTime();
  copy(headerTemp.begin(), headerTemp.end(), responseHeader);
  responseHeader[headerTemp.length()] = 0;

  if(strcmp((char*)content, "") != 0 && file_size != 0) {
    headerTemp += "Content-Length: " + to_string(file_size) + " \r\n\r\n";
    copy(headerTemp.begin(), headerTemp.end(), responseHeader);
    responseHeader[headerTemp.length()] = 0;

    message = responseHeader;

    string message_temp ((char *)message);
    unsigned char message_temp_array[message_temp.length() + file_size];
    copy(message_temp.begin(), message_temp.end(), message_temp_array);
    message_temp_array[message_temp.length() + file_size] = 0;

    memcpy(message_temp_array+message_temp.length(), content, file_size);
    message = message_temp_array;

    memcpy(message_array, message_temp_array, sizeof(message_temp_array));
    message = message_array;
  } else {
    headerTemp += "\r\n";
    copy(headerTemp.begin(), headerTemp.end(), responseHeader);
    responseHeader[headerTemp.length()] = 0;

    message = responseHeader;

    string message_temp ((char *)message);
    unsigned char message_temp_array[message_temp.length()];
    copy(message_temp.begin(), message_temp.end(), message_temp_array);
    message_temp_array[message_temp.length()] = 0;

    memcpy(message_temp_array+message_temp.length(), content, file_size);
    message = message_temp_array;

    memcpy(message_array, message_temp_array, sizeof(message_temp_array));
    message = message_array;
  }
}

string HTTPRes::getObjectStatus() {
    return object_status;
}

string HTTPRes::getObjectLength() {
    return object_length;
}

unsigned char* HTTPRes::getObjectContent() {
    return object_content_array;
}

void HTTPRes::parseMessage(unsigned char* serverMessage) {
  string message_temp ((char *)serverMessage);

  object_status = message_temp.substr(message_temp.find(" ")+1, message_temp.find("\r\n")-message_temp.find(" ")-2);

  if (object_status == "200 OK") {
    object_length = message_temp.substr(message_temp.find("Content-Length: ")+16, message_temp.find("\r\n\r\n")-message_temp.find("Content-Length: ")-16);

    int aux = message_temp.find("\r\n\r\n")+4;
    for (unsigned int i = aux; i < message_temp.find("\r\n\r\n")+4 + stoi(object_length); i++)
      object_content_array[i - aux] = serverMessage[i];
    object_content_array[stoi(object_length)] = 0;
    object_content = object_content_array;
  }
}

string HTTPRes::getTime() {
  time_t rawtime;
  struct tm* timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  return asctime(timeinfo);
}

#endif
