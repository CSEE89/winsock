#include"general_traits.h"


class Store{

	std::vector<MailRequest> store;

public:
	Store();
	void add(){}
	bool save(){} //komplett k�r�s elment�se
	bool read(){}
	bool markProcessed(const MailRequest &req){}
};
