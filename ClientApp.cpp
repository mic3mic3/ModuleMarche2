#include "ClientApp.h"

#include <fstream> //Utilisé pour opérations dans les fichiers
#include <sstream> //Utilisé pour la transformation int en string (exemple ligne 376)
#include <vector>

#include "Date.h"
#include "Affichage.h"
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

using namespace std; //Pour ne jamais avoir à écrire std:: puisque j'utilise beaucoup de fonctions de std dans ce fichier

ClientApp::ClientApp(void)
{
}

ClientApp::ClientApp(Affichage* aff)
{
	affichage = aff;
	client = NULL;
	while (true) //À chaque fois qu'une opération est complétée, on revient à un des deux menus
	{
		if (client == NULL) //Si aucun client n'est connecté, on envoit au premier menu, sinon on envoie au deuxième
			demarrer();
		else
			selection();
	}
}

ClientApp::~ClientApp(void)
{
}

//On va appeler le premier menu
void ClientApp::demarrer()
{
	char choix = affichage->menuDemarrer();
	//On envoie l'utilisateur à l'un des deux menus selon son input
	if (choix == '1')
	{
		inscription();
	}
	else
	{
		connexion();
	}
}

//On appelle le menu d'inscription
void ClientApp::inscription()
{
	affichage->menuInscription();
}

//On appelle le menu de connexion, puis on crée le client et le marché si c'est un succès
void ClientApp::connexion()
{
	string retour = affichage->menuConnexion();
	if (retour == "exit")
	{
		return;
	}
	else
	{
		
		compte = retour;
		creationMarche("Centre-ville");
		creationClient(retour); //Le nom du marché aux puces, et donc du fichier ***ATTENTION, le programme se fermera automatiquement si le fichier Centre-ville.txt n'existe pas
	}
}

//Selon le nom de compte, on va chercher les informations du client (nom, prenom, adresse, solde du compte, achats) dans un fichier
void ClientApp::creationClient(const string &nomCompte)
{
	//On recherche les informations du client sur la première ligne du fichier
	fstream compte2(nomCompte+".txt",ios::in);
	string ligne;
	getline(compte2,ligne);
	int nbPtsVirgs = 0;
	string nom;
	string prenom;
	string adresse;
	string soldeStr;
	char forfait;
	string salaireStr;
	string rabaisStr;
	float salaire;
	float rabais;
	float solde;
	for (size_t cpt=0;cpt < ligne.length();cpt++)
	{
		if (ligne[cpt]==';')
		{
			nbPtsVirgs++;
		}
		else
		{
			switch(nbPtsVirgs)
			{
				case 1:
					nom+=ligne[cpt];
					break;
				case 2:
					prenom+=ligne[cpt];
					break;
				case 3:
					adresse+=ligne[cpt];
					break;
				case 4:
					soldeStr+=ligne[cpt];
					break;
				case 5:
					forfait=ligne[cpt];
					break;
				case 6:
					salaireStr+=ligne[cpt];
					break;
				case 7:
					rabaisStr+=ligne[cpt];
					break;
				default:
					break;

			}
		}
	}
	solde = stof(soldeStr.c_str()); //Transfert des string en float
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
				bool found= false;
				for (size_t cpt = 0;cpt < comptesEmployes.size(); cpt++)
				{
					if (compte == comptesEmployes[cpt])
					{
						client = marcheAuxPuces->getPersonnel()[cpt];
						found = true;
					}
				}
				if (!found)
				{
					salaire = stof(salaireStr.c_str());
					rabais = stof(rabaisStr.c_str());
					client = new Employe(nom,prenom,adresse,new Compte(solde),salaire,rabais);
				}
				
				break;
		}


	//On récupère les achats du client après la première ligne du fichier
	if (forfait != 'E')
	{
	if (compte2.is_open())
	{
		
	string ligneAchats;
	while (getline(compte2,ligneAchats) && ligneAchats.length() != NULL)
	{
		string nomArticle;
		string prixStr;
		float prix;
		string description;
		string etat;
		string dateFabricationStr;
		struct Date dateFabrication;
		nbPtsVirgs = 0;
		char type;
		string attribut1Str;
		string attribut3Str;
		int attribut1;
		string attribut2;
		int attribut3;
		for (size_t cpt=0;cpt < ligneAchats.length();cpt++)
		{
			if (ligneAchats[cpt]==';')
			{
				nbPtsVirgs++;
			}
			else
			{
				switch(nbPtsVirgs)
				{
					case 0:
						nomArticle+=ligneAchats[cpt];
						break;
					case 1:
						type = ligneAchats[cpt];
						break;
					case 2:
						prixStr+=ligneAchats[cpt];
						break;
					case 3:
						description+=ligneAchats[cpt];
						break;
					case 4:
						etat+=ligneAchats[cpt];
						break;
					case 5:
						dateFabricationStr+=ligneAchats[cpt];
						break;
					case 6:
						attribut1Str+=ligneAchats[cpt];
						break;
					case 7:
						attribut2+=ligneAchats[cpt];
						break;
					case 8:
						attribut3Str+=ligneAchats[cpt];
						break;
					default:
						break;

				}
			}
		}
		prix = stof(prixStr.c_str());
		string jourStr = "";
		string moisStr = "";
		string anneeStr = "";
		int nbSlashs=0;
		for (size_t cpt=0;cpt < dateFabricationStr.length();cpt++)
		{
			if (dateFabricationStr[cpt]=='/')
			{
				nbSlashs++;
			}
			else
			{
				switch(nbSlashs)
				{
					case 0:
						jourStr+=dateFabricationStr[cpt];
						break;
					case 1:
						moisStr+=dateFabricationStr[cpt];
						break;
					case 2:
						anneeStr+=dateFabricationStr[cpt];
						break;
					default:
						break;

				}
			}
		}
		dateFabrication.jour = atoi(jourStr.c_str());
		dateFabrication.mois = atoi(moisStr.c_str());
		dateFabrication.annee = atoi(anneeStr.c_str());
		switch (type)
		{
			case 'D':
				client->ajouterArticle(new Divers(nomArticle,prix,description,etat,dateFabrication));
				break;
			case 'V':
				attribut1 = atoi(attribut1Str.c_str());
				attribut3 = atoi(attribut3Str.c_str());
				client->ajouterArticle(new Voiture(nomArticle,prix,description,etat,dateFabrication,attribut1,attribut2,attribut3));
				break;
			case 'B':
				attribut1 = atoi(attribut1Str.c_str());
				client->ajouterArticle(new Bijou(nomArticle,prix,description,etat,dateFabrication,attribut2,attribut1));
				break;
		}
	}
	}
	}
	compte2.close();
}

