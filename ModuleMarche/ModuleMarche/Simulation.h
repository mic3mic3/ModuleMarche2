#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <Windows.h>

#include "MarcheAuxPuces.h"
#include <random>
#include "Journee.h"

using std::vector;

class Vendeur;
class ClientApp;
class SimulationGestion;
class Simulation
{
private:
	int heures;
	SimulationGestion simulationGestion;

public:
	static ClientApp clientApp;
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

	// Gestion du faux marché simulé
	Article* genererNouvelArticleAleatoire();
	void ajouterNouvelEmployeAleatoire();
	void ajoutClient();
	void commanderArticlesManquantsMarche();
	void ajouterArticleManquantVendeur(Vendeur*);
};

#endif