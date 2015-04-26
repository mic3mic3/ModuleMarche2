#include "Affichage.h"

#include <iostream> //Pour les inputs/outputs avec cout/cin
#include <string>
#include <fstream> //Pour les opérations dans un fichier
#include <vector>
#include <iomanip> //Pour des opérations sur l'affichage (utilisé pour mieux aligner les trucs)
#include <sstream> //Pour la conversion int en string

#include "Fichier.h"
#include "ExceptionMarche.h"

#include "Article.h"
#include "Vendeur.h"
#include "Acheteur.h"
#include "Superclient.h"
#include "Employe.h"
#include "MarcheAuxPuces.h"
#include "Bijou.h"
#include "Divers.h"
#include "Voiture.h"
#include "Compte.h"
#include "Transaction.h"
#include "FabriqueClient.h"
#include "Simulation.h"

#include "ClientApp.h"

using namespace std; //Pour ne jamais avoir à écrire std:: puisque j'utilise beaucoup de fonctions de std dans ce fichier

const string Affichage::CS_EXIT_INPUT = "exit";
const int Affichage::CI_INDEX_CLIENT_CONNECTE = 0; // Utiliser dans le vecteur de clients de ClientApp.  Le client à l'indice 0 est le client connecté dans l'affichage.
ClientApp Affichage::clientApp = ClientApp();

Affichage::Affichage(void)
{
}

Affichage::~Affichage(void)
{
}

//On affiche le premier menu, l'utilisateur décide s'il veut faire un nouveau compte ou se connecter, puis on renvoit le choix
void Affichage::menuDemarrer()
{
	string choix = "1";
	do
	{
		system("cls"); //Pour effacer la console
		if (choix[0] != '1' && choix[0] != '2')
		{
			cout << "Erreur dans le choix!" << endl;
		}
		cout << "Que voulez-vous faire?" << endl;
		cout << "1 - Nouveau compte" << endl;
		cout << "2 - Se connecter avec un compte existant" << endl;
		cout << "3 - Quitter le programme" << endl;
		getline(cin, choix);
		try
		{
			if (choix[0] == '1')
			{
				menuInscription();
			}
			else if (choix[0] == '2')
			{
				menuConnexion();
			}
		}
		catch (ExceptionMarche loExceptionMarche)
		{
			loExceptionMarche.Gerer();
		}
	} while (choix[0] != '3');
}

string Affichage::validationEntreesChaines(string titre)
{
	string chaine;
	do
	{
		if (chaine == ";")
		{
			cout << "Erreur dans l'entree" << endl;
		}
		cout << titre << " (" << CS_EXIT_INPUT << " pour sortir): ";
		
		getline(cin, chaine);
		if (chaine.length() == 0)
		{
			chaine = ";";
		}
		for (unsigned int cpt = 0; cpt < chaine.length(); cpt++)
		{
			if (chaine[cpt] == ';')
			{
				chaine = ";";
				break;
			}
		}
	} while (chaine == ";");
	return chaine;
}

//Vérifier si on peut transférer le string en float (chiffres et un point: «99999.99» ou «9» ou «.9»).
bool Affichage::validationFloat(const string & solde)
{
	//Vérifier si on peut transférer le string en float
	int avantPoint = 0;
	int nbPoints = 0;
	int decimales = 0;
	for (size_t cpt = 0; cpt < solde.length(); cpt++)
	{
		if (solde[cpt] == '.')
		{
			nbPoints++;
			if (nbPoints > 1)
			{
				cout << "Erreur: Trop de points dans le solde." << endl;
				return false;
			}
		}
		else if (isdigit(solde[cpt]))
		{
			if (nbPoints > 0)
			{
				decimales++;
				if (decimales > 2)
				{
					cout << "Erreur: Trop de decimales." << endl;
					return false;
				}
			}
			else
			{
				avantPoint++;
				if (avantPoint > 7)
				{
					cout << "Erreur: Trop de chiffres avant le point." << endl;
					return false;
				}
			}
		}
		else
		{
			cout << "Erreur: Contient un caractere invalide." << endl;
			return false;
		}
	}
	return true;
}

//Création du fichier avec les informations sur le compte (première ligne du fichier)
void Affichage::creationFichierCompte(const string & nomCompte, const string & nom, const string & prenom, const string & adresse, float solde, const string & forfait)
{
	fstream compte(nomCompte + ".txt", ios::app);
	if (compte)
	{
		compte << nomCompte << ";" << nom << ";" << prenom << ";" << adresse << ";" << solde << ";" << forfait << "\n";
	}

	compte.close();
}

