#ifndef GENERAL
#define  GENERAL

#include <winsock2.h>
#include <iostream>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include<string>
#include<vector>
#include<fstream>

#define BUFFER_LEN 100
#define MAX_CLIENTS 10
// Kérés válaszüzenetei
#define connectOK "connect ok \r\n"
#define mailtoOK "mailto ok \r\n"
#define mailfromOK "mailfrom ok \r\n" 
#define dataOK "data end \r\n"
#define MF "MAIL FROM"
#define MT "MAIL TO"
#define DT "DATA"
#define SP " "
#define CRLF "\r\n"

//class Server_Socket;
//class Mail_request : public Server_socket;

using namespace std;
struct MailRequest{
	int id;
	bool processed = false;
	string mailto;
	string mailfrom;
	string data;
};



#endif GENERAL