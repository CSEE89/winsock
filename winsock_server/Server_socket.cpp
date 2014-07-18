#include "stdafx.h"
#include"Server_socket.h"

Server_socket::Server_socket(){};

Server_socket::~Server_socket(){ shutdown(Socket, SD_SEND); closesocket(Socket); WSACleanup(); };

Server_socket& Server_socket::init(const int port){//Winsock inicializálás
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
	return *this;
};
Server_socket& Server_socket::s_bind(){
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
	return *this;
};

void Server_socket::s_listen(){ listen(Socket, 1); };

void Server_socket::s_accept(SOCKET &TempSock){
	
	TempSock = SOCKET_ERROR;
	while (TempSock == SOCKET_ERROR)
	{
		TempSock = accept(Socket, NULL, NULL);
	}
};
void Server_socket::s_send(const char* message){
	int iResult = send(Socket, message, strlen(message), 0);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"send failed with error: %d\n", WSAGetLastError());
		closesocket(Socket);
		WSACleanup();
		throw "send error";
	}
};

char* Server_socket::s_receive(char* message, const int size){
	int lasterror=recv(Socket, message, size, 0); 
	if (lasterror==SOCKET_ERROR) {
		lasterror = WSAGetLastError();
		std::cerr << "recv error: " << lasterror;
		closesocket(Socket);
		WSACleanup();
		throw "recv error: ";
	}
};



