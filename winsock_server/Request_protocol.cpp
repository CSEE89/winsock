#include "stdafx.h"
#include"Request_protocol.h"

Mail_request::Mail_request(const int port):Server_socket(port){
		s_bind();
		s_listen();
		s_accept();
};
void Mail_request::recv_mailFrom(){
	char buffer[BUFFER_LEN];
	memset(buffer, 0, BUFFER_LEN);
	s_receive(buffer, BUFFER_LEN);
	mail.mailfrom=getdata(buffer);
};
void Mail_request::recv_mailTo(){
	char buffer[BUFFER_LEN];
	memset(buffer, 0, BUFFER_LEN);
	s_receive(buffer, BUFFER_LEN);
	mail.mailto=getdata(buffer);
};
void Mail_request::recv_data(){
	char buffer[BUFFER_LEN];
	memset(buffer, 0, BUFFER_LEN);
	s_receive(buffer, BUFFER_LEN);
	if (string(buffer, strlen(DT)).compare(DT) == 0)
	{
		string s(buffer, strnlen(buffer, BUFFER_LEN));
		int pos1 = s.find_first_of(SP) + 1;
		int pos2 = s.find_first_of(CRLF);
		mail.data = s.substr(pos1,pos2-pos1);
	}
	else{
		throw "Data read error";
		}
};

void Mail_request::send_connectRe(){
	s_send(connectOK);
};
void Mail_request::send_mailFromRe(){
	s_send(mailfromOK);
};
void Mail_request::send_mailToRe(){
	s_send(mailtoOK);
};
void Mail_request::send_dataRe(){
	s_send(dataOK);
};
MailRequest Mail_request::getMail(){
	return mail;
};
bool Mail_request::responcheck(const std::string c){
	char inbuff[100];
	memset(inbuff, 0, 100);
	recv(Socket, inbuff, 100, 0);
	std::string s(inbuff, strnlen(inbuff, 100));
	//if (compare(inbuff,c))
	if (c.compare(s) == 0)
		return true;
	return false;
};
std::string Mail_request::getdata(const string &s){
	int firstpos = s.find_first_of(SP)+1;
	int pos1=s.find_first_of(SP,firstpos)+1;
	int pos2 = s.find_first_of(CRLF);
	string temp=s.substr(pos1,pos2-pos1);
	return temp;
};