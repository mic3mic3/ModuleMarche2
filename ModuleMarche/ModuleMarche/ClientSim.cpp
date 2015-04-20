#include "ClientSim.h"


ClientSim::ClientSim()
{
	client = new Acheteur();
}

void ClientSim::miseAJour(HANDLE mutex)
{
	//do something
	WaitForSingleObject(mutex, INFINITE);
	//do something with mutex
	ReleaseMutex(mutex);
}

ClientSim::~ClientSim()
{
}
