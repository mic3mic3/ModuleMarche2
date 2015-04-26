#include "ClientSim.h"

#include <iostream>

using namespace std;
ClientSim::ClientSim(int num)
{
	this->num = num;
	client = new Acheteur();
}

void ClientSim::miseAJour(HANDLE mutex)
{
	//do something
	WaitForSingleObject(mutex, INFINITE);
	cout << endl << "Mise a jour: " << num;
	
	//do something with mutex
	ReleaseMutex(mutex);
}

ClientSim::~ClientSim()
{
}
