#include "Simulation.h"

#include <Windows.h>
#include <winnt.h>
#include <ctime>
#include <iostream>
#include "ClientApp.h"
using namespace std;
Simulation::Simulation()
{
	clientApp = ClientApp();
	srand(time(NULL));
}


Simulation::~Simulation()
{
}

void Simulation::miseAJour()
{
	int chanceClient = rand() % 12 + 1;
	if (chanceClient == 1)
	{
		//DWORD* idThread = new DWORD();
		clients.push_back(new ClientSim(clients.size()));
		connexion()
		//threads.push_back(CreateThread(0, 0, appelClient, client, 0, idThread));
	}
	HANDLE* t = new HANDLE[clients.size()];
	/*for (unsigned int cpt = 0; cpt < threads.size(); cpt++)
	{
		WaitForSingleObject(threads[cpt], INFINITE);
	}*/
	for (unsigned int cpt = 0; cpt < clients.size(); cpt++)
	{
		DWORD* idThread = new DWORD();
		//clients.push_back(new ClientSim());
		t[cpt] = CreateThread(0, 0, appelClient, clients[cpt], 0, idThread);
		//threads.push_back(CreateThread(0, 0, appelClient, client, 0, idThread));
	}
	WaitForMultipleObjects(clients.size(), t,true,INFINITE);
	cout << endl << "----------" << endl;
	delete[] t;
}
HANDLE mutex=CreateMutex(NULL,false,NULL);
DWORD WINAPI appelClient(LPVOID client)
{
	((ClientSim*)client)->miseAJour(mutex);
	return 0;
}
/*DWORD WINAPI Simulation::appelClient(LPVOID client)
{
	((ClientSim*)client)->miseAJour(mutex);
	return 0;
}*/

int Simulation::getHeures()
{
	return heures;
}