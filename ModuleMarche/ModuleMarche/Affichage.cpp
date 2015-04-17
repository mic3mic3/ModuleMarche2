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

#include "ClientApp.h"

using namespace std; //Pour ne jamais avoir à écrire std:: puisque j'utilise beaucoup de fonctions de std dans ce fichier

const string Affichage::CS_EXIT_INPUT = "exit";
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
		getline(cin,choix);
		if (choix[0] == '1')
		{
			menuInscription();
		}
		else if(choix[0] == '2')
		{
			menuConnexion();
		}
	}while(choix[0] != '3');
}

//Vérification si le compte existe déjà
bool Affichage::validationCompteInscription(const string & nomCompte)
{
	fstream compteVerif(nomCompte + ".txt", ios::in);
	if (compteVerif.is_open())
	{
		cout << "Compte existant." << endl;
		return false;
	}
	compteVerif.close();
	return true;
}

////Vérifier si on peut transférer le string en float (chiffres et un point: «99999.99» ou «9» ou «.9»).
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
void Affichage::creationFichierCompte(const string & nomCompte, const string & nom, const string & prenom, const string & adresse, const string & solde, const string & forfait)
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
	string nomCompte;
	string nom;
	string prenom;
	string adresse;
	string solde;
	system("cls");

	cout << "Inscription" << endl;
	do
	{
		nomCompte = "";
		
		string ligne;
		cout << "Nom de compte (" << CS_EXIT_INPUT << " pour sortir): ";
		getline(cin,nomCompte);
		
		if (nomCompte == CS_EXIT_INPUT)
		{
			return;
		}
	}while(!validationCompteInscription(nomCompte));

	//Entrée de différentes informations
	cout << "Nom: ";
	getline(cin,nom);
	cout << "Prenom: ";
	getline(cin,prenom);
	cout << "Adresse: ";
	getline(cin,adresse);

	do
	{
		cout << "Solde (maximum de 7 chiffres avant le point et de 2 decimales): ";
		getline(cin,solde);
	}while(!validationFloat(solde));
	string forfait;
	do
	{
		cout << "Forfait (A = Acheteur, V = Vendeur, S = Superclient): ";
		getline(cin,forfait);
	}while(forfait != "A" && forfait != "V" && forfait != "S");

	//Puisque tout est valide, on crée le fichier avec les informations sur le compte (première ligne du fichier)
	creationFichierCompte(nomCompte,nom,prenom,adresse,solde,forfait);
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

	try
	{
		clientApp.connexion(nomCompte);
		menuSelection();
	}
	catch (ExceptionMarche loExceptionMarche)
	{
		loExceptionMarche.Gerer();
	}
}

