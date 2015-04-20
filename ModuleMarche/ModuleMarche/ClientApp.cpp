#include "ClientApp.h"

#include <fstream> //Utilisé pour opérations dans les fichiers
#include <sstream> //Utilisé pour la transformation int en string (exemple ligne 376)
#include <vector>

#include "Date.h"
#include "Affichage.h"
#include "ExceptionMarche.h"
#include "Client.h"
#include "MarcheAuxPuces.h"
#include "Article.h"
#include "Transaction.h"
#include "Compte.h"
#include "Divers.h"
#include "Voiture.h"
#include "Bijou.h"
#include "Acheteur.h"
#include "Vendeur.h"
#include "Superclient.h"
#include "Employe.h"
#include "Fichier.h"
#include "FabriqueClient.h"

using namespace std; //Pour ne jamais avoir à écrire std:: puisque j'utilise beaucoup de fonctions de std dans ce fichier

ClientApp::ClientApp(void)
{
}

ClientApp::~ClientApp(void)
{
}

Client* ClientApp::getClient() const
{
	return client;
}

string ClientApp::getCompte() const
{
	return compte;
}

MarcheAuxPuces* ClientApp::getMarcheAuxPuces() const
{
	return marcheAuxPuces;
}

void ClientApp::setClient(Client* poClient)
{
	client = poClient;
}

//Déconnexion: On retire le marché et le client connecté de la mémoire
void ClientApp::deconnexion()
{
	delete client;
	client = NULL;
	delete marcheAuxPuces;
	marcheAuxPuces = NULL;
}

Client* ClientApp::inscription(string nom,string prenom,string adresse,float solde,string forfait)
{
	return FabriqueClient::creationClient(nom, prenom, adresse, solde, forfait);
}

// On crée le client et le marché
void ClientApp::connexion(const string& psNomCompte)
{
	compte = psNomCompte;
	creationMarche("Centre-ville");
	creationClient(psNomCompte);
}

Client* ClientApp::changementForfait(char forfait)
{
	return client = FabriqueClient::modificationForfaitClient(client,forfait);
}

//Selon le nom de compte, on va chercher les informations du client (nom, prenom, adresse, solde du compte, achats) dans un fichier
void ClientApp::creationClient(const string &nomCompte)
{
	//On recherche les informations du client sur la première ligne du fichier
	// Todo: Extract Method GetClientFromStructure
	vector<vector<string>> loEntreesClient = Fichier::getContenu(nomCompte);

	string nom = loEntreesClient.at(0)[1];

	string prenom = loEntreesClient.at(0)[2];

	string adresse = loEntreesClient.at(0)[3];

	string soldeStr = loEntreesClient.at(0)[4];
	float solde = stof(soldeStr.c_str());

	string forfaitStr = loEntreesClient.at(0)[5];
	char forfait = forfaitStr[0];

	switch (forfait)
	{
		case 'A':
			client = new Acheteur(nom,prenom,adresse,new Compte(solde));
			break;
		case 'V':
			client = new Vendeur(nom,prenom,adresse,new Compte(solde));
			break;
		case 'S':
			client = new Superclient(nom,prenom,adresse,new Compte(solde));
			break;
		case 'E':
			// Todo: Extract method int GetEmployeExistant, renvoie l'employé ou null
			bool found= false;
			for (size_t cpt = 0;cpt < comptesEmployes.size() && found == false; cpt++)
			{
				if (compte == comptesEmployes[cpt])
				{
					client = marcheAuxPuces->getPersonnel()[cpt];
					found = true;
				}
			}
			if (!found)
			{
				string salaireStr = loEntreesClient.at(0)[6];
				float salaire = stof(salaireStr.c_str());

				string rabaisStr = loEntreesClient.at(0)[7];
				float rabais = stof(rabaisStr.c_str());

				client = new Employe(nom,prenom,adresse,new Compte(solde),salaire,rabais);
			}
				
			break;
	}

	if (client == nullptr)
	{
		throw ExceptionMarche(string("Le client n'a pas pu être ouvert."), false);
	}
	
	//On récupère les achats du client après la première ligne du fichier
	if (forfait != 'E')
	{
		string ligneAchats;
		for (size_t cptLigne = 1; cptLigne <= loEntreesClient.size(); cptLigne++)
		{
			Article* loArticle = getArticleFromStructure(loEntreesClient, cptLigne);
			if(loArticle != nullptr)
				client->ajouterArticle(loArticle);
		}
	}
}

