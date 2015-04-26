#include "ClientApp.h"

#include <fstream> //Utilis� pour op�rations dans les fichiers
#include <sstream> //Utilis� pour la transformation int en string (exemple ligne 376)
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

using namespace std; //Pour ne jamais avoir � �crire std:: puisque j'utilise beaucoup de fonctions de std dans ce fichier

const string ClientApp::CS_NOM_MARCHE_AUX_PUCES = "Centre-ville";

ClientApp::ClientApp(void)
{
}

ClientApp::~ClientApp(void)
{
}

Client* ClientApp::getClient(int index) const
{
	return clients[index];
}

string ClientApp::getCompte() const
{
	return compte;
}

MarcheAuxPuces* ClientApp::getMarcheAuxPuces() const
{
	return marcheAuxPuces;
}

void ClientApp::setClient(Client* poClient,int index)
{
	clients[index] = poClient;
}

int ClientApp::ajoutClient()
{
	return clients.size() - 1;
}

//D�connexion: On retire le march� et le client connect� de la m�moire
void ClientApp::deconnexion()
{
	while (!clients.empty())
	{
		delete clients.back();
		clients.back() = NULL;
		clients.pop_back();
	}
	delete marcheAuxPuces;
	marcheAuxPuces = NULL;
}

Client* ClientApp::inscription(string nom,string prenom,string adresse,float solde,string forfait)
{
	return FabriqueClient::creationClient(nom, prenom, adresse, solde, forfait);
}

// On cr�e le client et le march�
void ClientApp::connexion(const Fichier& poFichierClient, const Fichier& poFichierMarche, const vector<Fichier>& poFichiersEmploye)
{
	compte = poFichierClient.getEntree(0)[0];
	creationMarche(poFichierMarche, poFichiersEmploye);
	creationClient(poFichierClient);
}

Client* ClientApp::changementForfait(char forfait,int index)
{
	Client* tempCli = FabriqueClient::modificationForfaitClient(clients[index], forfait);
	if (tempCli != nullptr)
	{
		clients[index] = tempCli;
	}
	return tempCli;
}

//Selon le nom de compte, on va chercher les informations du client (nom, prenom, adresse, solde du compte, achats) dans un fichier
void ClientApp::creationClient(const Fichier& poEntreesClient)
{
	//On recherche les informations du client sur la premi�re ligne du fichier
	// Todo: Extract Method GetClientFromStructure
	string nom = poEntreesClient.getEntree(0)[1];

	string prenom = poEntreesClient.getEntree(0)[2];

	string adresse = poEntreesClient.getEntree(0)[3];

	string soldeStr = poEntreesClient.getEntree(0)[4];
	float solde = stof(soldeStr.c_str());

	string forfaitStr = poEntreesClient.getEntree(0)[5];
	char forfait = forfaitStr[0];

	Client* loClient = nullptr;
	switch (forfait)
	{
		case 'A':
			loClient = new Acheteur(nom, prenom, adresse, new Compte(solde));
			break;
		case 'V':
			loClient = new Vendeur(nom, prenom, adresse, new Compte(solde));
			break;
		case 'S':
			loClient = new Superclient(nom, prenom, adresse, new Compte(solde));
			break;
		case 'E':
			loClient = getEmployeExistant(compte);
			if (!loClient)
			{
				// L'employ� n'existe pas encore, on le cr�e.
				string salaireStr = poEntreesClient.getEntree(0)[6];
				float salaire = stof(salaireStr.c_str());

				string rabaisStr = poEntreesClient.getEntree(0)[7];
				float rabais = stof(rabaisStr.c_str());

				loClient = new Employe(nom, prenom, adresse, new Compte(solde), salaire, rabais);
			}				
			break;
		default:
			break;
	}
	if (!loClient)
	{
		throw ExceptionMarche(string("Le client n'a pas pu �tre ouvert."), false);
	}
	
	//On r�cup�re les achats du client apr�s la premi�re ligne du fichier
	if (forfait != 'E')
	{
		string ligneAchats;
		for (size_t cptLigne = 1; cptLigne <= poEntreesClient.nombreEntrees(); cptLigne++)
		{
			Article* loArticle = getArticleFromStructure(poEntreesClient, cptLigne);
			if(loArticle != nullptr)
				loClient->ajouterArticle(loArticle);
		}
	}

	clients.push_back(loClient);
}

