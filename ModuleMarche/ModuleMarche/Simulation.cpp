#include "Simulation.h"

#include <Windows.h>
#include <winnt.h>
#include <iostream>
#include "ClientApp.h"
#include "Acheteur.h"
#include "Vendeur.h"
#include "Superclient.h"
#include "Employe.h"
#include "Compte.h"
#include "Article.h"
#include <random>
#include <ctime>
#include "FabriqueArticle.h"
#include "Date.h"
using namespace std;

ClientApp Simulation::clientApp;
std::default_random_engine Simulation::generator;

Simulation::Simulation() :MONTANT_DEPART(1000)
{
	clientApp = ClientApp();
	clientApp.setMarcheAuxPuces(new MarcheAuxPuces("", "", new Compte(MONTANT_DEPART)));
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
		Vendeur* vnd = nullptr;
		if (vnd = dynamic_cast<Superclient*>(Simulation::clientApp.getClient(client->getNum())))
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
		else if (vnd = dynamic_cast<Vendeur*>(Simulation::clientApp.getClient(client->getNum())))
		{
			transactionAFaire = "V";
		}
		else if ((dynamic_cast<Acheteur*>(Simulation::clientApp.getClient(client->getNum()))))
		{
			transactionAFaire = "A";
		}
		

		//do something
		WaitForSingleObject(mutex, INFINITE);

		cout << transactionAFaire;

		// On commande des articles au marché aux puces s'il en manque.
		while (!clientApp.getMarcheAuxPuces()->quantiteArticlesSuffisante())
		{
			commanderArticlesManquants();
		}
		if (transactionAFaire == "A")
		{
			size_t cpt;
			for (cpt = 0; cpt < clientApp.getMarcheAuxPuces()->getArticlesEnVente().size(); cpt++)
			{
				if (clientApp.getClient(client->getNum())->validerCompte(clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getPrixEtat()))
				{
					break;
				}
			}
			if (cpt != clientApp.getMarcheAuxPuces()->getArticlesEnVente().size())
			{
				clientApp.venteArticleAuClient(cpt, client->getNum());
			}
			//client->achat();
		}
		else if (transactionAFaire == "V")
		{
			size_t cpt;
			for (cpt = 0; cpt < vnd->getArticles().size(); cpt++)
			{
				if (clientApp.getMarcheAuxPuces()->validerCompte(vnd->getArticles()[cpt]->getPrixEtat()))
				{
					break;
				}
			}//client->vente();
			if (cpt != clientApp.getMarcheAuxPuces()->getArticlesEnVente().size())
			{
				clientApp.venteArticleAuClient(cpt, client->getNum());
			}
		}
	}
	WaitForSingleObject(mutex,INFINITE);
	cout << endl << "Mise a jour: " << client->getNum();

	//do something with mutex
	ReleaseMutex(mutex);
}

void Simulation::commanderArticlesManquants()
{
	if (clientApp.getMarcheAuxPuces()->quantiteArticlesSuffisante())
		return;

	// On doit commander davantage d'articles.
	size_t liNombreArticlesAjout = 5;
	for (size_t cptAjout = 1; cptAjout < liNombreArticlesAjout; cptAjout++)
	{
		int liPrixMinimum = 0;
		int liPrixMaximum = 0;

		char lcType = '-';
		std::uniform_int_distribution<int> distributionType(1, 3);
		int liRandomType = distributionType(Simulation::generator);
		if (liRandomType == 1)
		{
			lcType = 'D';
			liPrixMinimum = 1;
			liPrixMaximum = 15000;
		}
		else if (liRandomType == 2)
		{
			lcType = 'B';
			liPrixMinimum = 10;
			liPrixMaximum = 3000;
		}
		else if (liRandomType == 3)
		{
			lcType = 'V';
			liPrixMinimum = 15000;
			liPrixMaximum = 100000;
		}

		string lsNomArticle = "Article abc";

		std::uniform_int_distribution<int> distributionPrix(liPrixMinimum, liPrixMaximum);
		float lfPrix = (float)distributionPrix(Simulation::generator);

		string lsDescription = "Lorem ipsum";

		string etat = "-";
		std::uniform_int_distribution<int> distributionEtat(1, 3);
		int liRandomEtat = distributionEtat(Simulation::generator);
		if (liRandomEtat == 1)
		{
			etat = "Neuf";
		}
		else if (liRandomEtat == 2)
		{
			etat = "Usage";
		}
		else if (liRandomEtat == 3)
		{
			etat = "Materiaux";
		}

		struct Date loDateFabrication;
		loDateFabrication.annee = 1900;
		loDateFabrication.mois = 1;
		loDateFabrication.jour = 1;

		int liAttribut1 = 0;
		string lsAttribut2 = "";
		int liAttribut3 = 0;

		clientApp.getMarcheAuxPuces()->ajouterArticle(FabriqueArticle::creationArticle(lcType, lsNomArticle, lfPrix, lsDescription, etat, loDateFabrication, liAttribut1, lsAttribut2, liAttribut3));
	}
}