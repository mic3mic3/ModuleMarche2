#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <Windows.h>

#include "MarcheAuxPuces.h"
#include <random>
#include "Journee.h"
#include "GestionSimulation.h"

using std::vector;

class Vendeur;
class ClientApp;
//class GestionSimulation;
class Simulation
{
private:
	int heures;
	GestionSimulation gestionSimulation;
	ClientApp* clientApp;

public:
	static std::default_random_engine generator;
	const float MONTANT_DEPART;
	vector<Journee> journees;
	vector<Transaction*> historiqueTransactions;

	Simulation();
	~Simulation();

	// Mise à jour de la simulation
	void miseAJour();
	int getHeures();
	void ecrireSimulation();

	// Threads des clients
	void simulerClient(HANDLE,int);
	friend DWORD WINAPI appelClient(LPVOID);
};

#endif