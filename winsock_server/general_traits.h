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
#include<thread>
#include<mutex>

#define BUFFER_LEN 100
#define MAX_CLIENTS 10
// Kérés válaszüzenetei
#define connectOK "connect ok \r\n"
#define mailtoOK "mailto ok \r\n"
#define mailfromOK "mailfrom ok \r\n" 
#define dataOK "data end \r\n"
#define H "HELO"
#define MF "MAIL FROM: "
#define RCPT "RCPT TO: "
#define MT "MAIL TO"
#define DT "DATA: "
#define SP " "
#define CRLF "\r\n"
#define DATAEND "DATAEND"
#define HOSTNAME "SMTP host"


typedef std::string string;

// e-mail üzenet tárolása történik ezzel az osztállyal
class MailRequest{
public:
	MailRequest(){
		id = 0;
		processed = 0;		
	}
	MailRequest(const MailRequest &m){ 
		this->id = m.id; 
		this->processed = m.processed; 
		this->mailto = m.mailto; 
		this->mailfrom = m.mailfrom; 
		this->data = m.data; 
	}
	int id;
	bool processed = false;
	string mailto;
	string mailfrom;
	string data;
	void clear(){
		id = 0; processed = 0; mailto.clear();
		mailfrom.clear(); data.clear();
	}
};
// kiró oprátor
inline std::ostream &operator<<(std::ostream &a, const MailRequest &mail)
{
	a << mail.id << std::endl;
	a << mail.processed << std::endl;
	a << mail.mailto << std::endl;
	a << mail.mailfrom << std::endl;
	a << mail.data << std::endl;
	a << DATAEND<<std::endl;
	return a;
};
class BASE_SOCKET{
public:
	SOCKET Socket = NULL;

	virtual void s_send(const char* message){
		int iResult = send(Socket, message, strlen(message), 0);
		if (iResult == SOCKET_ERROR) {
			wprintf(L"send failed with error: %d\n", WSAGetLastError());
			closesocket(Socket);
			WSACleanup();
			throw "send error";
		}
	}
	virtual void s_recv(char* message, const int size){
		memset(message, 0, size);
		int lasterror = recv(Socket, message, size, 0);
		if (lasterror == SOCKET_ERROR) {
			lasterror = WSAGetLastError();
			std::cerr << "recv error: " << lasterror;
			closesocket(Socket);
			WSACleanup();
			throw "recv error: ";
		}
	}
};
#endif GENERAL