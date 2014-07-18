#include "stdafx.h"
#include"Request_protocol.h"

Mail_request::Mail_request(SOCKET &s) :socket(s){};

void Mail_request::recv_mailFrom(){
	char buffer[BUFFER_LEN];
	memset(buffer, 0, BUFFER_LEN);
	m_recv(buffer, BUFFER_LEN);
	mail.mailfrom = getdata(buffer);
};
void Mail_request::recv_mailTo(){
	char buffer[BUFFER_LEN];
	memset(buffer, 0, BUFFER_LEN);
	m_recv(buffer, BUFFER_LEN);
	mail.mailto = getdata(buffer);
};
void Mail_request::recv_data(){
	char buffer[1000];
	memset(buffer, 0, 1000);
	m_recv(buffer, 1000);
	if (string(buffer, strlen("DATA")).compare("DATA") == 0)
	{
		int real_len = strnlen(buffer, BUFFER_LEN);
		string s(buffer, real_len);
		int pos1 = s.find_first_of(SP) + 1;
		mail.data = s.substr(pos1, real_len - pos1);
	}
	else{
		throw "Data read error";
	}
};

void Mail_request::send_connectRe(){
	m_send(connectOK);
};
void Mail_request::send_mailFromRe(){
	m_send(mailfromOK);
};
void Mail_request::send_mailToRe(){
	m_send(mailtoOK);
};
void Mail_request::send_dataRe(){
	m_send(dataOK);
};
MailRequest Mail_request::getMail(){
	return mail;
};
void Mail_request::m_send(const char* message){
	int iResult = send(socket, message, strlen(message), 0);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"send failed with error: %d\n", WSAGetLastError());
		closesocket(socket);
		WSACleanup();
		throw "send error";
	}
};
void Mail_request::m_recv(char *message,const int size){
	int lasterror = recv(socket, message, size, 0); 
	if (lasterror == SOCKET_ERROR) {
		lasterror = WSAGetLastError();
		std::cerr << "recv error: " << lasterror;
		closesocket(socket);
		WSACleanup();
		throw "recv error: ";
	}
}
std::string Mail_request::getdata(const string &s){
	int firstpos = s.find_first_of(SP) + 1;
	int pos1 = s.find_first_of(SP, firstpos) + 1;
	int pos2 = s.find_first_of(CRLF);
	string temp = s.substr(pos1, pos2 - pos1);
	return temp;
};

void Store::add(const MailRequest& mail){ store.push_back(mail); };