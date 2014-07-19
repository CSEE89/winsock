// MAIN //

#include "stdafx.h"
#include "Request_protocol.h"
#include"Server_socket.h"
#include"SMTP_prot.h"

static int mailID = 0; //e-mail-ek egyedi azonoítója
std::mutex mtx; //thread sync

// Kliens csatlakozásakor indított szál
void clientThread(SOCKET &socket){
	try{
		// e-mail kérés fogadása
		Mail_request server(socket); // e-mail kérés fogadás , saját protokollal
		MailRequest mail; //adatárolás
		server.send_connectRe();
		server.recv_mailFrom();
		server.send_mailFromRe();
		server.recv_mailTo();
		server.send_mailToRe();
		server.recv_data();
		mail = server.getMail(); //fogadott kérés 
		mailID++;
		mail.id = mailID;  //egyedi azonosító minden e-mail-hez
		mtx.lock();
		std::ofstream datafile;
		//fogadott kérés elemnetése
		datafile.open("data.txt", std::ofstream::app);
		if (datafile.is_open())
		{
			datafile << mail;
		}
		datafile.close();
		mtx.unlock();

		//SMTP kommunikáció, e-mail továbbküldés
		SMTP_prot smtp(mail);
		smtp.connectTo("localhost", 25);  //25-ös porton küldi az SMTP servernek
		smtp.HELO(); //HELO uzenet küldés, hibás válasz esetén kiírja a hibaüzenetet
		smtp.MAILFROM();
		smtp.RCPT_TO();
		smtp.DATA();
		smtp.Subject();
		smtp.send_body();
		smtp.bodyend();
		smtp.QUIT();

		//elmentett kérés feldolgozottnak jelölése
		DataBase file("data.txt");
		file.setprocessed(mail.id);
	}
	catch (const char* error)
	{
		std::cerr << error;
	}
	

}
// program indításkor a tovább nem küldött üzenetek elküldése
void sendunprocessed(const char* fn){
	DataBase file("data.txt");
	file.fileRead();
	file.sendtoSMTP(mailID);
	file.writeChanges();
}

int main()
{	
	
	int nClient(0); //kilensek száma
	std::vector<SOCKET> clients; //kliens Socketek
	std::vector<std::thread*> threads; //minden kliens csatlakozásakor új szál

	sendunprocessed("data.txt"); //korábbi fel nem dolgozott üzenetek küldése
	Server_socket ServerSocket; // Szerver socket
	try{
		ServerSocket.init(8888).s_bind().s_listen(); //socket inicializálás, bind, listen
	}
	catch (const char* error){
		std::cerr << error;
	}
	SOCKET TempSock = SOCKET_ERROR;
	std::thread* t;
	while (true) //itt várjuk a kilenseket
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
		
		t=new std::thread(clientThread,clients.back());  // új szál a kilensnek
		std::cout << "Client connected";
		threads.push_back(t);
		nClient++;
		for (auto& th : threads)
		{
			if (th->joinable())  // szálak befejezése
			th->join(); 
		}
	}

	while (!threads.empty()) delete threads.back(), threads.pop_back(); //szálak törlése

	system("PAUSE");
	return 0;
}