//Affichage du menu de sélection de fonctions une fois connecté, puis on renvoie le choix -> il change selon le forfait du client
void Affichage::menuSelection()
{
	string choix = "1";
	Employe* emp;
	Vendeur* vnd;
	Acheteur* ach;
	Superclient* sup;
	if (sup = dynamic_cast<Superclient*>(clientApp.getClient()))
	{
		do
		{
			system("cls");
			if (choix != "1" && choix != "2" && choix != "3" && choix != "4")
			{
				cout << "Erreur dans le choix!" << endl;
			}
			cout << "Que voulez-vous faire?" << endl;
			cout << "1 - Voir mes articles" << endl;
			cout << "2 - Voir les articles achetables" << endl;
			cout << "3 - Vendre mes articles" << endl;
			cout << "4 - Changer mon forfait" << endl;
			cout << "5 - Deconnexion" << endl;
			getline(cin,choix);
			if (choix == "1")
			{
				clientApp.voirAchats();
			}
			else if (choix == "2")
			{
				clientApp.voirArticles();
			}
			else if (choix == "3")
			{
				clientApp.voirVenteArticles();
			}
			else if (choix == "4")
			{
				clientApp.voirForfaits();
			}
		}while(choix != "5");
	}
	else if (emp = dynamic_cast<Employe*>(clientApp.getClient()))
	{
		do
		{
			system("cls");
			if (choix != "1" && choix != "2")
			{
				cout << "Erreur dans le choix!" << endl;
			}
			cout << "Que voulez-vous faire?" << endl;
			cout << "1 - Voir mes articles" << endl;
			cout << "2 - Voir les articles achetables" << endl;
			cout << "3 - Deconnexion" << endl;
			getline(cin,choix);
			if (choix == "1")
			{
				clientApp.voirAchats();
			}
			else if (choix == "2")
			{
				clientApp.voirArticles();
			}
		}while(choix != "3");
	}
	else if (ach = dynamic_cast<Acheteur*>(clientApp.getClient()))
	{
		do
		{
			system("cls");
			if (choix != "1" && choix != "2" && choix != "3")
			{
				cout << "Erreur dans le choix!" << endl;
			}
			cout << "Que voulez-vous faire?" << endl;
			cout << "1 - Voir mes articles" << endl;
			cout << "2 - Voir les articles achetables" << endl;
			cout << "3 - Changer mon forfait" << endl;
			cout << "4 - Deconnexion" << endl;
			getline(cin,choix);
			if (choix == "1")
			{
				clientApp.voirAchats();
			}
			else if (choix == "2")
			{
				clientApp.voirArticles();
			}
			else if (choix == "3")
			{
				clientApp.voirForfaits();
			}
		}while(choix != "4");
	}
	else if (vnd = dynamic_cast<Vendeur*>(clientApp.getClient()))
	{
		do
		{
			system("cls");
			if (choix != "1" && choix != "2" && choix != "3")
			{
				cout << "Erreur dans le choix!" << endl;
			}
			cout << "Que voulez-vous faire?" << endl;
			cout << "1 - Voir mes articles" << endl;
			cout << "2 - Vendre mes articles" << endl;
			cout << "3 - Changer mon forfait" << endl;
			cout << "4 - Deconnexion" << endl;
			getline(cin,choix);
			if (choix == "1")
			{
				clientApp.voirAchats();
			}
			else if (choix == "2")
			{
				clientApp.voirVenteArticles();
			}
			else if (choix == "3")
			{
				clientApp.voirForfaits();
			}
		}while(choix != "4");
	}

	clientApp.deconnexion();
}

//Affichage du menu pour changer de forfait
char Affichage::menuForfaits()
{
	string choix;
	choix = "A";
	do
	{
		system("cls");
		if (choix != "A" && choix != "V" && choix != "S")
		{
			cout << "Erreur dans le choix!" << endl;
		}
		cout << "Quel forfait voulez-vous?" << endl;
		cout << "A - Acheteur" << endl;
		cout << "V - Vendeur" << endl;
		cout << "S - Superclient" << endl;
		cout << "Q - Quitter" << endl;
		getline(cin,choix);
		if (choix == "Q")
		{
			return 'Q';
		}
	}while(choix != "A" && choix != "V" && choix != "S");

	return choix[0];
}

//Affichage des achats faits par le client, on reçoit une référence de son vecteur d'achats
void Affichage::menuAchats(const vector<Article*> &achats)
{
	system("cls");
	cout << "Mes achats" << endl << endl;
	cout << setw(15) << left << "Article" << setw(12) << left << "Prix" << setw(25) << left << "Description" << setw(17) << left << "Etat" << setw(10) << left << "Date" << endl << endl;
	for (size_t cpt=0;cpt < achats.size();cpt++)
	{
		cout << achats[cpt];
	}
	if (achats.size() == 0)
	{
		cout << "Aucun article en votre possession" << endl;
	}
	string x;
	cout << endl << "Appuyez sur une touche pour revenir au menu";
	getline(cin,x);
}

//Affichage du marché aux puces avec du solde du client et les articles à vendre du marché, puis on retourne le choix de l'article ou -1 si l'utilisateur veut sortir (a écrit exit)
int Affichage::menuMarche(float solde,const vector<Article*> &listeArticles,char categorie)
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
		int cpt2=0;
		//On met une précision aux variables float avec fixed et setprecision(), puis on aligne bien les colonnes avec setw() et left
		cout << "Marche Aux Puces (Mode Achat)" << "\tVotre solde: " << fixed << setprecision(2) << solde << endl << endl;
		cout << setw(4) << left << "    " << setw(13) << left << "Article" << setw(10) << left << "Prix" << setw(25) << left << "Description" << setw(17) << left << "Etat" << setw(10) << left << "Date" << endl << endl;
		for (size_t cpt=0;cpt < listeArticles.size();cpt++)
		{
			switch(categorie)
			{
				case 'T':
					cout << setw(4) << left << (cpt2+1)<< listeArticles[cpt];
					cpt2++;
					break;
				case 'B':
					Bijou* bij;
					if (bij = dynamic_cast<Bijou*>(listeArticles[cpt]))
					{
						cout << setw(4) << left << (cpt2+1)<< listeArticles[cpt];
						cpt2++;
					}
					break;
				case 'V':
					Voiture* voi;
					if (voi = dynamic_cast<Voiture*>(listeArticles[cpt]))
					{
						cout << setw(4) << left << (cpt2+1)<< listeArticles[cpt];
						cpt2++;
					}
					break;
				case 'D':
					Divers* div;
					if (div = dynamic_cast<Divers*>(listeArticles[cpt]))
					{
						cout << setw(4) << left << (cpt2+1)<< listeArticles[cpt];
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
		getline(cin,choix);

		if (choix == CS_EXIT_INPUT)
		{
			return -1;
		}
		retour = validationChoixArticle(choix, cpt2);
	}while(retour == 0);

	retour = transformationEnPositionTableau(retour,categorie,listeArticles);
	listeArticles[retour - 1]->afficherDetails();

	return retour;
}

int Affichage::transformationEnPositionTableau(int retour,char categorie,const vector<Article*> & listeArticles)
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
			break;
		}
	}

	return -1;
}

