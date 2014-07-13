//E-mail küldési kérés protokollja TCP fölött
#include"Server_socket.h"

 
class Mail_request:  public Server_socket{
	WSADATA WsaDat;
	SOCKET Socket;
	SOCKADDR_IN SockAddr;
	MailRequest mail;
public:
	Mail_request(const int port);
	//fogadás
	void recv_mailFrom();
	void recv_mailTo();
	void recv_data();
	//megerõsítés
	void send_connectRe();
	void send_mailFromRe();
	void send_mailToRe();
	void send_dataRe();
	//adat
	MailRequest getMail();
private:
	bool responcheck(const std::string c);
	std::string getdata(const string &s);
};