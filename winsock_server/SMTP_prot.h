//client
#include"general_traits.h"
#define HELLO "HELLO"
#define MAIL FROM
#define MAIL TO

// SMTP protokoll 
class SMTP_prot: public BASE_SOCKET {

	WSADATA WsaDat;
	SOCKADDR_IN SockAddr;
	string subject;
	const MailRequest& mail;
public:
	SMTP_prot(const MailRequest& m);
	~SMTP_prot();
	void connectTo(const char *host, int port); // csatlakoz�s SMTP serverhez
	void HELO();  //SMTP HELO �zenet k�ld�se
	void MAILFROM(); //SMTP MAIL FROM �zenet
	void RCPT_TO();  //SMTP RCPT �zenet 
	void DATA();  // SMTP data �zenet 
	void Subject(); // Lev�l t�rgya
	void send_body(); // lev�l tartalma
	void bodyend();  //lev�l v�ge �zenet
	void QUIT();
	void s_receive();	
private:
	void SMTPresponse(const int code); // SMTP server v�lasz�zenet�nek fogad�sa, hibak�d ki�r�sa
};
// File kezel�s 
class DataBase{
	std::vector<MailRequest> messages; //beolvasott �zenet vektor
	const char *filename;
public:
	DataBase(const char* fn);
	void sendtoSMTP(int &mailID); // el nem k�ld�tt �zenetek tov�bb�t�sa
	void fileRead();  // file tartalm�nak kiolvas�sa messages vectorba
	void writeChanges();  //file friss�t�se
	void setprocessed(const int id); // tov�bb k�ld�tt e-mail adatainak frisss�t�se
};