//Affichage du menu d'inscription et vérification s'il existe déjà
void Affichage::menuInscription()
{
	system("cls");
	cout << "Inscription" << endl;

	string nomCompte;
	string nom;
	string prenom;
	string adresse;
	string solde;
	bool lbCompteDejaExistant = false;
	do
	{
		if (lbCompteDejaExistant)
			cout << "Ce compte existe deja" << endl;
		nomCompte = "";
		//cout << "Nom de compte (" << CS_EXIT_INPUT << " pour sortir): ";
		//getline(cin, nomCompte);
		if ((nomCompte = validationEntreesChaines("Nom de compte")) == "exit")
		{
			return;
		}
		/*if (nomCompte == CS_EXIT_INPUT)
		{
			return;
		}*/
	} while (lbCompteDejaExistant = Fichier::fichierExistant(nomCompte)); // Si le compte existe déjà, on demande d'en saisir un différent.

	//Entrée de différentes informations
	if ((nom = validationEntreesChaines("Nom")) == "exit")
	{
		return;
	}
	cout << "Prenom: ";
	getline(cin, prenom);
	cout << "Adresse: ";
	getline(cin, adresse);

	do
	{
		cout << "Solde (maximum de 7 chiffres avant le point et de 2 decimales): ";
		getline(cin, solde);
	} while (!validationFloat(solde));
	string forfait;
	bool first = true;
	Client* client = NULL;
	do
	{
		if (client == nullptr && !first)
		{
			cout << "Impossible de s'abonner à ce forfait." << endl;
		}
		cout << "Forfait (A = Acheteur -> 5$, V = Vendeur -> 20$, S = Superclient -> 25$ ou "+CS_EXIT_INPUT+" pour sortir): ";
		getline(cin, forfait);
		if (forfait == "exit")
		{
			return;
		}
		first = false;
	} while ((client = clientApp.inscription(nom, prenom, adresse, stof(solde.c_str()), forfait)) == nullptr);

	//Puisque tout est valide, on crée le fichier avec les informations sur le compte (première ligne du fichier)
	creationFichierCompte(nomCompte, nom, prenom, adresse, client->getCompte()->getSolde(), forfait);
	delete client;

	Fichier loFichierClient = Fichier(Fichier::getContenu(nomCompte));
	Fichier loFichierMarcheAuxPuces = Fichier(Fichier::getContenu(ClientApp::CS_NOM_MARCHE_AUX_PUCES));
	vector<Fichier> loFichiersEmploye = getEntreesEmploye(Fichier::getContenu(ClientApp::CS_NOM_MARCHE_AUX_PUCES + "_Employes"));
	clientApp.connexion(loFichierClient, loFichierMarcheAuxPuces, loFichiersEmploye);
	menuSelection();
}

//Affichage du menu de connexion, puis vérification de l'existence du compte, puis on renvoie le nom du compte avec lequel on s'est connecté
void Affichage::menuConnexion()
{
	system("cls");
	cout << "Connexion" << endl;

	string nomCompte = "";
	bool compteValide = true;
	do
	{
		if (!compteValide)
		{
			cout << "Compte non-existant" << endl;
		}
		compteValide = false;
		cout << "Nom de compte (" << CS_EXIT_INPUT << " pour sortir): ";
		getline(cin, nomCompte);
		if (nomCompte == CS_EXIT_INPUT)
		{
			return;
		}
	} while (!Fichier::fichierExistant(nomCompte));

	clientApp.connexion(Fichier::getContenu(nomCompte), Fichier::getContenu(ClientApp::CS_NOM_MARCHE_AUX_PUCES), getEntreesEmploye(Fichier(Fichier::getContenu(ClientApp::CS_NOM_MARCHE_AUX_PUCES + "_Employes"))));
	menuSelection();
}

//Affichage du menu de sélection de fonctions une fois connecté, puis on renvoie le choix -> il change selon le forfait du client
void Affichage::menuSelection()
{
	string choix = "1";
	string lsValeurQuitter = "";
	Employe* emp;
	Vendeur* vnd;
	Acheteur* ach;
	Superclient* sup;
	bool valide = true;
	do
	{
		system("cls");
		if (sup = dynamic_cast<Superclient*>(getClientConnecte()))
		{
			lsValeurQuitter = "5";
			if (!valide)
			{
				cout << "Erreur dans le choix!" << endl;
			}
			cout << "Que voulez-vous faire?" << endl;
			cout << "1 - Voir mes articles" << endl;
			cout << "2 - Voir les articles achetables" << endl;
			cout << "3 - Vendre mes articles" << endl;
			cout << "4 - Changer mon forfait" << endl;
			cout << "5 - Deconnexion" << endl;
			getline(cin, choix);
			valide = true;
			if (choix == "1")
			{
				menuAchats();
			}
			else if (choix == "2")
			{
				menuCategories();
			}
			else if (choix == "3")
			{
				menuVenteArticles();
			}
			else if (choix == "4")
			{
				menuForfaits();
			}
			else
			{
				valide = false;
			}
		}
		else if (emp = dynamic_cast<Employe*>(getClientConnecte()))
		{
			lsValeurQuitter = "3";
			if (!valide)
			{
				cout << "Erreur dans le choix!" << endl;
			}
			cout << "Que voulez-vous faire?" << endl;
			cout << "1 - Voir mes articles" << endl;
			cout << "2 - Voir les articles achetables" << endl;
			cout << "3 - Deconnexion" << endl;
			getline(cin, choix);
			valide = true;
			if (choix == "1")
			{
				menuAchats();
			}
			else if (choix == "2")
			{
				menuCategories();
			}
			else
			{
				valide = false;
			}
		}
		else if (ach = dynamic_cast<Acheteur*>(getClientConnecte()))
		{
			lsValeurQuitter = "4";
			if (!valide)
			{
				cout << "Erreur dans le choix!" << endl;
			}
			cout << "Que voulez-vous faire?" << endl;
			cout << "1 - Voir mes articles" << endl;
			cout << "2 - Voir les articles achetables" << endl;
			cout << "3 - Changer mon forfait" << endl;
			cout << "4 - Deconnexion" << endl;
			getline(cin, choix);
			valide = true;
			if (choix == "1")
			{
				menuAchats();
			}
			else if (choix == "2")
			{
				menuCategories();
			}
			else if (choix == "3")
			{
				menuForfaits();
			}
			else
			{
				valide = false;
			}
		}
		else if (vnd = dynamic_cast<Vendeur*>(getClientConnecte()))
		{
			lsValeurQuitter = "4";
			if (!valide)
			{
				cout << "Erreur dans le choix!" << endl;
			}
			cout << "Que voulez-vous faire?" << endl
				<< "1 - Voir mes articles" << endl
				<< "2 - Vendre mes articles" << endl
				<< "3 - Changer mon forfait" << endl
				<< "4 - Deconnexion" << endl;
			getline(cin, choix);
			valide = true;
			if (choix == "1")
			{
				menuAchats();
			}
			else if (choix == "2")
			{
				menuVenteArticles();
			}
			else if (choix == "3")
			{
				menuForfaits();
			}
			else
			{
				valide = false;
			}
		}
	}while (choix != lsValeurQuitter);

	clientApp.deconnexion();
}

