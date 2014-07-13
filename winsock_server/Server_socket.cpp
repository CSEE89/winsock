#include "stdafx.h"
#include"Server_socket.h"

Server_socket::Server_socket(const int port){	//Winsock inicializálás
	nClient = 0;
	//ServerSocket = NULL;
	if (WSAStartup(MAKEWORD(2, 2), &WsaDat) != 0)
	{
		WSACleanup();
		throw "WSA Initialization failed!\r\n";
	}
	//Socket létrehozás
	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Socket == INVALID_SOCKET)
	{
		WSACleanup();
		throw "Socket creation failed.\r\n";
	}
	//Socket címstruktúra beállítás
	serverInf.sin_family = AF_INET;
	serverInf.sin_addr.s_addr = INADDR_ANY;
	serverInf.sin_port = htons(port);
	
};
Server_socket::~Server_socket(){ shutdown(Socket, SD_SEND); closesocket(Socket); WSACleanup(); };

bool Server_socket::s_bind(){
	/*if (bind(ServerSocket, (LPSOCKADDR)&serverInf, sizeof(serverInf)) == SOCKET_ERROR)
	{
		WSACleanup();
		throw "Unable to bind socket!\r\n";
	}*/
	if (bind(Socket, (SOCKADDR*)(&serverInf), sizeof(serverInf)) == SOCKET_ERROR)
	{		
		WSACleanup();
		throw "Unable to bind socket!\r\n";
	}
	return true;
};

void Server_socket::s_listen(){ listen(Socket, 1); };

void Server_socket::s_accept(){
	/*
	if (nClient<nMaxClients)
	{
		int size = sizeof(sockaddr);
		Socket[nClient] = accept(wParam, &sockAddrClient, &size);
		if (Socket[nClient] == INVALID_SOCKET)
		{
			int nret = WSAGetLastError();
			WSACleanup();
		}
		std::cout << "Client connected!\r\n\r\n";
	}
	nClient++; */
 
	SOCKET TempSock = SOCKET_ERROR;
	while (TempSock == SOCKET_ERROR)
	{
		std::cout << "Waiting for incoming connections...\r\n";
		TempSock = accept(Socket, NULL, NULL);
	}
	Socket = TempSock;
	
	std::cout << "Client connected!\r\n\r\n";
};
void Server_socket::s_send(const char* message){ send(Socket, message, strlen(message), 0); }
//void Server_socket::s_receive(char* message){ recv(Socket, message,strlen(message), 0); }
char* Server_socket::s_receive(char* message, const int size){ recv(Socket, message, size, 0); return message; }



