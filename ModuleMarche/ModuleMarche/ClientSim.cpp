#include "ClientSim.h"

#include <iostream>
#include "Simulation.h"
#include "ClientApp.h"
#include "Acheteur.h"
#include "Vendeur.h"
#include "Superclient.h"

using namespace std;
ClientSim::ClientSim()
{
	int forfaitRand = rand() % 3 + 1;
	string forfait;
	int min;
	switch (forfaitRand)
	{
		case 1:
			forfait = 'A';
			min = Acheteur::FORFAIT;
			break;
		case 2:
			forfait = 'V';
			min = Vendeur::FORFAIT;
			break;
		case 3:
			forfait = 'S';
			min = Superclient::FORFAIT;
			break;
		default:
			break;
	}
	float solde = rand() % 1000000 + min;
	this->num = Simulation::clientApp.ajoutClient("","","",solde,forfait);
	//client = new Acheteur();
}

void ClientSim::miseAJour(HANDLE mutex)
{
	int chanceClient = rand() % 24 + 1;
	if (chanceClient == 1)
	{

	}
	//do something
	WaitForSingleObject(mutex, INFINITE);
	cout << endl << "Mise a jour: " << num;
	
	//do something with mutex
	ReleaseMutex(mutex);
}

ClientSim::~ClientSim()
{
}

ClientSim::parametresClient()