//Affichage du menu pour changer de forfait
void Affichage::menuForfaits()
{
	string choix;
	choix = "A";
	bool first = true;
	Client* client = NULL;
	do
	{
		system("cls");
		if (client == nullptr && !first)
		{
			cout << "Impossible de s'abonner à ce forfait" << endl;
		}
		cout << "Quel forfait voulez-vous? (" << CS_EXIT_INPUT << " pour sortir):" << endl;
		cout << "A - Acheteur -> 5$" << endl;
		cout << "V - Vendeur -> 20$" << endl;
		cout << "S - Superclient -> 25$" << endl;
		getline(cin, choix);
		if (choix == CS_EXIT_INPUT)
		{
			return;
		}
		first = false;
	} while ((client = clientApp.changementForfait(choix[0], CI_INDEX_CLIENT_CONNECTE)) == nullptr);
	
	fstream achats(clientApp.getCompte() + ".txt");
	if (achats)
	{
		achats << clientApp.getCompte() << ";" 
			<< getClientConnecte()->getNom() << ";"
			<< getClientConnecte()->getPrenom() << ";"
			<< getClientConnecte()->getAdresse() << ";"
			<< getClientConnecte()->getCompte()->getSolde();
		Employe* emp;
		Superclient* sup;
		Vendeur* vnd;
		Acheteur* ach;
		if (sup = dynamic_cast<Superclient*>(getClientConnecte()))
		{
			achats << ";S\n";
		}
		else if (emp = dynamic_cast<Employe*>(getClientConnecte()))
		{
			achats << ";E;" << emp->getSalaire() << ";" << emp->getRabais() << "\n";
		}
		else if (ach = dynamic_cast<Acheteur*>(getClientConnecte()))
		{
			achats << ";A\n";
		}
		else if (vnd = dynamic_cast<Vendeur*>(getClientConnecte()))
		{
			achats << ";V\n";
		}
		for (size_t cpt = 0; cpt < getClientConnecte()->getArticles().size(); cpt++)
		{
			ostringstream conversion; //Conversion avec sstream d'un int en string
			conversion << getClientConnecte()->getArticles()[cpt]->getDate().jour << "/"
				<< getClientConnecte()->getArticles()[cpt]->getDate().mois << "/"
				<< getClientConnecte()->getArticles()[cpt]->getDate().annee;
			string date = conversion.str();
			Divers* div;
			Bijou* bij;
			Voiture* voit;
			if (div = dynamic_cast<Divers*>(getClientConnecte()->getArticles()[cpt]))
			{
				achats << getClientConnecte()->getArticles()[cpt]->getNom() << ";D;"
					<< getClientConnecte()->getArticles()[cpt]->getPrix() << ";"
					<< getClientConnecte()->getArticles()[cpt]->getDescription() << ";"
					<< getClientConnecte()->getArticles()[cpt]->getEtat()->getDescription() << ";"
					<< date << "\n";
			}
			else if (bij = dynamic_cast<Bijou*>(getClientConnecte()->getArticles()[cpt]))
			{
				achats << getClientConnecte()->getArticles()[cpt]->getNom() << ";B;" 
					<< getClientConnecte()->getArticles()[cpt]->getPrix() << ";" 
					<< getClientConnecte()->getArticles()[cpt]->getDescription() << ";" 
					<< getClientConnecte()->getArticles()[cpt]->getEtat()->getDescription() << ";"
					<< date << ";" 
					<< bij->getPurete() << ";" 
					<< bij->getMateriau() << "\n";
			}
			else if (voit = dynamic_cast<Voiture*>(getClientConnecte()->getArticles()[cpt]))
			{
				achats << getClientConnecte()->getArticles()[cpt]->getNom() << ";V;" 
					<< getClientConnecte()->getArticles()[cpt]->getPrix() << ";" 
					<< getClientConnecte()->getArticles()[cpt]->getDescription() << ";" 
					<< getClientConnecte()->getArticles()[cpt]->getEtat()->getDescription() << ";"
					<< date << ";" 
					<< voit->getKilometrage() << ";" 
					<< voit->getCouleur() << ";" 
					<< voit->getAnnee() << "\n";
			}
		}
	}
}