//On crée le marché à partir d'un fichier
void ClientApp::creationMarche(const string &nom)
{
	//La première ligne contient les informations de base (nom, adresse, revenu)
	fstream marche(nom+".txt",ios::in);
	if (marche.is_open())
	{
		string ligne;
		getline(marche,ligne);
		int nbPtsVirgs = 0;
		string adresse;
		string revenuStr;
		float revenu;
		for (size_t cpt=0;cpt < ligne.length();cpt++)
		{
			if (ligne[cpt]==';')
			{
				nbPtsVirgs++;
			}
			else
			{
				switch(nbPtsVirgs)
				{
					case 1:
						adresse+=ligne[cpt];
						break;
					case 2:
						revenuStr+=ligne[cpt];
						break;
					default:
						break;

				}
			}
		}
		revenu = stof(revenuStr.c_str()); //
		marcheAuxPuces = new MarcheAuxPuces(nom,adresse,new Compte(revenu)); //Création du marché

		//On recherche les articles de ce marché dans le reste du fichier
		while (getline(marche,ligne) && ligne.length() != NULL)
		{
			//Processus pour mettre les parties du fichier (séparées par point-virgule) dans les bonnes variables
			string nomArticle;
			string prixStr;
			float prix;
			string description;
			string etat;
			string dateFabricationStr;
			struct Date dateFabrication;
			nbPtsVirgs = 0;
			char type;
			string attribut1Str;
			string attribut2;
			string attribut3Str;
			int attribut1;
			int attribut3;
			for (size_t cpt=0;cpt < ligne.length();cpt++)
			{
				if (ligne[cpt]==';')
				{
					nbPtsVirgs++;
				}
				else
				{
					switch(nbPtsVirgs)
					{
						case 0:
						nomArticle+=ligne[cpt];
						break;
					case 1:
						type = ligne[cpt];
						break;
					case 2:
						prixStr+=ligne[cpt];
						break;
					case 3:
						description+=ligne[cpt];
						break;
					case 4:
						etat+=ligne[cpt];
						break;
					case 5:
						dateFabricationStr+=ligne[cpt];
						break;
					case 6:
						attribut1Str+=ligne[cpt];
						break;
					case 7:
						attribut2+=ligne[cpt];
						break;
					case 8:
						attribut3Str+=ligne[cpt];
						break;
					default:
						break;

					}
				}
			}
			prix = stof(prixStr.c_str());

			//Processus pour transformer la date du fichier en struct Date
			string jourStr = "";
			string moisStr = "";
			string anneeStr = "";
			int nbSlashs=0;
			for (size_t cpt=0;cpt < dateFabricationStr.length();cpt++)
			{
				if (dateFabricationStr[cpt]=='/')
				{
					nbSlashs++;
				}
				else
				{
					switch(nbSlashs)
					{
						case 0:
							jourStr+=dateFabricationStr[cpt];
							break;
						case 1:
							moisStr+=dateFabricationStr[cpt];
							break;
						case 2:
							anneeStr+=dateFabricationStr[cpt];
							break;
						default:
							break;

					}
				}
			}
			dateFabrication.jour = atoi(jourStr.c_str());
			dateFabrication.mois = atoi(moisStr.c_str());
			dateFabrication.annee = atoi(anneeStr.c_str());
			switch (type)
			{
				case 'D':
					marcheAuxPuces->ajouterArticle(new Divers(nomArticle,prix,description,etat,dateFabrication));
					break;
				case 'V':
					attribut1 = atoi(attribut1Str.c_str());
					attribut3 = atoi(attribut3Str.c_str());
					marcheAuxPuces->ajouterArticle(new Voiture(nomArticle,prix,description,etat,dateFabrication,attribut1,attribut2,attribut3));
					break;
				case 'B':
					attribut1 = atoi(attribut1Str.c_str());
					marcheAuxPuces->ajouterArticle(new Bijou(nomArticle,prix,description,etat,dateFabrication,attribut2,attribut1));
					break;
			}
		}
		//Maintenant, on ajoute le personnel à la liste d'employés du MarcheAuxPuces
		fstream personnel(nom+"_Employes.txt",ios::in);
		if (personnel.is_open())
		{
			string employe;
			while (getline(personnel,employe) && employe.length() != NULL)
			{
				fstream infos(employe+".txt",ios::in);
				if (infos.is_open())
				{
					comptesEmployes.push_back(employe);
					string ligne;
					getline(infos,ligne);
					int nbPtsVirgs = 0;
					string nomP;
					string prenomP;
					string adresseP;
					string soldeStrP;
					char forfait;
					string salaireStr;
					string rabaisStr;
					float salaire;
					float rabais;
					float soldeP;
					for (size_t cpt=0;cpt < ligne.length();cpt++)
					{
						if (ligne[cpt]==';')
						{
							nbPtsVirgs++;
						}
						else
						{
							switch(nbPtsVirgs)
							{
								case 1:
									nomP+=ligne[cpt];
									break;
								case 2:
									prenomP+=ligne[cpt];
									break;
								case 3:
									adresseP+=ligne[cpt];
									break;
								case 4:
									soldeStrP+=ligne[cpt];
									break;
								case 5:
									forfait=ligne[cpt];
									break;
								case 6:
									salaireStr+=ligne[cpt];
									break;
								case 7:
									rabaisStr+=ligne[cpt];
									break;
								default:
									break;

							}
						}
					}
					soldeP = stof(soldeStrP.c_str()); //Transfert des string en float

					salaire = stof(salaireStr.c_str());
					rabais = stof(rabaisStr.c_str());
					Employe* unEmploye = new Employe(nomP,prenomP,adresseP,new Compte(soldeP),salaire,rabais);


					//On récupère les achats du client après la première ligne du fichier
		
					string ligneAchats;
					while (getline(infos,ligneAchats) && ligneAchats.length() != NULL)
					{
						string nomArticle;
						string prixStr;
						float prix;
						string description;
						string etat;
						string dateFabricationStr;
						struct Date dateFabrication;
						nbPtsVirgs = 0;
						char type;
						string attribut1Str;
						string attribut3Str;
						int attribut1;
						string attribut2;
						int attribut3;
						for (size_t cpt=0;cpt < ligneAchats.length();cpt++)
						{
							if (ligneAchats[cpt]==';')
							{
								nbPtsVirgs++;
							}
							else
							{
								switch(nbPtsVirgs)
								{
									case 0:
										nomArticle+=ligneAchats[cpt];
										break;
									case 1:
										type = ligneAchats[cpt];
										break;
									case 2:
										prixStr+=ligneAchats[cpt];
										break;
									case 3:
										description+=ligneAchats[cpt];
										break;
									case 4:
										etat+=ligneAchats[cpt];
										break;
									case 5:
										dateFabricationStr+=ligneAchats[cpt];
										break;
									case 6:
										attribut1Str+=ligneAchats[cpt];
										break;
									case 7:
										attribut2+=ligneAchats[cpt];
										break;
									case 8:
										attribut3Str+=ligneAchats[cpt];
										break;
									default:
										break;

								}
							}
						}
						prix = stof(prixStr.c_str());
						string jourStr = "";
						string moisStr = "";
						string anneeStr = "";
						int nbSlashs=0;
						for (size_t cpt=0;cpt < dateFabricationStr.length();cpt++)
						{
							if (dateFabricationStr[cpt]=='/')
							{
								nbSlashs++;
							}
							else
							{
								switch(nbSlashs)
								{
									case 0:
										jourStr+=dateFabricationStr[cpt];
										break;
									case 1:
										moisStr+=dateFabricationStr[cpt];
										break;
									case 2:
										anneeStr+=dateFabricationStr[cpt];
										break;
									default:
										break;

								}
							}
						}
						dateFabrication.jour = atoi(jourStr.c_str());
						dateFabrication.mois = atoi(moisStr.c_str());
						dateFabrication.annee = atoi(anneeStr.c_str());
						switch (type)
						{
							case 'D':
								unEmploye->ajouterArticle(new Divers(nomArticle,prix,description,etat,dateFabrication));
								break;
							case 'V':
								attribut1 = atoi(attribut1Str.c_str());
								attribut3 = atoi(attribut3Str.c_str());
								unEmploye->ajouterArticle(new Voiture(nomArticle,prix,description,etat,dateFabrication,attribut1,attribut2,attribut3));
								break;
							case 'B':
								attribut1 = atoi(attribut1Str.c_str());
								unEmploye->ajouterArticle(new Bijou(nomArticle,prix,description,etat,dateFabrication,attribut2,attribut1));
								break;
						}

					}
					marcheAuxPuces->ajouterEmploye(unEmploye);
					infos.close();
				}
				else
				{
					exit(0);
				}

			}
		}
		else
		{
			exit(0);
		}
	}
	else
	{
		exit(0);
	}
	marche.close();
}

