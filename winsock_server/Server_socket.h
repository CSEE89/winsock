#include"general_traits.h"

class Server_socket{
	WSADATA WsaDat;
	
	SOCKADDR_IN serverInf;
	char inbuffer[BUFFER_LEN];
	int nClient;
	//SOCKET Socket[MAX_CLIENTS];
	//SOCKET ServerSocket;

public:
	SOCKET Socket;
	Server_socket();
	~Server_socket();
	Server_socket& init(const int port);
	Server_socket& s_bind();
    void  s_listen();
	void s_accept(SOCKET &TempSock);
	void s_send(const char* message);
	char* s_receive(char* message,const int size);



};