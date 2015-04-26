#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <Windows.h>

#include "ClientSim.h"
#include "MarcheAuxPuces.h"
#include <random>

using std::vector;

class ClientApp;
class Simulation
{
private:
	vector<ClientSim*> clients;
	int heures;

public:
	static ClientApp clientApp;
	static std::default_random_engine generator;

	Simulation();
	void miseAJour();
	int getHeures();
	void simulerClient(HANDLE,ClientSim*);
	friend DWORD WINAPI appelClient(LPVOID);
	~Simulation();
};

#endif