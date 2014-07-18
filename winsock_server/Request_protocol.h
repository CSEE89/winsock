//E-mail küldési kérés protokollja TCP fölött
#include"general_traits.h"


class Mail_request{
	SOCKET &socket;
	MailRequest mail;
public:
	Mail_request(SOCKET &s);
	//fogadás
	void recv_mailFrom();
	void recv_mailTo();
	void recv_data();
	//megerõsítés
	void send_connectRe();
	void send_mailFromRe();
	void send_mailToRe();
	void send_dataRe();
	void m_send(const char* message);
	void m_recv(char *message, const int size);
	//adat
	MailRequest getMail();
private:
	
	string getdata(const std::string &s);
};

class Store{
	std::vector<MailRequest> store;
public:
	void add(const MailRequest& mail);
	bool save(); //komplett kérés elmentése
	bool read();
	bool markProcessed(const MailRequest &req);
};
