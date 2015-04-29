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
#include <ctime>
#include "Date.h"
#include "Fichier.h"
#include "Transaction.h"
#include "GestionSimulation.h"

using namespace std;

std::default_random_engine Simulation::generator;

Simulation::Simulation() :MONTANT_DEPART(1000)
{
	clientApp = new ClientApp();
	gestionSimulation = GestionSimulation(clientApp);
	clientApp->setMarcheAuxPuces(new MarcheAuxPuces("MarchéSimulation", "AdresseMarché", new Compte(MONTANT_DEPART)));
	generator = default_random_engine((unsigned int)time(NULL));

	// Génération d'employés aléatoires
	for (size_t cptEmploye = 1; cptEmploye <= 5; cptEmploye++)
	{
		gestionSimulation.ajouterNouvelEmployeAleatoire();
	}
}

Simulation::~Simulation()
{
	delete clientApp;
}
struct ThreadParameters //Pour passer plusieurs paramètres dans les Thread
{
	Simulation* sim;
	int noClient;
};
void Simulation::miseAJour()
{
	if (heures % 24 == 0)
	{
		Journee jour = Journee();
		jour.nbrArticlesAchetes = 0;
		jour.nbrArticlesVendus = 0;
		jour.totalAchats = 0;
		jour.totalVentes = 0;
		journees.push_back(jour);
	}
	std::uniform_int_distribution<int> distribution(1, 12);
	int chanceClient = distribution(generator);
	if (chanceClient == 1)
	{
		gestionSimulation.ajoutClient();
	}
	HANDLE* t = new HANDLE[clientApp->getAllClients().size()];

	for (unsigned int cpt = 0; cpt < clientApp->getAllClients().size(); cpt++)
	{
		DWORD* idThread = new DWORD();
		//clients.push_back(new ClientSim());
		ThreadParameters* param = new ThreadParameters();
		param->sim = this;
		param->noClient = cpt;
		t[cpt] = CreateThread(0, 0, appelClient, param, 0, idThread);
	}
	WaitForMultipleObjects(clientApp->getAllClients().size(), t, true, INFINITE);
	journees[heures / 24].solde = clientApp->getMarcheAuxPuces()->getRevenu();
	delete[] t;
	heures++;
}
HANDLE mutex=CreateMutex(NULL,false,NULL);
DWORD WINAPI appelClient(LPVOID params)
{
	ThreadParameters* param = (ThreadParameters*)params;
	param->sim->simulerClient(mutex,param->noClient);
	return 0;
}

int Simulation::getHeures()
{
	return heures;
}

void Simulation::simulerClient(HANDLE mutex,int client)
{
	std::uniform_int_distribution<int> distribution(1, 24);
	int chanceClient = distribution(Simulation::generator);
	if (chanceClient == 1)
	{
		string transactionAFaire = "";
		Vendeur* vnd = nullptr;
		if (vnd = dynamic_cast<Superclient*>(Simulation::clientApp->getClient(client)))
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
		else if (vnd = dynamic_cast<Vendeur*>(Simulation::clientApp->getClient(client)))
		{
			transactionAFaire = "V";
		}
		else if (dynamic_cast<Acheteur*>(Simulation::clientApp->getClient(client)))
		{
			transactionAFaire = "A";
		}

		WaitForSingleObject(mutex, INFINITE);


		// On commande des articles au marché aux puces s'il en manque.
		while (!clientApp->getMarcheAuxPuces()->quantiteArticlesSuffisante())
		{
			gestionSimulation.commanderArticlesManquantsMarche();
		}
		if (transactionAFaire == "A")
		{
			size_t cpt;
			for (cpt = 0; cpt < clientApp->getMarcheAuxPuces()->getArticlesEnVente().size(); cpt++)
			{
				if (clientApp->getClient(client)->validerCompte(clientApp->getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getPrixEtat()))
				{
					break;
				}
			}
			if (cpt != clientApp->getMarcheAuxPuces()->getArticlesEnVente().size())
			{
				journees[heures / 24].totalVentes += clientApp->getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getPrixEtat();
				clientApp->venteArticleAuClient(cpt, client);
				historiqueTransactions.push_back(clientApp->getMarcheAuxPuces()->getDerniereTransaction());
				journees[heures / 24].nbrArticlesVendus++;
			}
			//client->achat();
		}
		else if (transactionAFaire == "V")
		{
			gestionSimulation.ajouterArticleManquantVendeur(vnd);

			size_t cpt;
			for (cpt = 0; cpt < vnd->getArticles().size(); cpt++)
			{
				if (clientApp->getMarcheAuxPuces()->validerCompte(vnd->getArticles()[cpt]->getPrixEtat()))
				{
					break;
				}
			}//client->vente();
			if (cpt != vnd->getArticles().size())
			{
				journees[heures / 24].totalAchats += vnd->getArticles()[cpt]->getPrixEtat();
				clientApp->venteArticleDuClient(cpt, client);
				historiqueTransactions.push_back(vnd->getDerniereTransaction());
				journees[heures / 24].nbrArticlesAchetes++;
			}
		}
	}
	ReleaseMutex(mutex);
}

