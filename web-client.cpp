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

using namespace std; 

int main(int argc, char *argv[]) { 
  for (int i = 1; i < argc; ++i) 
    cout << argv[i] << endl; 
}