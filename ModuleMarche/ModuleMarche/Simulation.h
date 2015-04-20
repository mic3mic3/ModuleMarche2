#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <Windows.h>

#include "ClientSim.h"
#include "MarcheAuxPuces.h"

using std::vector;

class Simulation
{
private:
	vector<ClientSim*> clients;
	MarcheAuxPuces* marche;
	int heures;
	vector<HANDLE> threads;
	HANDLE mutex;

public:
	Simulation();
	void miseAJour();
	int getHeures();
	friend DWORD WINAPI appelClient(LPVOID);
	~Simulation();
};

#endif