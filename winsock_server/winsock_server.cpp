// MAIN //

#include "stdafx.h"
#include "Request_protocol.h"
#include"Server_socket.h"
#include"SMTP_prot.h"

static int mailID = 0; //e-mail-ek egyedi azono�t�ja
std::mutex mtx; //thread sync

// Kliens csatlakoz�sakor ind�tott sz�l
void clientThread(SOCKET &socket){
	try{
		// e-mail k�r�s fogad�sa
		Mail_request server(socket); // e-mail k�r�s fogad�s , saj�t protokollal
		MailRequest mail; //adat�rol�s
		server.send_connectRe();
		server.recv_mailFrom();
		server.send_mailFromRe();
		server.recv_mailTo();
		server.send_mailToRe();
		server.recv_data();
		mail = server.getMail(); //fogadott k�r�s 
		mailID++;
		mail.id = mailID;  //egyedi azonos�t� minden e-mail-hez
		mtx.lock();
		std::ofstream datafile;
		//fogadott k�r�s elemnet�se
		datafile.open("data.txt", std::ofstream::app);
		if (datafile.is_open())
		{
			datafile << mail;
		}
		datafile.close();
		mtx.unlock();

		//SMTP kommunik�ci�, e-mail tov�bbk�ld�s
		SMTP_prot smtp(mail);
		smtp.connectTo("localhost", 25);  //25-�s porton k�ldi az SMTP servernek
		smtp.HELO(); //HELO uzenet k�ld�s, hib�s v�lasz eset�n ki�rja a hiba�zenetet
		smtp.MAILFROM();
		smtp.RCPT_TO();
		smtp.DATA();
		smtp.Subject();
		smtp.send_body();
		smtp.bodyend();
		smtp.QUIT();

		//elmentett k�r�s feldolgozottnak jel�l�se
		DataBase file("data.txt");
		file.setprocessed(mail.id);
	}
	catch (const char* error)
	{
		std::cerr << error;
	}
	

}
// program ind�t�skor a tov�bb nem k�ld�tt �zenetek elk�ld�se
void sendunprocessed(const char* fn){
	DataBase file("data.txt");
	file.fileRead();
	file.sendtoSMTP(mailID);
	file.writeChanges();
}

int main()
{	
	
	int nClient(0); //kilensek sz�ma
	std::vector<SOCKET> clients; //kliens Socketek
	std::vector<std::thread*> threads; //minden kliens csatlakoz�sakor �j sz�l

	sendunprocessed("data.txt"); //kor�bbi fel nem dolgozott �zenetek k�ld�se
	Server_socket ServerSocket; // Szerver socket
	try{
		ServerSocket.init(8888).s_bind().s_listen(); //socket inicializ�l�s, bind, listen
	}
	catch (const char* error){
		std::cerr << error;
	}
	SOCKET TempSock = SOCKET_ERROR;
	std::thread* t;
	while (true) //itt v�rjuk a kilenseket
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
		
		t=new std::thread(clientThread,clients.back());  // �j sz�l a kilensnek
		std::cout << "Client connected";
		threads.push_back(t);
		nClient++;
		for (auto& th : threads)
		{
			if (th->joinable())  // sz�lak befejez�se
			th->join(); 
		}
	}

	while (!threads.empty()) delete threads.back(), threads.pop_back(); //sz�lak t�rl�se

	system("PAUSE");
	return 0;
}