//Affichage des achats faits par le client
void Affichage::menuAchats()
{
	system("cls");
	cout << "Mes achats" << endl << endl;
	cout << setw(15) << left << "Article" << setw(12) << left << "Prix" << setw(25) << left << "Description" << setw(17) << left << "Etat" << setw(10) << left << "Date" << endl << endl;
	for (size_t cpt = 0; cpt < getClientConnecte()->getArticles().size(); cpt++)
	{
		cout << getClientConnecte()->getArticles()[cpt];
	}
	if (getClientConnecte()->getArticles().size() == 0)
	{
		cout << "Aucun article en votre possession" << endl;
	}
	attendreTouche();
}

//Affichage du marché aux puces avec du solde du client et les articles à vendre du marché, puis on retourne le choix de l'article ou -1 si l'utilisateur veut sortir (a écrit exit)
int Affichage::menuMarche(float solde, const vector<Article*> &listeArticles, char categorie)
{
	bool valide = true;
	int retour;
	system("cls");
	do
	{
		if (!valide)
		{
			cout << "Erreur dans l'entree" << endl;
		}
		valide = true;
		int cpt2 = 0;
		//On met une précision aux variables float avec fixed et setprecision(), puis on aligne bien les colonnes avec setw() et left
		cout << "Marche Aux Puces (Mode Achat)" << "\tVotre solde: " << fixed << setprecision(2) << solde << endl << endl;
		cout << setw(4) << left << "    " << setw(13) << left << "Article" << setw(10) << left << "Prix" << setw(25) << left << "Description" << setw(17) << left << "Etat" << setw(10) << left << "Date" << endl << endl;
		for (size_t cpt = 0; cpt < listeArticles.size(); cpt++)
		{
			switch (categorie)
			{
			case 'T':
				cout << setw(4) << left << (cpt2 + 1) << listeArticles[cpt];
				cpt2++;
				break;
			case 'B':
				Bijou* bij;
				if (bij = dynamic_cast<Bijou*>(listeArticles[cpt]))
				{
					cout << setw(4) << left << (cpt2 + 1) << listeArticles[cpt];
					cpt2++;
				}
				break;
			case 'V':
				Voiture* voi;
				if (voi = dynamic_cast<Voiture*>(listeArticles[cpt]))
				{
					cout << setw(4) << left << (cpt2 + 1) << listeArticles[cpt];
					cpt2++;
				}
				break;
			case 'D':
				Divers* div;
				if (div = dynamic_cast<Divers*>(listeArticles[cpt]))
				{
					cout << setw(4) << left << (cpt2 + 1) << listeArticles[cpt];
					cpt2++;
				}
				break;
			default:
				break;
			}
		}
		if (listeArticles.size() == 0)
		{
			cout << "Aucun article en vente" << endl;
		}
		string choix;
		cout << endl << "Choisir l'article selon son numero (" << CS_EXIT_INPUT << " pour sortir):" << endl;
		getline(cin, choix);

		if (choix == CS_EXIT_INPUT)
		{
			return -1;
		}
		retour = validationChoixArticle(choix, cpt2);
	} while (retour == 0);

	retour = transformationEnPositionTableau(retour, categorie, listeArticles);
	listeArticles[retour - 1]->afficherDetails();

	return retour;
}

int Affichage::transformationEnPositionTableau(int retour, char categorie, const vector<Article*> & listeArticles)
{
	int cpt2 = 0;
	for (size_t cpt = 0; cpt < listeArticles.size(); cpt++)
	{
		switch (categorie)
		{
		case 'B':
			Bijou* bij;
			if (bij = dynamic_cast<Bijou*>(listeArticles[cpt]))
			{
				cpt2++;
				if (cpt2 == retour)
				{
					return cpt + 1;
				}
			}
			break;
		case 'V':
			Voiture* voi;
			if (voi = dynamic_cast<Voiture*>(listeArticles[cpt]))
			{
				cpt2++;
				if (cpt2 == retour)
				{
					return cpt + 1;
				}
			}
			break;
		case 'D':
			Divers* div;
			if (div = dynamic_cast<Divers*>(listeArticles[cpt]))
			{
				cpt2++;
				if (cpt2 == retour)
				{
					return cpt + 1;
				}
			}
			break;
		default:
			return retour;
			break;
		}
	}

	return -1;
}

//Affichage de la vérification de l'achat selon si le solde était assez grand pour l'achat de l'objet choisi, puis on renvoit si l'utilisateur valide l'achat (true) ou non (false)
bool Affichage::menuVerifAchat(bool soldeOk)
{
	string choixStr = "O";
	if (!soldeOk)
	{
		cout << "L'acheteur n'a pas assez d'argent pour l'article." << endl;
		getline(cin, choixStr);
		return false;
	}

	do
	{
		if (choixStr != "O" && choixStr != "N")
		{
			cout << "Erreur! Veuillez entrer O ou N" << endl;
		}
		cout << "L'acheteur a assez d'argent pour l'article." << endl;

		cout << "Etes-vous sur de vouloir proceder? (O/N)" << endl;
		getline(cin, choixStr);
	} while (choixStr != "O" && choixStr != "N");

	return choixStr == "O";
}