//Une fois connecté, on peut voir le menu
void ClientApp::selection()
{
	char choix = affichage->menuSelection(client);
	Employe* emp;
	Vendeur* vnd;
	Acheteur* ach;
	Superclient* sup;
	if (sup = dynamic_cast<Superclient*>(client))
	{
		if (choix == '1')
		{
			voirAchats();
		}
		else if (choix == '2')
		{
			voirArticles();
		}
		else if (choix == '3')
		{
			voirVenteArticles();
		}
		else if (choix == '4')
		{
			voirForfaits();
		}
		else //On se déconnecte et supprime le client et le marché
		{
			delete client;
			client = NULL;
			delete marcheAuxPuces;
			marcheAuxPuces = NULL;
		}
	}
	else if (emp = dynamic_cast<Employe*>(client))
	{
		if (choix == '1')
		{
			voirAchats();
		}
		else if (choix == '2')
		{
			voirArticles();
		}
		else //On se déconnecte et supprime le client et le marché
		{
			delete client;
			client = NULL;
			delete marcheAuxPuces;
			marcheAuxPuces = NULL;
		}
	}
	else if (ach = dynamic_cast<Acheteur*>(client))
	{
		if (choix == '1')
		{
			voirAchats();
		}
		else if (choix == '2')
		{
			voirArticles();
		}
		else if (choix == '3')
		{
			voirForfaits();
		}
		else //On se déconnecte et supprime le client et le marché
		{
			delete client;
			client = NULL;
			delete marcheAuxPuces;
			marcheAuxPuces = NULL;
		}
	}
	else if (vnd = dynamic_cast<Vendeur*>(client))
	{
		if (choix == '1')
		{
			voirAchats();
		}
		else if (choix == '2')
		{
			voirVenteArticles();
		}
		else if (choix == '3')
		{
			voirForfaits();
		}
		else //On se déconnecte et supprime le client et le marché
		{
			delete client;
			client = NULL;
			delete marcheAuxPuces;
			marcheAuxPuces = NULL;
		}
	}
	//On renvoit vers la fonction choisie par l'utilisateur (voir les achats ou voir les articles disponibles au marché)
	
}

