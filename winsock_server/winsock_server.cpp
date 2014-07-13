// winsock_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Request_protocol.h"
#include<string>

std::ostream &operator<<(std::ostream &a, const MailRequest &mail)
{
	a << mail.processed << endl;;
	a << mail.mailto << endl;
	a << mail.mailfrom << endl;
	a << mail.data << endl;
	return a;
};


int main()
{
	try{
		Mail_request server(8888); //szerver socket létrehozás
		MailRequest mail; //adatárolás
	
		
		server.send_connectRe();
		server.recv_mailFrom();
		server.send_mailFromRe();
		server.recv_mailTo();
		server.send_mailToRe();
		server.recv_data();
		cout << server.getMail();
		mail = server.getMail();
	
	
	ofstream datafile;
	datafile.open("data.txt",std::ofstream::app);
	if (datafile.is_open())
	{
		datafile << mail;
	}
	datafile.close();
	}
	catch (const char* error)
	{
		std::cout << error;
	}
	system("PAUSE");
	return 0;
}