#include "ClientSim.h"

#include <iostream>
#include "Simulation.h"
#include "ClientApp.h"
#include "Acheteur.h"
#include "Vendeur.h"
#include "Superclient.h"
#include "Employe.h"
#include <ctime>

using namespace std;
ClientSim::ClientSim()
{
	std::uniform_int_distribution<int> distribution(1, 3);
	int forfaitRand = distribution(Simulation::generator);
	string forfait;
	float min;
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
	std::uniform_int_distribution<int> distribution2(min, 1000000);
	int solde = distribution2(Simulation::generator);
	this->num = Simulation::clientApp.ajoutClient("","","",solde,forfait);
	//client = new Acheteur();
}



ClientSim::~ClientSim()
{

}

int ClientSim::getNum()
{
	return num;
}

void ClientSim::achat()
{

}

void ClientSim::vente()
{

}