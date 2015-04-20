#include "Simulation.h"

#include <Windows.h>

Simulation::Simulation()
{
	marche = new MarcheAuxPuces();
}


Simulation::~Simulation()
{
}

void Simulation::miseAJour()
{
	int chanceClient = rand() % 12 + 1;
	if (chanceClient == 1)
	{
		DWORD* idThread = new DWORD();
		ClientSim* client = new ClientSim();
		threads.push_back(CreateThread(0, 0, appelClient, client, 0, idThread));
	}
	if (threads.size() > 0)
	WaitForMultipleObjects(threads.size(), &threads[0], true, INFINITE);
}
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