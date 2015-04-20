#ifndef CLIENTSIM_H
#define CLIENTSIM_H

#include <Windows.h>
#include "Client.h"
#include "Acheteur.h"

class ClientSim
{
private:
	Client* client;
public:
	ClientSim();
	~ClientSim();

	void miseAJour(HANDLE);
};

#endif