#include "ClientSim.h"

#include <iostream>

using namespace std;
ClientSim::ClientSim()
{
	client = new Acheteur();
}

void ClientSim::miseAJour(HANDLE mutex)
{
	//do something
	cout << endl << "Mise à jour: " << client;
	WaitForSingleObject(mutex, INFINITE);
	//do something with mutex
	ReleaseMutex(mutex);
}

ClientSim::~ClientSim()
{
}