//Affichage de la vérification de l'achat selon si le solde était assez grand pour l'achat de l'objet choisi, puis on renvoit si l'utilisateur valide l'achat (true) ou non (false)
bool Affichage::menuVerifAchat(bool soldeOk)
{
	string choixStr = "O";
	if (soldeOk)
	{
		do
		{
			if (choixStr != "O" && choixStr != "N")
			{
				cout << "Erreur! Veuillez entrer O ou N" << endl;
			}
			cout << "L'acheteur a assez d'argent pour l'article." << endl;

			cout << "Etes-vous sur de vouloir proceder? (O/N)" << endl;
			getline(cin,choixStr);
		}while (choixStr != "O" && choixStr != "N");
	}
	else
	{
		cout << "L'acheteur n'a pas assez d'argent pour l'article." << endl; //
		getline(cin,choixStr);
		return false;
	}
	if (choixStr == "O")
	{
		return true;
	}
	return false;
}

//Pour choisir la façon dont on trie les articles à vendre
char Affichage::menuCategories()
{
	string choix;
	choix = "V";
	do
	{
		system("cls");
		if (choix != "V" && choix != "B" && choix != "D" && choix != "T")
		{
			cout << "Erreur dans le choix!" << endl;
		}
		cout << "Par quel type voulez-vous chercher vos produits?" << endl;
		cout << "V - Voitures" << endl;
		cout << "B - Bijoux" << endl;
		cout << "D - Divers" << endl;
		cout << "T - Tout" << endl;
		cout << "Q - Quitter" << endl;
		getline(cin,choix);
		if (choix == "Q")
		{
			return 'Q';
		}
	}while(choix != "V" && choix != "B" && choix != "D" && choix != "T");

	return choix[0];
}

//Validation choix d'un article (vente ou achat)
int Affichage::validationChoixArticle(const string & choix,int max)
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

//Le menu pour la vente d'articles
int Affichage::menuVenteArticles(float solde,const vector<Article*> &listeArticles)
{
	int retour;
	system("cls");
	do
	{
		//On met une précision aux variables float avec fixed et setprecision(), puis on aligne bien les colonnes avec setw() et left
		cout << "Marche Aux Puces (Mode Vente)" << "\tSon solde: " << fixed << setprecision(2) << solde << endl << endl;
		cout << setw(4) << left << "    " << setw(13) << left << "Article" << setw(10) << left << "Prix" << setw(25) << left << "Description" << setw(17) << left << "Etat" << setw(10) << left << "Date" << endl << endl;
		for (size_t cpt=0;cpt < listeArticles.size();cpt++)
		{
			cout << setw(4) << left << (cpt+1)<< listeArticles[cpt];
		}
		if (listeArticles.size() == 0)
		{
			cout << "Aucun article a vendre" << endl;
		}
		string choix;
		cout << endl << "Choisir l'article selon son numero (" << CS_EXIT_INPUT << " pour sortir):" << endl;
		getline(cin,choix);

		if (choix == CS_EXIT_INPUT)
		{
			return -1;
		}
		
		retour = validationChoixArticle(choix, listeArticles.size());
	} while (retour == 0);

	listeArticles[retour - 1]->afficherDetails();

	return retour;
}

int main()
{
	Affichage::menuDemarrer();

	cout << "Fermeture de l'application..." << endl;
	
	return EXIT_SUCCESS;
}