//Afficher et acheter les articles du marché
void Affichage::menuCategories()
{
	string lsChoixCategorie = "V";
	do
	{
		system("cls");
		if (lsChoixCategorie != "V" && lsChoixCategorie != "B" && lsChoixCategorie != "D" && lsChoixCategorie != "T")
		{
			cout << "Erreur dans le choix!" << endl;
		}
		cout << "Par quel type voulez-vous chercher vos produits?" << endl;
		cout << "V - Voitures" << endl;
		cout << "B - Bijoux" << endl;
		cout << "D - Divers" << endl;
		cout << "T - Tout" << endl;
		cout << "Q - Quitter" << endl;
		getline(cin, lsChoixCategorie);
		if (lsChoixCategorie == "Q")
		{
			return;
		}
	} while (lsChoixCategorie != "V" && lsChoixCategorie != "B" && lsChoixCategorie != "D" && lsChoixCategorie != "T");

	int liNumeroArticleChoisi = menuMarche(getClientConnecte()->getCompte()->getSolde(), clientApp.getMarcheAuxPuces()->getArticlesEnVente(), lsChoixCategorie[0]);
	if (liNumeroArticleChoisi == -1) // L'utilisateur a entré la vleur pour quitter CS_EXIT_INPUT (exit)
		return;

	bool prixValide = getClientConnecte()->validerCompte(clientApp.getMarcheAuxPuces()->getArticlesEnVente()[liNumeroArticleChoisi - 1]->getPrixEtat());
	if (!Affichage::menuVerifAchat(prixValide)) //Si le client n'achète pas l'article
		return;

	int  liIndexArticle = liNumeroArticleChoisi - 1;
	if (!clientApp.venteArticleAuClient(liIndexArticle, CI_INDEX_CLIENT_CONNECTE))
	{
		cout << "Transaction annulee" << endl;
		attendreTouche();
		return;
	}

	cout << "Transaction completee" << endl;
	attendreTouche();

	//On ajoute l'achat au fichier du client
	fstream achats(clientApp.getCompte() + ".txt");
	if (achats)
	{
		achats << clientApp.getCompte() << ";" 
			<< getClientConnecte()->getNom() << ";" 
			<< getClientConnecte()->getPrenom() << ";" 
			<< getClientConnecte()->getAdresse() << ";" 
			<< getClientConnecte()->getCompte()->getSolde();
		Employe* emp;
		Superclient* sup;
		Vendeur* vnd;
		Acheteur* ach;
		if (sup = dynamic_cast<Superclient*>(getClientConnecte()))
		{
			achats << ";S\n";
		}
		else if (emp = dynamic_cast<Employe*>(getClientConnecte()))
		{
			achats << ";E;" << emp->getSalaire() << ";" << emp->getRabais() << "\n";
		}
		else if (ach = dynamic_cast<Acheteur*>(getClientConnecte()))
		{
			achats << ";A\n";
		}
		else if (vnd = dynamic_cast<Vendeur*>(getClientConnecte()))
		{
			achats << ";V\n";
		}

		for (size_t cpt = 0; cpt < getClientConnecte()->getArticles().size(); cpt++)
		{
			ostringstream conversion; //Conversion avec sstream d'un int en string
			conversion << getClientConnecte()->getArticles()[cpt]->getDate().jour << "/"
				<< getClientConnecte()->getArticles()[cpt]->getDate().mois << "/"
				<< getClientConnecte()->getArticles()[cpt]->getDate().annee;
			string date = conversion.str();
			Divers* div;
			Bijou* bij;
			Voiture* voit;
			if (div = dynamic_cast<Divers*>(getClientConnecte()->getArticles()[cpt]))
			{
				achats << getClientConnecte()->getArticles()[cpt]->getNom() << ";D;" 
					<< getClientConnecte()->getArticles()[cpt]->getPrix() << ";" 
					<< getClientConnecte()->getArticles()[cpt]->getDescription() << ";" 
					<< getClientConnecte()->getArticles()[cpt]->getEtat()->getDescription() << ";" 
					<< date << "\n";
			}
			else if (bij = dynamic_cast<Bijou*>(getClientConnecte()->getArticles()[cpt]))
			{
				achats << getClientConnecte()->getArticles()[cpt]->getNom() << ";B;" 
					<< getClientConnecte()->getArticles()[cpt]->getPrix() << ";" 
					<< getClientConnecte()->getArticles()[cpt]->getDescription() << ";" 
					<< getClientConnecte()->getArticles()[cpt]->getEtat()->getDescription() << ";" 
					<< date << ";" 
					<< bij->getPurete() << ";" 
					<< bij->getMateriau() << "\n";
			}
			else if (voit = dynamic_cast<Voiture*>(getClientConnecte()->getArticles()[cpt]))
			{
				achats << getClientConnecte()->getArticles()[cpt]->getNom() << ";V;" 
					<< getClientConnecte()->getArticles()[cpt]->getPrix() << ";" 
					<< getClientConnecte()->getArticles()[cpt]->getDescription() << ";" 
					<< getClientConnecte()->getArticles()[cpt]->getEtat()->getDescription() << ";" 
					<< date << ";" 
					<< voit->getKilometrage() << ";" 
					<< voit->getCouleur() << ";" 
					<< voit->getAnnee() << "\n";
			}
		}
	}
	achats.close();

	//On append la transaction dans le fichier de transaction du marché aux puces
	fstream trans(clientApp.getMarcheAuxPuces()->getNom() + "_Trans.txt", ios::app);
	if (trans)
	{
		trans << "Client: " 
			<< clientApp.getMarcheAuxPuces()->getDerniereTransaction().client->getNom() << ";" 
			<< clientApp.getMarcheAuxPuces()->getDerniereTransaction().client->getPrenom() << ";" 
			<< clientApp.getMarcheAuxPuces()->getDerniereTransaction().client->getAdresse() << "\n";
		trans << "Marche: " 
			<< clientApp.getMarcheAuxPuces()->getDerniereTransaction().marche->getNom() << ";" 
			<< clientApp.getMarcheAuxPuces()->getDerniereTransaction().marche->getAdresse() << "\n";
		trans << "Article: " 
			<< clientApp.getMarcheAuxPuces()->getDerniereTransaction().article->getNom() << ";" 
			<< clientApp.getMarcheAuxPuces()->getDerniereTransaction().article->getPrix() << ";" 
			<< clientApp.getMarcheAuxPuces()->getDerniereTransaction().article->getDescription() << ";" 
			<< clientApp.getMarcheAuxPuces()->getDerniereTransaction().article->getEtat()->getDescription() << ";" 
			<< clientApp.getMarcheAuxPuces()->getDerniereTransaction().article->getDate().jour << "/" 
			<< clientApp.getMarcheAuxPuces()->getDerniereTransaction().article->getDate().mois << "/" 
			<< clientApp.getMarcheAuxPuces()->getDerniereTransaction().article->getDate().annee << "\n";
		trans << "Date: " << clientApp.getMarcheAuxPuces()->getDerniereTransaction().date.jour << "/" 
			<< clientApp.getMarcheAuxPuces()->getDerniereTransaction().date.mois << "/" 
			<< clientApp.getMarcheAuxPuces()->getDerniereTransaction().date.annee << "\n";
		trans << "-----\n";
	}
	trans.close();

	//On enlève l'article qui vient d'être vendu au fichier du marché aux puces
	ofstream marche(clientApp.getMarcheAuxPuces()->getNom() + ".txt");
	if (marche)
	{
		marche << clientApp.getMarcheAuxPuces()->getNom() << ";" 
			<< clientApp.getMarcheAuxPuces()->getAdresse() << ";" 
			<< clientApp.getMarcheAuxPuces()->getRevenu() << "\n";
		for (size_t cpt = 0; cpt < clientApp.getMarcheAuxPuces()->getArticlesEnVente().size(); cpt++)
		{
			ostringstream conversion;
			conversion << clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getDate().jour << "/" 
				<< clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getDate().mois << "/" 
				<< clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getDate().annee;
			string date = conversion.str();
			Divers* div;
			Bijou* bij;
			Voiture* voit;
			if (div = dynamic_cast<Divers*>(clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]))
			{
				marche << clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getNom() << ";D;" 
					<< clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getPrix() << ";" 
					<< clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getDescription() << ";"
					<< clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getEtat()->getDescription() << ";"
					<< date << "\n";
			}
			else if (bij = dynamic_cast<Bijou*>(clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]))
			{
				marche << clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getNom() << ";B;" 
					<< clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getPrix() << ";" 
					<< clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getDescription() << ";" 
					<< clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getEtat()->getDescription() << ";"
					<< date << ";" 
					<< bij->getPurete() << ";" 
					<< bij->getMateriau() << "\n";
			}
			else if (voit = dynamic_cast<Voiture*>(clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]))
			{
				marche << clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getNom() << ";V;" 
					<< clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getPrix() << ";" 
					<< clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getDescription() << ";" 
					<< clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getEtat()->getDescription() << ";"
					<< date << ";" 
					<< voit->getKilometrage() << ";" 
					<< voit->getCouleur() << ";" 
					<< voit->getAnnee() << "\n";
			}
		}
	}
	marche.close();
}

