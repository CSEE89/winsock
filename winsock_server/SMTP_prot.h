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
	void connectTo(const char *host, int port); // csatlakozás SMTP serverhez
	void HELO();  //SMTP HELO üzenet küldése
	void MAILFROM(); //SMTP MAIL FROM üzenet
	void RCPT_TO();  //SMTP RCPT üzenet 
	void DATA();  // SMTP data üzenet 
	void Subject(); // Levél tárgya
	void send_body(); // levél tartalma
	void bodyend();  //levél vége üzenet
	void QUIT();
	void s_receive();	
private:
	void SMTPresponse(const int code); // SMTP server válaszüzenetének fogadása, hibakód kiírása
};
// File kezelés 
class DataBase{
	std::vector<MailRequest> messages; //beolvasott üzenet vektor
	const char *filename;
public:
	DataBase(const char* fn);
	void sendtoSMTP(int &mailID); // el nem küldött üzenetek továbbítása
	void fileRead();  // file tartalmának kiolvasása messages vectorba
	void writeChanges();  //file frissítése
	void setprocessed(const int id); // tovább küldött e-mail adatainak frisssítése
};