//On cr�e le march� � partir d'un fichier
void ClientApp::creationMarche(const Fichier& poFichierMarche, const vector<Fichier>& poFichiersEmploye)
{
	if (poFichierMarche.nombreEntrees() < 1)
	{
		// Sans march� aux puces, l'application ne peut plus poursuivre.
		throw ExceptionMarche(string("Une erreur est survenue lors de l'ouverture du marche aux puces."), true);
	}

	// Cr�ation du march� � partir de la premi�re ligne du fichier contenant dans l'ordre: nom, adresse, revenu.
	string adresse = poFichierMarche.getEntree(1)[1];
	string revenuStr = poFichierMarche.getEntree(1)[2];
	float revenu = stof(revenuStr.c_str());
	marcheAuxPuces = new MarcheAuxPuces(CS_NOM_MARCHE_AUX_PUCES, adresse, new Compte(revenu));

	//On cr�e les articles de ce march� � partir du reste du fichier
	for (size_t cptLigne = 1; cptLigne < poFichierMarche.nombreEntrees(); cptLigne++)
	{
		//Obtention des valeurs des propri�t�s d'un article
		Article* loArticle = getArticleFromStructure(poFichierMarche, cptLigne);
		if (loArticle != nullptr)
			marcheAuxPuces->ajouterArticle(loArticle);
	}

	//Maintenant, on ajoute le personnel � la liste d'employ�s du MarcheAuxPuces
	// Todo: Extract method: AjouterEmployesDuFichier(Marche)
	for (size_t cptFichierEmploye = 0; cptFichierEmploye < poFichiersEmploye.size(); cptFichierEmploye++)
	{
		Employe* loEmploye = getEmployeFromStructure(poFichiersEmploye[cptFichierEmploye], 0);
		if (loEmploye == nullptr)
			continue;

		// Obtention des achats de l'employ� dans les lignes qui suivent.
		for (size_t cptEntree = 1; cptEntree < poFichiersEmploye[cptFichierEmploye].nombreEntrees(); cptEntree++)
		{
			Article* loArticle = getArticleFromStructure(poFichiersEmploye[cptFichierEmploye], cptEntree);
			if (loArticle != nullptr)
				loEmploye->ajouterArticle(loArticle);
		}

		// Ajout de l'employ� au personnel du march� aux puces
		string lsNomCompteEmploye = poFichiersEmploye[cptFichierEmploye].getEntree(0)[0];
		comptesEmployes.push_back(lsNomCompteEmploye);
		marcheAuxPuces->ajouterEmploye(loEmploye);
	}
}

Article* ClientApp::getArticleFromStructure(const Fichier& poArticleStructure, size_t piLigne)
{
	// On v�rifie que la ligne de de l'article est valide.
	if (poArticleStructure.nombreEntrees() - 1 < piLigne)
		return nullptr;

	// On construit l'article � partir de la ligne de la structure.
	string nomArticle = poArticleStructure.getEntree(piLigne)[0];

	string typeStr = poArticleStructure.getEntree(piLigne)[1];
	char type = NULL;
	if (typeStr.length() > 0)
		type = typeStr[0];

	string prixStr = poArticleStructure.getEntree(piLigne)[2];
	float prix = stof(prixStr.c_str());

	string description = poArticleStructure.getEntree(piLigne)[3];

	string etat = poArticleStructure.getEntree(piLigne)[4];

	string dateFabricationStr = poArticleStructure.getEntree(piLigne)[5];
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
					string attribut1Str = poArticleStructure.getEntree(piLigne)[6];
			int attribut1 = atoi(attribut1Str.c_str());

			string attribut2 = poArticleStructure.getEntree(piLigne)[7];

			string attribut3Str = poArticleStructure.getEntree(piLigne)[8];
			int attribut3 = atoi(attribut3Str.c_str());

			loArticle = new Voiture(nomArticle, prix, description, vraiEtat, dateFabrication, attribut1, attribut2, attribut3);
		}
			break;
		case 'B':
		{
			string attribut1Str = poArticleStructure.getEntree(piLigne)[6];
			int attribut1 = atoi(attribut1Str.c_str());

			string attribut2 = poArticleStructure.getEntree(piLigne)[7];

			loArticle = new Bijou(nomArticle, prix, description, vraiEtat, dateFabrication, attribut2, attribut1);
		}
			break;
	}

	return loArticle;
}

