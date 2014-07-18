// MAIN //

#include "stdafx.h"
#include "Request_protocol.h"
#include"Server_socket.h"
#include"SMTP_prot.h"


std::mutex mtx; //thread sync
static int mailID=0;
Store messages;

void clientThread(SOCKET &socket){
	try{
		// e-mail kérés fogadása
		Mail_request server(socket);
		MailRequest mail; //adatárolás
		server.send_connectRe();
		server.recv_mailFrom();
		server.send_mailFromRe();
		server.recv_mailTo();
		server.send_mailToRe();
		server.recv_data();
		mail = server.getMail();
		// E-mail kérés elmentése
		mtx.lock();
		mailID++;
		mail.id = mailID;
		std::ofstream datafile;
		datafile.open("data.txt", std::ofstream::app);
		messages.add(mail);
		if (datafile.is_open())
		{
			datafile << mail;
		}
		datafile.close();
		mtx.unlock();

		//SMTP kommunikáció, e-mail továbbküldés
		SMTP_prot smtp(mail);
		smtp.connectTo("localhost", 25);
		smtp.HELO();
		smtp.s_receive();
		smtp.MAILFROM();
		smtp.s_receive();
		smtp.RCPT_TO();
		smtp.DATA();
		smtp.Subject();
		smtp.send_body();
		smtp.bodyend();
		smtp.QUIT();

		//elmentett kérés feldolgozottnak jelölése


	
	}
	catch (const char* error)
	{
		std::cerr << error;
	}
	

}
void fileRead(const char* fn)
{
	char buff[100];
	std::ifstream myReadFile;
	myReadFile.open(fn);
	int cnt(0);
	MailRequest mail;
	if (myReadFile.is_open()) {
		while (!myReadFile.eof()) {					
			myReadFile.getline(buff, 100);
			switch (cnt)
			{
			case 0:
				mail.id = (int)buff[0]-48;
				break;
			case 1:
				mail.processed=buff[0]-48;
				break;
			case 2:
				mail.mailfrom.append(buff);
				break;
			case 3:
				mail.mailto.append(buff);
				break;
			case 4:
				while (string(buff).compare(DATAEND) != 0)
				{
					myReadFile.getline(buff, 100);
					if (string(buff).compare(DATAEND) != 0)
					mail.data.append(buff);
				}
				cnt = -1;
				mail.data.clear();
				std::cout << mail;
				break;
			}
			cnt++;
		}
	}
	myReadFile.close();
}


int main()
{	
	
	//korábbi fel nem dolgozott üzenetek küldése
	fileRead("data.txt");


	//Socket létrehozás
	int nClient(0);
	std::vector<SOCKET> clients;
	std::vector<std::thread*> threads;
	Server_socket ServerSocket;
	try{
		ServerSocket.init(8888).s_bind().s_listen();
	}
	catch (const char* error){
		std::cerr << error;
	}
	SOCKET TempSock = SOCKET_ERROR;
	std::thread* t;
	while (true)
	{
		std::cout << "wating for connection" << std::endl;
		ServerSocket.s_accept(TempSock);
		clients.push_back(TempSock);
		if (TempSock == INVALID_SOCKET)
		{
			int nret = WSAGetLastError();
			WSACleanup();
			std::cerr << "accept hiba"<<nret;
		}
		
		t=new std::thread(clientThread,clients.back());
		std::cout << "Client connected";
		threads.push_back(t);
		nClient++;
		for (auto& th : threads)
		{
			if (th->joinable())
			th->join(); 
		}
	}


	

	system("PAUSE");
	return 0;
}