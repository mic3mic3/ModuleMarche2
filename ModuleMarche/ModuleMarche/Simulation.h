#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <Windows.h>

#include "ClientSim.h"
#include "MarcheAuxPuces.h"

using std::vector;

class ClientApp;
class Simulation
{
private:
	vector<ClientSim*> clients;
	int heures;

public:
	static ClientApp clientApp;

	Simulation();
	void miseAJour();
	int getHeures();
	friend DWORD WINAPI appelClient(LPVOID);
	~Simulation();
};

#endif