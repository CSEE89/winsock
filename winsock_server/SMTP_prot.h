//client
#include"general_traits.h"
#define HELLO "HELLO"
#define MAIL FROM
#define MAIL TO


class SMTP_prot: public BASE_SOCKET {

	WSADATA WsaDat;
	SOCKADDR_IN SockAddr;
	string realName;// ????
	string subject;
	const MailRequest& mail;
public:
	SMTP_prot(const MailRequest& m);
	void connectTo(const char *host, int port);
	void HELO();
	void MAILFROM();
	void RCPT_TO();
	void FROM();
	void DATA();
	void Subject();
	void send_body();
	void bodyend();
	void QUIT();
	void s_receive();

private:
	bool responcheck(const std::string c);
};