//Le client voit les forfaits
void ClientApp::voirForfaits()
{
	char newForfait = affichage->menuForfaits();
	switch(newForfait)
	{
		case 'A':
			client = new Acheteur(client);
			break;
		case 'V':
			client = new Vendeur(client);
			break;
		case 'S':
			client = new Superclient(client);
			break;
		default:
			break;
	}
	//On update le fichier après avoir changer le forfait
			fstream achats(compte+".txt");
			if (achats)
			{
				achats << compte << ";" << client->getNom() << ";" << client->getPrenom() << ";" << client->getAdresse() << ";" << client->getSolde();
				Employe* emp;
				Superclient* sup;
				Vendeur* vnd;
				Acheteur* ach;
				if (sup = dynamic_cast<Superclient*>(client))
				{
					achats << ";S\n";	
				}
				else if (emp = dynamic_cast<Employe*>(client))
				{
					achats << ";E;" << emp->getSalaire() << ";" << emp->getRabais() << "\n";	
				}
				else if (ach = dynamic_cast<Acheteur*>(client))
				{
					achats << ";A\n";	
				}
				else if (vnd = dynamic_cast<Vendeur*>(client))
				{
					achats << ";V\n";	
				}
				for (size_t cpt=0;cpt < client->getArticles().size();cpt++)
				{
					ostringstream conversion; //Conversion avec sstream d'un int en string
					conversion << client->getArticles()[cpt]->getDate().jour << "/" << client->getArticles()[cpt]->getDate().mois << "/" << client->getArticles()[cpt]->getDate().annee;
					string date = conversion.str();
					Divers* div;
					Bijou* bij;
					Voiture* voit;
					if (div = dynamic_cast<Divers*>(client->getArticles()[cpt]))
					{
						achats << client->getArticles()[cpt]->getNom() << ";D;"<<client->getArticles()[cpt]->getPrix()<<";"<<client->getArticles()[cpt]->getDescription()<<";"<<client->getArticles()[cpt]->getEtat()<<";"<<date<<"\n";
					}
					else if(bij = dynamic_cast<Bijou*>(client->getArticles()[cpt]))
					{
						achats << client->getArticles()[cpt]->getNom() << ";B;"<<client->getArticles()[cpt]->getPrix()<<";"<<client->getArticles()[cpt]->getDescription()<<";"<<client->getArticles()[cpt]->getEtat()<<";"<<date<<";"<<bij->getPurete()<<";"<<bij->getMateriau()<<"\n";
					}
					else if(voit = dynamic_cast<Voiture*>(client->getArticles()[cpt]))
					{
						achats << client->getArticles()[cpt]->getNom() << ";V;"<<client->getArticles()[cpt]->getPrix()<<";"<<client->getArticles()[cpt]->getDescription()<<";"<<client->getArticles()[cpt]->getEtat()<<";"<<date<<";"<<voit->getKilometrage()<<";"<<voit->getCouleur()<<";"<<voit->getAnnee()<<"\n";
					}
					
				}
			}
}

