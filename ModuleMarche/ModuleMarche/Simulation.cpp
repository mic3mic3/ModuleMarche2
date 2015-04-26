#include "Simulation.h"

#include <Windows.h>
#include <winnt.h>
#include <iostream>
#include "ClientApp.h"
#include "Acheteur.h"
#include "Vendeur.h"
#include "Superclient.h"
#include "Employe.h"
#include <random>
#include <ctime>
#include "FabriqueArticle.h"
#include "Date.h"
using namespace std;

ClientApp Simulation::clientApp;
std::default_random_engine Simulation::generator;

Simulation::Simulation()
{
	clientApp = ClientApp();
	generator = default_random_engine(time(NULL));
	//srand(time(NULL));
	
}

Simulation::~Simulation()
{
}
struct ThreadParameters
{
	Simulation* sim;
	ClientSim* client;
};
void Simulation::miseAJour()
{
	
	std::uniform_int_distribution<int> distribution(1, 12);
	int chanceClient = distribution(generator);
	if (chanceClient == 1)
	{
		//DWORD* idThread = new DWORD();
		clients.push_back(new ClientSim());
		//threads.push_back(CreateThread(0, 0, appelClient, client, 0, idThread));
	}
	HANDLE* t = new HANDLE[clients.size()];
	/*for (unsigned int cpt = 0; cpt < threads.size(); cpt++)
	{
		WaitForSingleObject(threads[cpt], INFINITE);
	}*/
	for (unsigned int cpt = 0; cpt < clients.size(); cpt++)
	{
		DWORD* idThread = new DWORD();
		//clients.push_back(new ClientSim());
		ThreadParameters* param = new ThreadParameters();
		param->sim = this;
		param->client = clients[cpt];
		t[cpt] = CreateThread(0, 0, appelClient, param, 0, idThread);
		//threads.push_back(CreateThread(0, 0, appelClient, client, 0, idThread));
	}
	WaitForMultipleObjects(clients.size(), t,true,INFINITE);
	cout << endl << "----------" << endl;
	delete[] t;
}
HANDLE mutex=CreateMutex(NULL,false,NULL);
DWORD WINAPI appelClient(LPVOID params)
{
	ThreadParameters* param = (ThreadParameters*)params;
	param->sim->simulerClient(mutex,param->client);
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

void Simulation::simulerClient(HANDLE mutex,ClientSim* client)
{
	std::uniform_int_distribution<int> distribution(1, 24);
	int chanceClient = distribution(Simulation::generator);
	if (chanceClient == 1)
	{
		string transactionAFaire = "";
		if (dynamic_cast<Superclient*>(Simulation::clientApp.getClient(client->getNum())))
		{
			std::uniform_int_distribution<int> distribution2(1, 2);
			int transac = distribution2(Simulation::generator);
			if (transac == 1)
			{
				transactionAFaire = "A";
			}
			else
			{
				transactionAFaire = "V";
			}

		}
		else if (dynamic_cast<Vendeur*>(Simulation::clientApp.getClient(client->getNum())))
		{
			transactionAFaire = "V";
		}
		else if ((dynamic_cast<Acheteur*>(Simulation::clientApp.getClient(client->getNum()))))
		{
			transactionAFaire = "A";
		}
		cout << transactionAFaire;

		//do something
		WaitForSingleObject(mutex, INFINITE);
		if (transactionAFaire == "A")
		{
			client->achat();
		}
		else if (transactionAFaire == "V")
		{
			client->vente();
		}
	}
	cout << endl << "Mise a jour: " << client->getNum();

	//do something with mutex
	ReleaseMutex(mutex);
}

void Simulation::commanderArticlesManquants()
{
	if (!clientApp.getMarcheAuxPuces()->quantiteArticlesSuffisante())
		return;

	// On doit commander davantage d'articles.
	size_t liNombreArticlesAjout = 5;
	for (size_t cptAjout = 1; cptAjout < liNombreArticlesAjout; cptAjout++)
	{
		char type;
		string nomArticle;
		float prix;
		string description;
		string etat;
		struct Date dateFabrication;
		int attribut1 = 0;
		string attribut2 = "";
		int attribut3 = 0;



		clientApp.getMarcheAuxPuces()->ajouterArticle(FabriqueArticle::creationArticle(type, nomArticle, prix, description, etat, dateFabrication, attribut1, attribut2, attribut3));
	}
}