//Validation choix d'un article (vente ou achat)
int Affichage::validationChoixArticle(const string & choix, int max)
{
	system("cls");
	for (size_t cpt = 0; cpt < choix.length(); cpt++)
	{
		if (!isdigit(choix[cpt]))
		{
			cout << "Erreur: Digits seulement" << endl;
			return 0;
		}
	}
	int retour = atoi(choix.c_str());
	if (!(retour > 0 && retour <= max))
	{
		cout << "Erreur: Nombre invalide." << endl;
		return 0;
	}
	return retour;
}

//Le menu pour la vente d'articles du client
void Affichage::menuVenteArticles()
{
	int retour;
	system("cls");
	do
	{
		//On met une précision aux variables float avec fixed et setprecision(), puis on aligne bien les colonnes avec setw() et left
		cout << "Marche Aux Puces (Mode Vente)" << "\tSon solde: " << fixed << setprecision(2) << clientApp.getMarcheAuxPuces()->getRevenu() << endl << endl;
		cout << setw(4) << left << "    " << setw(13) << left << "Article" << setw(10) << left << "Prix" << setw(25) << left << "Description" << setw(17) << left << "Etat" << setw(10) << left << "Date" << endl << endl;
		
		if (getClientConnecte()->getArticles().size() == 0)
		{
			cout << "Aucun article a vendre" << endl;
		}
		else
		{
			// Affichage des articles
			for (size_t cpt = 0; cpt < getClientConnecte()->getArticles().size(); cpt++)
			{
				cout << setw(4) << left << (cpt + 1) << getClientConnecte()->getArticles()[cpt];
			}
		}

		string choix;
		cout << endl << "Choisir l'article selon son numero (" << CS_EXIT_INPUT << " pour sortir):" << endl;
		getline(cin, choix);
		if (choix == CS_EXIT_INPUT)
			return;

		retour = validationChoixArticle(choix, getClientConnecte()->getArticles().size());
	} while (retour == 0);

	cout << getClientConnecte()->getArticles()[retour - 1]->afficherDetails();

	bool lbPrixValide = clientApp.getMarcheAuxPuces()->validerCompte(getClientConnecte()->getArticles()[retour - 1]->getPrixEtat());
	bool lbClientVeutVendre = menuVerifAchat(lbPrixValide); //On renvoit un message selon la possibilité de l'achat de l'article après la vérification du solde du client
	if (!lbClientVeutVendre) //Si le client ne veut pas vendre son article
		return;

	int liIndexArticle = retour - 1;
	if (!clientApp.venteArticleDuClient(liIndexArticle, CI_INDEX_CLIENT_CONNECTE))
	{
		cout << "Transaction annulee" << endl;
		attendreTouche();
		return;
	}

	cout << "Transaction completee" << endl;
	attendreTouche();

	//On ajoute l'achat au fichier du client
	fstream achats(clientApp.getCompte() + ".txt", std::ofstream::out | std::ofstream::trunc);
	if (achats)
	{
		achats << clientApp.getCompte() << ";" 
			<< getClientConnecte()->getNom() << ";" 
			<< getClientConnecte()->getPrenom() << ";" 
			<< getClientConnecte()->getAdresse() << ";" 
			<< getClientConnecte()->getCompte()->getSolde();
		Employe* emp;
		Superclient* sup;
		Vendeur* vnd;
		Acheteur* ach;
		if (sup = dynamic_cast<Superclient*>(getClientConnecte()))
		{
			achats << ";S\n";
		}
		else if (emp = dynamic_cast<Employe*>(getClientConnecte()))
		{
			achats << ";E;" << emp->getSalaire() << ";" << emp->getRabais() << "\n";
		}
		else if (ach = dynamic_cast<Acheteur*>(getClientConnecte()))
		{
			achats << ";A\n";
		}
		else if (vnd = dynamic_cast<Vendeur*>(getClientConnecte()))
		{
			achats << ";V\n";
		}

		for (size_t cpt = 0; cpt < getClientConnecte()->getArticles().size(); cpt++)
		{
			ostringstream conversion; //Conversion avec sstream d'un int en string
			conversion << getClientConnecte()->getArticles()[cpt]->getDate().jour << "/" 
				<< getClientConnecte()->getArticles()[cpt]->getDate().mois << "/" 
				<< getClientConnecte()->getArticles()[cpt]->getDate().annee;
			string date = conversion.str();
			Divers* div;
			Bijou* bij;
			Voiture* voit;
			if (div = dynamic_cast<Divers*>(getClientConnecte()->getArticles()[cpt]))
			{
				achats << getClientConnecte()->getArticles()[cpt]->getNom() << ";D;" 
					<< getClientConnecte()->getArticles()[cpt]->getPrix() << ";" 
					<< getClientConnecte()->getArticles()[cpt]->getDescription() << ";" 
					<< getClientConnecte()->getArticles()[cpt]->getEtat() << ";" 
					<< date << "\n";
			}
			else if (bij = dynamic_cast<Bijou*>(getClientConnecte()->getArticles()[cpt]))
			{
				achats << getClientConnecte()->getArticles()[cpt]->getNom() << ";B;" 
					<< getClientConnecte()->getArticles()[cpt]->getPrix() << ";" 
					<< getClientConnecte()->getArticles()[cpt]->getDescription() << ";" 
					<< getClientConnecte()->getArticles()[cpt]->getEtat() << ";" 
					<< date << ";" 
					<< bij->getPurete() << ";" 
					<< bij->getMateriau() << "\n";
			}
			else if (voit = dynamic_cast<Voiture*>(getClientConnecte()->getArticles()[cpt]))
			{
				achats << getClientConnecte()->getArticles()[cpt]->getNom() << ";V;" 
					<< getClientConnecte()->getArticles()[cpt]->getPrix() << ";" 
					<< getClientConnecte()->getArticles()[cpt]->getDescription() << ";" 
					<< getClientConnecte()->getArticles()[cpt]->getEtat() << ";" 
					<< date << ";" 
					<< voit->getKilometrage() << ";" 
					<< voit->getCouleur() << ";" 
					<< voit->getAnnee() << "\n";
			}
		}
	}
	achats.close();

	//On append la transaction dans le fichier de transactions du client
	fstream trans(clientApp.getCompte() + "_Trans.txt", ios::app);
	if (trans)
	{
		Vendeur* vnd;
		if (vnd = dynamic_cast<Vendeur*>(getClientConnecte()))
		{
			trans << "Client: " << vnd->getDerniereTransaction()->client->getNom() << ";"
				<< vnd->getDerniereTransaction()->client->getPrenom() << ";"
				<< vnd->getDerniereTransaction()->client->getAdresse() << "\n";
			trans << "Marche: " << vnd->getDerniereTransaction()->marche->getNom() << ";"
				<< vnd->getDerniereTransaction()->marche->getAdresse() << "\n";
			trans << "Article: " << vnd->getDerniereTransaction()->article->getNom() << ";"
				<< vnd->getDerniereTransaction()->article->getPrix() << ";"
				<< vnd->getDerniereTransaction()->article->getDescription() << ";"
				<< vnd->getDerniereTransaction()->article->getEtat()->getDescription() << ";"
				<< vnd->getDerniereTransaction()->article->getDate().jour << "/"
				<< vnd->getDerniereTransaction()->article->getDate().mois << "/"
				<< vnd->getDerniereTransaction()->article->getDate().annee << "\n";
			trans << "Date: " << vnd->getDerniereTransaction()->date.jour << "/"
				<< vnd->getDerniereTransaction()->date.mois << "/"
				<< vnd->getDerniereTransaction()->date.annee << "\n";
			trans << "-----\n";
		}
	}
	trans.close();

	//On enlève l'article qui vient d'être vendu au fichier du marché aux puces
	ofstream marche(clientApp.getMarcheAuxPuces()->getNom() + ".txt");
	if (marche)
	{
		marche << clientApp.getMarcheAuxPuces()->getNom() << ";" 
			<< clientApp.getMarcheAuxPuces()->getAdresse() << ";" 
			<< clientApp.getMarcheAuxPuces()->getRevenu() << "\n";
		for (size_t cpt = 0; cpt < clientApp.getMarcheAuxPuces()->getArticlesEnVente().size(); cpt++)
		{
			ostringstream conversion;
			conversion << clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getDate().jour << "/" 
				<< clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getDate().mois << "/" 
				<< clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getDate().annee;
			string date = conversion.str();
			Divers* div;
			Bijou* bij;
			Voiture* voit;
			if (div = dynamic_cast<Divers*>(clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]))
			{
				marche << clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getNom() << ";D;" 
					<< clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getPrix() << ";" 
					<< clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getDescription() << ";" 
					<< clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getEtat()->getDescription() << ";" 
					<< date << "\n";
			}
			else if (bij = dynamic_cast<Bijou*>(clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]))
			{
				marche << clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getNom() << ";B;" 
					<< clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getPrix() << ";" 
					<< clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getDescription() << ";" 
					<< clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getEtat()->getDescription() << ";" 
					<< date << ";" 
					<< bij->getPurete() << ";" 
					<< bij->getMateriau() << "\n";
			}
			else if (voit = dynamic_cast<Voiture*>(clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]))
			{
				marche << clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getNom() << ";V;" 
					<< clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getPrix() << ";" 
					<< clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getDescription() << ";" 
					<< clientApp.getMarcheAuxPuces()->getArticlesEnVente()[cpt]->getEtat()->getDescription() << ";" 
					<< date << ";" 
					<< voit->getKilometrage() << ";" 
					<< voit->getCouleur() << ";" 
					<< voit->getAnnee() << "\n";
			}
		}
	}
	marche.close();
}

