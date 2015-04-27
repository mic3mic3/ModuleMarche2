#include "Simulation.h"

#include <Windows.h>
#include <winnt.h>
#include "ClientApp.h"
#include "Acheteur.h"
#include "Vendeur.h"
#include "Superclient.h"
#include "Employe.h"
#include "Compte.h"
#include "Article.h"
#include <random>
#include "FabriqueArticle.h"
#include "FabriqueClient.h"
#include "Date.h"
#include "Transaction.h"
#include "GestionSimulation.h"
using namespace std;

ClientApp Simulation::clientApp;
std::default_random_engine Simulation::generator;

// On passe en paramètre le clientApp de la simulation.  Ainsi, on va gérer dans cette classe le contenu et le bon
// fonctionnement, comme par exemple le nombre d'articles restant, la création des employés, etc.
GestionSimulation::GestionSimulation(ClientApp* poClientApp)
{
	clientApp = poClientApp;
}

Simulation::~Simulation()
{
	// Attention, ne pas détruire le clientApp ici, ça se fait dans Simulation
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

	string lsNomArticle = "ArticleSim";

	std::uniform_int_distribution<int> distributionPrix(liPrixMinimum, liPrixMaximum);
	float lfPrix = (float)distributionPrix(Simulation::generator);

	string lsDescription = "DescriptionArticle";

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

void Simulation::ajoutClient()
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
	std::uniform_int_distribution<int> distribution2((int)min, 1000000);
	float solde = (float)distribution2(Simulation::generator);
	clientApp.ajoutClient("NomSim" + to_string(clientApp.getAllClients().size()), "PrenomSim", "AdresseSim", solde, forfait);
}