//Le client voit les achats qu'il a fait
void ClientApp::voirAchats()
{
	affichage->menuAchats(client->getArticles());
}

//On accède à la vente d'articles et on passe les étapes de validation, puis on update les fichiers requis à la fin
void ClientApp::voirVenteArticles()
{
	int retour = affichage->menuVenteArticles(marcheAuxPuces->getRevenu(),client->getArticles());
	bool prixValide;
	bool vendu;
	if (retour != -1)
	{
		prixValide = marcheAuxPuces->validerCompte(client->getArticles()[retour-1]->getPrix());
		vendu = affichage->menuVerifAchat(prixValide); //On renvoit un message selon la possibilité de l'achat de l'article après la vérification du solde du client
		if (vendu) //Si le client achète l'article
		{
			marcheAuxPuces->acheter(client->getArticles()[retour-1]); //On crée la transaction pour le marché aux puces
			Vendeur* vnd;
			if (vnd = dynamic_cast<Vendeur*>(client))
			{
				vnd->ajouterTransaction(retour-1,marcheAuxPuces,client->getArticles()[retour-1]); //On appelle la fonction acheter de client
			}
			
			//On ajoute l'achat au fichier du client
			fstream achats(compte+".txt",std::ofstream::out | std::ofstream::trunc);
			if (achats)
			{
				achats << compte << ";" << client->getNom() << ";" << client->getPrenom() << ";" << client->getAdresse() << ";" << client->getSolde();
				Employe* emp;
				Superclient* sup;
				Vendeur* vnd;
				Acheteur* ach;
				if (sup = dynamic_cast<Superclient*>(client))
				{
					achats << ";S\n";	
				}
				else if (emp = dynamic_cast<Employe*>(client))
				{
					achats << ";E;" << emp->getSalaire() << ";" << emp->getRabais() << "\n";	
				}
				else if (ach = dynamic_cast<Acheteur*>(client))
				{
					achats << ";A\n";	
				}
				else if (vnd = dynamic_cast<Vendeur*>(client))
				{
					achats << ";V\n";	
				}
				
				for (size_t cpt=0;cpt < client->getArticles().size();cpt++)
				{
					ostringstream conversion; //Conversion avec sstream d'un int en string
					conversion << client->getArticles()[cpt]->getDate().jour << "/" << client->getArticles()[cpt]->getDate().mois << "/" << client->getArticles()[cpt]->getDate().annee;
					string date = conversion.str();
					Divers* div;
					Bijou* bij;
					Voiture* voit;
					if (div = dynamic_cast<Divers*>(client->getArticles()[cpt]))
					{
						achats << client->getArticles()[cpt]->getNom() << ";D;"<<client->getArticles()[cpt]->getPrix()<<";"<<client->getArticles()[cpt]->getDescription()<<";"<<client->getArticles()[cpt]->getEtat()<<";"<<date<<"\n";
					}
					else if(bij = dynamic_cast<Bijou*>(client->getArticles()[cpt]))
					{
						achats << client->getArticles()[cpt]->getNom() << ";B;"<<client->getArticles()[cpt]->getPrix()<<";"<<client->getArticles()[cpt]->getDescription()<<";"<<client->getArticles()[cpt]->getEtat()<<";"<<date<<";"<<bij->getPurete()<<";"<<bij->getMateriau()<<"\n";
					}
					else if(voit = dynamic_cast<Voiture*>(client->getArticles()[cpt]))
					{
						achats << client->getArticles()[cpt]->getNom() << ";V;"<<client->getArticles()[cpt]->getPrix()<<";"<<client->getArticles()[cpt]->getDescription()<<";"<<client->getArticles()[cpt]->getEtat()<<";"<<date<<";"<<voit->getKilometrage()<<";"<<voit->getCouleur()<<";"<<voit->getAnnee()<<"\n";
					}
					
				}
			}

			achats.close();

			//On append la transaction dans le fichier de transaction du marché aux puces
			fstream trans(compte+"_Trans.txt",ios::app);
			if (trans)
			{
				Vendeur* vnd;
				if (vnd = dynamic_cast<Vendeur*>(client))
				{
					trans << "Client: " << vnd->getDerniereTransaction().client->getNom() << ";" << vnd->getDerniereTransaction().client->getPrenom() << ";" << vnd->getDerniereTransaction().client->getAdresse() << "\n";
					trans << "Marche: " << vnd->getDerniereTransaction().marche->getNom() << ";" << vnd->getDerniereTransaction().marche->getAdresse() << "\n";
					trans << "Article: " << vnd->getDerniereTransaction().article->getNom() << ";" << vnd->getDerniereTransaction().article->getPrix() << ";" << vnd->getDerniereTransaction().article->getDescription() << ";" << vnd->getDerniereTransaction().article->getEtat() << ";" << vnd->getDerniereTransaction().article->getDate().jour << "/" << vnd->getDerniereTransaction().article->getDate().mois << "/" << vnd->getDerniereTransaction().article->getDate().annee << "\n";
					trans << "Date: " << vnd->getDerniereTransaction().date.jour << "/" << vnd->getDerniereTransaction().date.mois << "/" << vnd->getDerniereTransaction().date.annee << "\n";
					trans << "-----\n";
				}
			}

			trans.close();

			//On enlève l'article qui vient d'être vendu au fichier du marché aux puces
			ofstream marche(marcheAuxPuces->getNom()+".txt");
			if (marche)
			{
				marche << marcheAuxPuces->getNom() << ";" << marcheAuxPuces->getAdresse() << ";" << marcheAuxPuces->getRevenu() << "\n";
				for (size_t cpt=0;cpt < marcheAuxPuces->getArticlesEnVente().size();cpt++)
				{
					ostringstream conversion;
					conversion << marcheAuxPuces->getArticlesEnVente()[cpt]->getDate().jour << "/" << marcheAuxPuces->getArticlesEnVente()[cpt]->getDate().mois << "/" << marcheAuxPuces->getArticlesEnVente()[cpt]->getDate().annee;
					string date = conversion.str();
					Divers* div;
					Bijou* bij;
					Voiture* voit;
					if (div = dynamic_cast<Divers*>(marcheAuxPuces->getArticlesEnVente()[cpt]))
					{
						marche << marcheAuxPuces->getArticlesEnVente()[cpt]->getNom() << ";D;"<<marcheAuxPuces->getArticlesEnVente()[cpt]->getPrix()<<";"<<marcheAuxPuces->getArticlesEnVente()[cpt]->getDescription()<<";"<<marcheAuxPuces->getArticlesEnVente()[cpt]->getEtat()<<";"<<date<<"\n";
					}
					else if(bij = dynamic_cast<Bijou*>(marcheAuxPuces->getArticlesEnVente()[cpt]))
					{
						marche << marcheAuxPuces->getArticlesEnVente()[cpt]->getNom() << ";B;"<<marcheAuxPuces->getArticlesEnVente()[cpt]->getPrix()<<";"<<marcheAuxPuces->getArticlesEnVente()[cpt]->getDescription()<<";"<<marcheAuxPuces->getArticlesEnVente()[cpt]->getEtat()<<";"<<date<<";"<<bij->getPurete()<<";"<<bij->getMateriau()<<"\n";
					}
					else if(voit = dynamic_cast<Voiture*>(marcheAuxPuces->getArticlesEnVente()[cpt]))
					{
						marche << marcheAuxPuces->getArticlesEnVente()[cpt]->getNom() << ";V;"<<marcheAuxPuces->getArticlesEnVente()[cpt]->getPrix()<<";"<<marcheAuxPuces->getArticlesEnVente()[cpt]->getDescription()<<";"<<marcheAuxPuces->getArticlesEnVente()[cpt]->getEtat()<<";"<<date<<";"<<voit->getKilometrage()<<";"<<voit->getCouleur()<<";"<<voit->getAnnee()<<"\n";
					}
				}
			}

			marche.close();
			
		}
	}
}