// À partir de la liste des noms de compte des employés, on revoie toutes les données de chaque employé.
// Comme on stocke habituellement le contenu d'un fichier dans vector<vector<string>>, et qu'ici on renvoie
// le contenu de plusieurs fichiers, on renvoie un vecteur de cette structure.
vector<Fichier> Affichage::getEntreesEmploye(const Fichier& poNomCompteEmploye)
{
	vector<Fichier> loEmployes = vector<Fichier>(poNomCompteEmploye.nombreEntrees());
	for (size_t cptLigne = 0; cptLigne < poNomCompteEmploye.nombreEntrees(); cptLigne++)
	{
		// Obtention des informations d'un employé.
		string lsNomCompteEmploye = poNomCompteEmploye.getEntree(cptLigne)[0];
		loEmployes[cptLigne] = Fichier(Fichier::getContenu(lsNomCompteEmploye));
	}
	return loEmployes;
}

int main()
{
	Affichage::menuDemarrer();
	//srand(static_cast<int>(time(0)));
	//Simulation simu;
	
	// Simulation d'une semaine
	//for (size_t cptJour = 1; cptJour <= 7; cptJour++)
	//{
	//	for (int cpt = 1; cpt <= 24; cpt++)
	//	{
	//		simu.miseAJour();
	//	}
	//}

	cout << "Fermeture de l'application..." << endl;

	Affichage::attendreTouche();
	return EXIT_SUCCESS;
}

void Affichage::attendreTouche()
{
	string lsInput;
	cout << endl << "Appuyez sur une Entree pour continuer...";
	getline(cin, lsInput);
}

Client* Affichage::getClientConnecte()
{
	return clientApp.getClient(CI_INDEX_CLIENT_CONNECTE);
}