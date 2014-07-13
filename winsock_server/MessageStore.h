#include"general_traits.h"


class Store{

	std::vector<MailRequest> store;

public:
	Store();
	void add(){}
	bool save(){} //komplett kérés elmentése
	bool read(){}
	bool markProcessed(const MailRequest &req){}
};