//On affiche à l'utilisateur les articles disponibles et s'il en choisit un, on demande une validation après la vérification du solde, puis s'il l'achète, on fait la transaction
void ClientApp::voirArticles()
{
	char categorie = affichage->menuCategories();
	if (categorie != 'Q')
	{
		int retour = affichage->menuMarche(client->getSolde(),marcheAuxPuces->getArticlesEnVente(),categorie);
		bool prixValide;
		bool vendu;
		if (retour != -1)
		{
			prixValide = client->validerCompte(marcheAuxPuces->getArticlesEnVente()[retour-1]->getPrix());
			vendu = affichage->menuVerifAchat(prixValide); //On renvoit un message selon la possibilité de l'achat de l'article après la vérification du solde du client
			if (vendu) //Si le client achète l'article
			{
				Acheteur* acht;
				if (acht = dynamic_cast<Acheteur*>(client))
				{
					acht->acheter(marcheAuxPuces->getArticlesEnVente()[retour-1]); //On appelle la fonction acheter de client
				}
			
				//On ajoute l'achat au fichier du client
				fstream achats(compte+".txt");
				if (achats)
				{
					achats << compte << ";" << client->getNom() << ";" << client->getPrenom() << ";" << client->getAdresse() << ";" << client->getSolde();
					Employe* emp;
					Superclient* sup;
					Vendeur* vnd;
					Acheteur* ach;
					if (sup = dynamic_cast<Superclient*>(client))
					{
						achats << ";S\n";	
					}
					else if (emp = dynamic_cast<Employe*>(client))
					{
						achats << ";E;" << emp->getSalaire() << ";" << emp->getRabais() << "\n";	
					}
					else if (ach = dynamic_cast<Acheteur*>(client))
					{
						achats << ";A\n";	
					}
					else if (vnd = dynamic_cast<Vendeur*>(client))
					{
						achats << ";V\n";	
					}
				
					for (size_t cpt=0;cpt < client->getArticles().size();cpt++)
					{
						ostringstream conversion; //Conversion avec sstream d'un int en string
						conversion << client->getArticles()[cpt]->getDate().jour << "/" << client->getArticles()[cpt]->getDate().mois << "/" << client->getArticles()[cpt]->getDate().annee;
						string date = conversion.str();
						Divers* div;
						Bijou* bij;
						Voiture* voit;
						if (div = dynamic_cast<Divers*>(client->getArticles()[cpt]))
						{
							achats << client->getArticles()[cpt]->getNom() << ";D;"<<client->getArticles()[cpt]->getPrix()<<";"<<client->getArticles()[cpt]->getDescription()<<";"<<client->getArticles()[cpt]->getEtat()<<";"<<date<<"\n";
						}
						else if(bij = dynamic_cast<Bijou*>(client->getArticles()[cpt]))
						{
							achats << client->getArticles()[cpt]->getNom() << ";B;"<<client->getArticles()[cpt]->getPrix()<<";"<<client->getArticles()[cpt]->getDescription()<<";"<<client->getArticles()[cpt]->getEtat()<<";"<<date<<";"<<bij->getPurete()<<";"<<bij->getMateriau()<<"\n";
						}
						else if(voit = dynamic_cast<Voiture*>(client->getArticles()[cpt]))
						{
							achats << client->getArticles()[cpt]->getNom() << ";V;"<<client->getArticles()[cpt]->getPrix()<<";"<<client->getArticles()[cpt]->getDescription()<<";"<<client->getArticles()[cpt]->getEtat()<<";"<<date<<";"<<voit->getKilometrage()<<";"<<voit->getCouleur()<<";"<<voit->getAnnee()<<"\n";
						}
					
					}
				}

				achats.close();
				marcheAuxPuces->ajouterTransaction(retour-1,client,marcheAuxPuces->getArticlesEnVente()[retour-1]); //On crée la transaction pour le marché aux puces

				//On append la transaction dans le fichier de transaction du marché aux puces
				fstream trans(marcheAuxPuces->getNom()+"_Trans.txt",ios::app);
				if (trans)
				{
					trans << "Client: " << marcheAuxPuces->getDerniereTransaction().client->getNom() << ";" << marcheAuxPuces->getDerniereTransaction().client->getPrenom() << ";" << marcheAuxPuces->getDerniereTransaction().client->getAdresse() << "\n";
					trans << "Marche: " << marcheAuxPuces->getDerniereTransaction().marche->getNom() << ";" << marcheAuxPuces->getDerniereTransaction().marche->getAdresse() << "\n";
					trans << "Article: " << marcheAuxPuces->getDerniereTransaction().article->getNom() << ";" << marcheAuxPuces->getDerniereTransaction().article->getPrix() << ";" << marcheAuxPuces->getDerniereTransaction().article->getDescription() << ";" << marcheAuxPuces->getDerniereTransaction().article->getEtat() << ";" << marcheAuxPuces->getDerniereTransaction().article->getDate().jour << "/" << marcheAuxPuces->getDerniereTransaction().article->getDate().mois << "/" << marcheAuxPuces->getDerniereTransaction().article->getDate().annee << "\n";
					trans << "Date: " << marcheAuxPuces->getDerniereTransaction().date.jour << "/" << marcheAuxPuces->getDerniereTransaction().date.mois << "/" << marcheAuxPuces->getDerniereTransaction().date.annee << "\n";
					trans << "-----\n";
				}

				trans.close();

				//On enlève l'article qui vient d'être vendu au fichier du marché aux puces
				ofstream marche(marcheAuxPuces->getNom()+".txt");
				if (marche)
				{
					marche << marcheAuxPuces->getNom() << ";" << marcheAuxPuces->getAdresse() << ";" << marcheAuxPuces->getRevenu() << "\n";
					for (size_t cpt=0;cpt < marcheAuxPuces->getArticlesEnVente().size();cpt++)
					{
						ostringstream conversion;
						conversion << marcheAuxPuces->getArticlesEnVente()[cpt]->getDate().jour << "/" << marcheAuxPuces->getArticlesEnVente()[cpt]->getDate().mois << "/" << marcheAuxPuces->getArticlesEnVente()[cpt]->getDate().annee;
						string date = conversion.str();
						Divers* div;
						Bijou* bij;
						Voiture* voit;
						if (div = dynamic_cast<Divers*>(marcheAuxPuces->getArticlesEnVente()[cpt]))
						{
							marche << marcheAuxPuces->getArticlesEnVente()[cpt]->getNom() << ";D;"<<marcheAuxPuces->getArticlesEnVente()[cpt]->getPrix()<<";"<<marcheAuxPuces->getArticlesEnVente()[cpt]->getDescription()<<";"<<marcheAuxPuces->getArticlesEnVente()[cpt]->getEtat()<<";"<<date<<"\n";
						}
						else if(bij = dynamic_cast<Bijou*>(marcheAuxPuces->getArticlesEnVente()[cpt]))
						{
							marche << marcheAuxPuces->getArticlesEnVente()[cpt]->getNom() << ";B;"<<marcheAuxPuces->getArticlesEnVente()[cpt]->getPrix()<<";"<<marcheAuxPuces->getArticlesEnVente()[cpt]->getDescription()<<";"<<marcheAuxPuces->getArticlesEnVente()[cpt]->getEtat()<<";"<<date<<";"<<bij->getPurete()<<";"<<bij->getMateriau()<<"\n";
						}
						else if(voit = dynamic_cast<Voiture*>(marcheAuxPuces->getArticlesEnVente()[cpt]))
						{
							marche << marcheAuxPuces->getArticlesEnVente()[cpt]->getNom() << ";V;"<<marcheAuxPuces->getArticlesEnVente()[cpt]->getPrix()<<";"<<marcheAuxPuces->getArticlesEnVente()[cpt]->getDescription()<<";"<<marcheAuxPuces->getArticlesEnVente()[cpt]->getEtat()<<";"<<date<<";"<<voit->getKilometrage()<<";"<<voit->getCouleur()<<";"<<voit->getAnnee()<<"\n";
						}
					}
				}

				marche.close();
			
			}
		}
	}
}

int main()
{
	ClientApp app(new Affichage); //Le main crée un objet ClientApp qui gère tout le reste
	return 0;
}