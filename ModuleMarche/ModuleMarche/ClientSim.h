#ifndef CLIENTSIM_H
#define CLIENTSIM_H

#include <Windows.h>
#include "Client.h"
#include "Acheteur.h"

class ClientSim
{
private:
	int num;
public:
	ClientSim(int);
	~ClientSim();

	void miseAJour(HANDLE);
};

#endif