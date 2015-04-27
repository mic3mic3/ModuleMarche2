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
#include "FabriqueClient.h"
#include "Date.h"
#include "Fichier.h"
#include "Transaction.h"

using namespace std;

ClientApp Simulation::clientApp;
std::default_random_engine Simulation::generator;

Simulation::Simulation() :MONTANT_DEPART(1000)
{
	clientApp = ClientApp();
	clientApp.setMarcheAuxPuces(new MarcheAuxPuces("", "", new Compte(MONTANT_DEPART)));
	generator = default_random_engine(time(NULL));
	//srand(time(NULL));

	// Génération d'employés aléatoires
	for (size_t cptEmploye = 1; cptEmploye <= 5; cptEmploye++)
	{
		ajouterNouvelEmployeAleatoire();
	}
}

Simulation::~Simulation()
{
	for (size_t cptClient = 0; cptClient < clients.size(); cptClient++)
	{
		delete clients[cptClient];
	}
}
struct ThreadParameters
{
	Simulation* sim;
	ClientSim* client;
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
	journees[heures / 24].solde = clientApp.getMarcheAuxPuces()->getRevenu();
	//cout << endl << "----------" << endl;
	delete[] t;
	heures++;
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
		else if (dynamic_cast<Acheteur*>(Simulation::clientApp.getClient(client->getNum())))
		{
			transactionAFaire = "A";
		}

		//do something
		WaitForSingleObject(mutex, INFINITE);

		//cout << transactionAFaire;

		// On commande des articles au marché aux puces s'il en manque.
		while (!clientApp.getMarcheAuxPuces()->quantiteArticlesSuffisante())
		{
			commanderArticlesManquantsMarche();
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
				journees[heures / 24].totalVentes += clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getPrixEtat();
				clientApp.venteArticleAuClient(cpt, client->getNum());
				historiqueTransactions.push_back(clientApp.getMarcheAuxPuces()->getDerniereTransaction());
				journees[heures / 24].nbrArticlesVendus++;
			}
			//client->achat();
		}
		else if (transactionAFaire == "V")
		{
			ajouterArticleManquantVendeur(vnd);

			size_t cpt;
			for (cpt = 0; cpt < vnd->getArticles().size(); cpt++)
			{
				if (clientApp.getMarcheAuxPuces()->validerCompte(vnd->getArticles()[cpt]->getPrixEtat()))
				{
					break;
				}
			}//client->vente();
			if (cpt != vnd->getArticles().size())
			{
				journees[heures / 24].totalAchats += vnd->getArticles()[cpt]->getPrixEtat();
				clientApp.venteArticleDuClient(cpt, client->getNum());
				historiqueTransactions.push_back(vnd->getDerniereTransaction());
				journees[heures / 24].nbrArticlesAchetes++;
			}
		}
		

	}
	//WaitForSingleObject(mutex,INFINITE);
	
	//cout << endl << "Mise a jour: " << client->getNum();

	//do something with mutex
	ReleaseMutex(mutex);
}

void Simulation::commanderArticlesManquantsMarche()
{
	if (clientApp.getMarcheAuxPuces()->quantiteArticlesSuffisante())
		return;

	// On doit commander davantage d'articles.
	size_t liNombreArticlesAjout = 5;
	for (size_t cptAjout = 1; cptAjout < liNombreArticlesAjout; cptAjout++)
	{
		clientApp.getMarcheAuxPuces()->ajouterArticle(genererNouvelArticleAleatoire());
	}
}

void Simulation::ajouterArticleManquantVendeur(Vendeur* poVendeur)
{
	if (poVendeur->getArticles().size() > 0)
		return;

	// On doit commander davantage d'articles.
	size_t liNombreArticlesAjout = 5;
	for (size_t cptAjout = 1; cptAjout < liNombreArticlesAjout; cptAjout++)
	{
		poVendeur->ajouterArticle(genererNouvelArticleAleatoire());
	}
}

void Simulation::ecrireSimulation()
{
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
	journeesAEcrire += "Montant de départ: " + to_string(MONTANT_DEPART) + "\n";
	journeesAEcrire += "Durée: " + to_string(journees.size()) + " jours et " + to_string(heures % 24) + " heures\n";
	journeesAEcrire += "Total des articles achetés: " + to_string(totalArticlesAchetes) + "\n";
	journeesAEcrire += "Total des articles vendus: " + to_string(totalArticlesVendus) + "\n";
	journeesAEcrire += "Grand total des achats: " + to_string(grandTotalAchats) + "\n";
	journeesAEcrire += "Grand total des ventes: " + to_string(grandTotalVentes) + "\n";
	journeesAEcrire += "Total des gains: " + to_string(totalGains) + "\n";
	journeesAEcrire += "---------\n";
	for (size_t cpt = 0; cpt < journees.size(); cpt++)
	{
		journeesAEcrire += "Journée " + to_string(cpt + 1) + "\n";
		journeesAEcrire += "Nombre d'articles achetés: " + to_string(journees[cpt].nbrArticlesAchetes) + "\n";
		journeesAEcrire += "Nombre d'articles vendus: " + to_string(journees[cpt].nbrArticlesVendus) + "\n";
		journeesAEcrire += "Total des achats: " + to_string(journees[cpt].totalAchats) + "$\n";
		journeesAEcrire += "Total des ventes: " + to_string(journees[cpt].totalVentes) + "$\n";
		journeesAEcrire += "Total des gains de la journée: " + to_string(journees[cpt].totalVentes-journees[cpt].totalAchats) + "$\n";
		journeesAEcrire += "Solde actuel: " + to_string(journees[cpt].solde) + "\n";
		journeesAEcrire += "---------\n";
	}
	Fichier::setContenuRaw(string("Simulation-Journées.txt"), journeesAEcrire);

	string historique = "Historique des transactions\n-----\n";
	for (size_t cpt = 0; cpt < historiqueTransactions.size(); cpt++)
	{
		historique += "Type de transaction: " + to_string(historiqueTransactions[cpt]->type) + "\n";
		historique += "Client: " + historiqueTransactions[cpt]->client->getNom() + ";"
			+ historiqueTransactions[cpt]->client->getPrenom() + ";"
			+ historiqueTransactions[cpt]->client->getAdresse() + "\n";
		historique += "Marche: " + historiqueTransactions[cpt]->marche->getNom() + ";"
			+ historiqueTransactions[cpt]->marche->getAdresse() + "\n";
		historique += "Article: " + historiqueTransactions[cpt]->article->getNom() + ";"
			+ to_string(historiqueTransactions[cpt]->article->getPrix()) + ";"
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

Article* Simulation::genererNouvelArticleAleatoire()
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

	return FabriqueArticle::creationArticle(lcType, lsNomArticle, lfPrix, lsDescription, etat, loDateFabrication, liAttribut1, lsAttribut2, liAttribut3);
}

void Simulation::ajouterNouvelEmployeAleatoire()
{
	string nom = "Employe abc";
	string prenom = "prenom abc";
	string adresse = "111 rue adresse";
	float solde = 5000;
	string forfait = "E";

	clientApp.ajoutClient(nom, prenom, adresse, solde, forfait);
}

int main()
{
	Simulation simu;

	// Simulation d'une semaine
	for (size_t cptJour = 1; cptJour <= 7; cptJour++)
	{
		for (int cpt = 1; cpt <= 24; cpt++)
		{
			simu.miseAJour();
		}
	}

	simu.ecrireSimulation();

	cout << "Fermeture de l'application..." << endl;
	return EXIT_SUCCESS;
}