//On crée le marché à partir d'un fichier
void ClientApp::creationMarche(const string &nom)
{
	vector<vector<string>> loEntreesMarche = Fichier::getContenu(nom);
	if (loEntreesMarche.size() < 1)
	{
		// Sans marché aux puces, l'application ne peut plus poursuivre.
		throw ExceptionMarche(string("Une erreur est survenue lors de l'ouverture du marche aux puces."), true);
	}

	// Création du marché à partir de la première ligne du fichier contenant dans l'ordre: nom, adresse, revenu.
	string adresse = loEntreesMarche.at(1)[1];
	string revenuStr = loEntreesMarche.at(1)[2];
	float revenu = stof(revenuStr.c_str());
	marcheAuxPuces = new MarcheAuxPuces(nom, adresse, new Compte(revenu));

	//On crée les articles de ce marché à partir du reste du fichier
	for (size_t cptLigne = 1; cptLigne < loEntreesMarche.size(); cptLigne++)
	{
		//Obtention des valeurs des propriétés d'un article
		Article* loArticle = getArticleFromStructure(loEntreesMarche, cptLigne);
		if (loArticle != nullptr)
			marcheAuxPuces->ajouterArticle(loArticle);
	}

	//Maintenant, on ajoute le personnel à la liste d'employés du MarcheAuxPuces
	// Todo: Extract method: AjouterEmployesDuFichier(Marche)
	vector<vector<string>> loEntreesEmploye = Fichier::getContenu(nom + "_Employes");
	for (size_t cptLigne = 0; cptLigne < loEntreesEmploye.size(); cptLigne++)
	{
		// Obtention des informations d'un employé.
		string lsNomCompteEmploye = loEntreesEmploye.at(cptLigne)[0];
		vector<vector<string>> loEmploye = Fichier::getContenu(lsNomCompteEmploye);
		Employe* unEmploye = getEmployeFromStructure(loEmploye, 0);
		if (unEmploye == nullptr)
			continue;

		// Obtention des achats de l'employé dans les lignes qui suivent.
		for (size_t cpt = 1; cpt < loEmploye.size(); cpt++)
		{
			Article* loArticle = getArticleFromStructure(loEmploye, cpt);
			if (loArticle != nullptr)
				unEmploye->ajouterArticle(loArticle);
		}
		comptesEmployes.push_back(lsNomCompteEmploye);
		marcheAuxPuces->ajouterEmploye(unEmploye);
	}
}

Article* ClientApp::getArticleFromStructure(vector<vector<string>>& poArticleStructure, size_t piLigne)
{
	// On vérifie que la ligne de de l'article est valide.
	if (poArticleStructure.size() - 1 < piLigne)
		return nullptr;

	// On construit l'article à partir de la ligne de la structure.
	string nomArticle = poArticleStructure.at(piLigne)[0];

	string typeStr = poArticleStructure.at(piLigne)[1];
	char type = NULL;
	if (typeStr.length() > 0)
		type = typeStr[0];

	string prixStr = poArticleStructure.at(piLigne)[2];
	float prix = stof(prixStr.c_str());

	string description = poArticleStructure.at(piLigne)[3];

	string etat = poArticleStructure.at(piLigne)[4];

	string dateFabricationStr = poArticleStructure.at(piLigne)[5];
	struct Date dateFabrication = Date::getDateFromString(dateFabricationStr);
	Etat* vraiEtat;
	Article* loArticle = nullptr;
	if (etat == "Neuf")
	{
		vraiEtat = new EtatNeuf();
	}
	else if (etat == "Usage")
	{
		vraiEtat = new EtatUsage();
	}
	else
	{
		vraiEtat = new EtatMateriaux();
	}
	switch (type)
	{
		case 'D':
			loArticle = new Divers(nomArticle, prix, description, vraiEtat, dateFabrication);
			break;
		case 'V':
		{
					string attribut1Str = poArticleStructure.at(piLigne)[6];
			int attribut1 = atoi(attribut1Str.c_str());

			string attribut2 = poArticleStructure.at(piLigne)[7];

			string attribut3Str = poArticleStructure.at(piLigne)[8];
			int attribut3 = atoi(attribut3Str.c_str());

			loArticle = new Voiture(nomArticle, prix, description, vraiEtat, dateFabrication, attribut1, attribut2, attribut3);
		}
			break;
		case 'B':
		{
			string attribut1Str = poArticleStructure.at(piLigne)[6];
			int attribut1 = atoi(attribut1Str.c_str());

			string attribut2 = poArticleStructure.at(piLigne)[7];

			loArticle = new Bijou(nomArticle, prix, description, vraiEtat, dateFabrication, attribut2, attribut1);
		}
			break;
	}

	return loArticle;
}

Employe* ClientApp::getEmployeFromStructure(vector<vector<string>>& poEmployeStructure, size_t piLigne)
{
	// On vérifie que la ligne de de l'employé est valide.
	if (poEmployeStructure.size() - 1 < piLigne)
		return nullptr;

	// On construit l'employé à partir de la ligne de la structure.
	string nomP = poEmployeStructure.at(0)[1];

	string prenomP = poEmployeStructure.at(0)[2];

	string adresseP = poEmployeStructure.at(0)[3];

	string soldeStrP = poEmployeStructure.at(0)[4];
	float soldeP = stof(soldeStrP.c_str());

	string forfaitStr = poEmployeStructure.at(0)[5];
	char forfait = NULL;
	if (forfaitStr.length() > 0)
		forfait = forfaitStr[0];

	string salaireStr = poEmployeStructure.at(0)[6];
	float salaire = stof(salaireStr.c_str());

	string rabaisStr = poEmployeStructure.at(0)[7];
	float rabais = stof(rabaisStr.c_str());

	Employe* loEmploye = new Employe(nomP, prenomP, adresseP, new Compte(soldeP), salaire, rabais);

	return loEmploye;
}

//Client* ClientApp::getClientFromStructure(vector<vector<string>>& poClientStructure, size_t piLigne)
//{
//}