//On écrit les données dans des fichiers
void Simulation::ecrireSimulation()
{
	//On écrit premièrement le fichier des journées
	string journeesAEcrire = "";
	int totalArticlesAchetes = 0;
	int totalArticlesVendus = 0;
	float grandTotalAchats = 0;
	float grandTotalVentes = 0;
	float totalGains;
	for (size_t cpt = 0; cpt < journees.size(); cpt++)
	{
		totalArticlesAchetes += journees[cpt].nbrArticlesAchetes;
		totalArticlesVendus += journees[cpt].nbrArticlesVendus;
		grandTotalAchats += journees[cpt].totalAchats;
		grandTotalVentes += journees[cpt].totalVentes;
	}
	totalGains = grandTotalVentes-grandTotalAchats;
	journeesAEcrire += "Données globales\n";
	journeesAEcrire += "Montant de départ: " + ClientApp::getMontantFormate(MONTANT_DEPART) + "\n";
	journeesAEcrire += "Durée: " + to_string(journees.size()) + " jours et " + to_string(heures % 24) + " heures\n";
	journeesAEcrire += "Total des articles achetés par le marché: " + to_string(totalArticlesAchetes) + "\n";
	journeesAEcrire += "Total des articles vendus par le marché: " + to_string(totalArticlesVendus) + "\n";
	journeesAEcrire += "Grand total des achats: " + ClientApp::getMontantFormate(grandTotalAchats) + "\n";
	journeesAEcrire += "Grand total des ventes: " + ClientApp::getMontantFormate(grandTotalVentes) + "\n";
	journeesAEcrire += "Total des gains: " + ClientApp::getMontantFormate(totalGains) + "\n";
	journeesAEcrire += "---------\n";
	for (size_t cpt = 0; cpt < journees.size(); cpt++)
	{
		journeesAEcrire += "Journée " + to_string(cpt + 1) + "\n";
		journeesAEcrire += "Nombre d'articles achetés par le marché: " + to_string(journees[cpt].nbrArticlesAchetes) + "\n";
		journeesAEcrire += "Nombre d'articles vendus par le marché: " + to_string(journees[cpt].nbrArticlesVendus) + "\n";
		journeesAEcrire += "Total des achats du marché: " + ClientApp::getMontantFormate(journees[cpt].totalAchats) + "\n";
		journeesAEcrire += "Total des ventes du marché: " + ClientApp::getMontantFormate(journees[cpt].totalVentes) + "\n";
		journeesAEcrire += "Total des gains de la journée: " + ClientApp::getMontantFormate(journees[cpt].totalVentes - journees[cpt].totalAchats) + "\n";
		journeesAEcrire += "Solde actuel: " + ClientApp::getMontantFormate(journees[cpt].solde) + "\n";
		journeesAEcrire += "---------\n";
	}
	Fichier::setContenuRaw(string("Simulation-Journées.txt"), journeesAEcrire);

	//On écrit le fichier des historiques
	string historique = "Historique des transactions\n-----\n";
	for (size_t cpt = 0; cpt < historiqueTransactions.size(); cpt++)
	{
		historique += "Type de transaction: " + string(1,historiqueTransactions[cpt]->type) + "\n";
		historique += "Client: " + historiqueTransactions[cpt]->client->getNom() + ";"
			+ historiqueTransactions[cpt]->client->getPrenom() + ";"
			+ historiqueTransactions[cpt]->client->getAdresse() + "\n";
		historique += "Marche: " + historiqueTransactions[cpt]->marche->getNom() + ";"
			+ historiqueTransactions[cpt]->marche->getAdresse() + "\n";
		historique += "Article: " + historiqueTransactions[cpt]->article->getNom() + ";"
			+ ClientApp::getMontantFormate(historiqueTransactions[cpt]->article->getPrixEtat()) + ";"
			+ historiqueTransactions[cpt]->article->getDescription() + ";"
			+ historiqueTransactions[cpt]->article->getEtat()->getDescription() + ";"
			+ to_string(historiqueTransactions[cpt]->article->getDate().jour) + "/"
			+ to_string(historiqueTransactions[cpt]->article->getDate().mois) + "/"
			+ to_string(historiqueTransactions[cpt]->article->getDate().annee) + "\n";
		historique += "Date: " + to_string(historiqueTransactions[cpt]->date.jour) + "/"
			+ to_string(historiqueTransactions[cpt]->date.mois) + "/"
			+ to_string(historiqueTransactions[cpt]->date.annee) + "\n";
		historique += "-----\n";
	}
	Fichier::setContenuRaw(string("Simulation-Historique_Transactions.txt"), historique);
}

//int main()
//{
//	Simulation simu;
//
//	// Simulation d'une semaine
//	for (size_t cptJour = 1; cptJour <= 7; cptJour++)
//	{
//		for (int cpt = 1; cpt <= 24; cpt++)
//		{
//			simu.miseAJour();
//		}
//	}
//
//	simu.ecrireSimulation();
//
//	cout << "Fermeture de l'application..." << endl;
//	return EXIT_SUCCESS;
//}