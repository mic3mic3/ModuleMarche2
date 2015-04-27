#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <Windows.h>

#include "ClientSim.h"
#include "MarcheAuxPuces.h"
#include <random>
#include "Journee.h"

using std::vector;

class Vendeur;
class ClientApp;
class Simulation
{
private:
	vector<ClientSim*> clients;
	int heures;

public:
	static ClientApp clientApp;
	static std::default_random_engine generator;
	const float MONTANT_DEPART;
	vector<Journee> journees;

	Simulation();
	void miseAJour();
	int getHeures();
	void simulerClient(HANDLE,ClientSim*);
	friend DWORD WINAPI appelClient(LPVOID);
	void commanderArticlesManquantsMarche();
	void ajouterArticleManquantVendeur(Vendeur*);
	void ecrireSimulation();
	Article* genererNouvelArticleAleatoire();
	~Simulation();
};

#endif