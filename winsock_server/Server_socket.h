#include"general_traits.h"

class Server_socket{
	WSADATA WsaDat;
	SOCKET Socket;
	SOCKADDR_IN serverInf;
	char inbuffer[BUFFER_LEN];
	int nClient;
	//SOCKET Socket[MAX_CLIENTS];
	//SOCKET ServerSocket;

public:
	
	Server_socket(const int port);
	~Server_socket();
	bool s_bind();
    void  s_listen();
	void s_accept();
	void s_send(const char* message);
	char* s_receive(char* message,const int size);



};