Employe* ClientApp::getEmployeFromStructure(const Fichier& poEmployeStructure, size_t piLigne)
{
	// On v�rifie que la ligne de de l'employ� est valide.
	if (poEmployeStructure.nombreEntrees() - 1 < piLigne)
		return nullptr;

	// On construit l'employ� � partir de la ligne de la structure.
	string nomP = poEmployeStructure.getEntree(0)[1];

	string prenomP = poEmployeStructure.getEntree(0)[2];

	string adresseP = poEmployeStructure.getEntree(0)[3];

	string soldeStrP = poEmployeStructure.getEntree(0)[4];
	float soldeP = stof(soldeStrP.c_str());

	string forfaitStr = poEmployeStructure.getEntree(0)[5];
	char forfait = NULL;
	if (forfaitStr.length() > 0)
		forfait = forfaitStr[0];

	string salaireStr = poEmployeStructure.getEntree(0)[6];
	float salaire = stof(salaireStr.c_str());

	string rabaisStr = poEmployeStructure.getEntree(0)[7];
	float rabais = stof(rabaisStr.c_str());

	Employe* loEmploye = new Employe(nomP, prenomP, adresseP, new Compte(soldeP), salaire, rabais);

	return loEmploye;
}

Employe* ClientApp::getEmployeExistant(string& psNomCompteEmploye)
{
	Employe* loEmploye = nullptr;
	bool lbFound = false;
	for (size_t cpt = 0; cpt < comptesEmployes.size() && lbFound == false; cpt++)
	{
		if (psNomCompteEmploye != comptesEmployes[cpt])
			continue;

		lbFound = true;
		loEmploye = marcheAuxPuces->getPersonnel()[cpt];
	}
	return loEmploye;
}

//Client* ClientApp::getClientFromStructure(vector<vector<string>>& poClientStructure, size_t piLigne)
//{
//}

// Effectuer la vente d'un article du client
bool ClientApp::venteArticleDuClient(int piNumeroArticle, int piIndexClient)
{
	int liPositionArticle = piNumeroArticle - 1;
	bool lbPrixValide = marcheAuxPuces->validerCompte(clients[piIndexClient]->getArticles()[liPositionArticle]->getPrix());
	if (!lbPrixValide)
		return false;

	marcheAuxPuces->acheter(clients[piIndexClient]->getArticles()[liPositionArticle]); //On cr�e la transaction pour le march� aux puces
	Vendeur* vnd;
	if (vnd = dynamic_cast<Vendeur*>(clients[piIndexClient]))
	{
		vnd->ajouterTransaction(liPositionArticle, marcheAuxPuces, clients[piIndexClient]->getArticles()[liPositionArticle]); //On appelle la fonction acheter de client
	}
	return true;
}

bool ClientApp::venteArticleAuClient(int piNumeroArticle, int piIndexClient)
{
	int liPositionArticle = piNumeroArticle - 1;
	bool lbPrixValide = clients[piIndexClient]->validerCompte(marcheAuxPuces->getArticlesEnVente()[liPositionArticle]->getPrix());
	if (!lbPrixValide) //Si le client n'ach�te pas l'article
		return false;

	Acheteur* acht;
	if (acht = dynamic_cast<Acheteur*>(clients[piIndexClient]))
	{
		acht->acheter(marcheAuxPuces->getArticlesEnVente()[liPositionArticle]); //On appelle la fonction acheter de client
	}
	return true;
}