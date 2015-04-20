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
	int chanceClient = rand() % 25 + 1;
	if (chanceClient == 1)
	{
		DWORD* idThread = new DWORD();
		threads.push_back(CreateThread(0, 0, appelClient, new ClientSim(), 0, idThread);
	}
}

DWORD WINAPI Simulation::appelClient(LPVOID client)
{
	((ClientSim*)client)->miseAJour(mutex);
}

int Simulation::getHeures()
{
	return heures;
}