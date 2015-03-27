#include <ctime>

#include "MarcheAuxPuces.h"
#include "Transaction.h"
#include "Date.h"
#include "Article.h"
#include "Client.h"
#include "Compte.h"
#include "Employe.h"

MarcheAuxPuces::MarcheAuxPuces(void)
{
	nom = "Non sp�cifi�";
	adresse = "Non sp�cifi�e";
}

//Le constructeur qui sera utilis�
MarcheAuxPuces::MarcheAuxPuces(string n, string a, Compte* c)
{
	nom = n;
	adresse = a;
	compte= c;
}

MarcheAuxPuces::~MarcheAuxPuces(void)
{
	//Suppression des articles dans le vecteur des articles disponibles
	while(!articlesEnVente.empty())
	{
		delete articlesEnVente.back();
		articlesEnVente.back() = NULL;
		articlesEnVente.pop_back();
	}
	//Dans une transaction, l'article et le client sur lequels pointe la transaction sont d�j� d�truits lors de la destruction du client donc pas besoin de le delete
	//Puis, le march� sur lequel la transaction pointe sera d�truit apr�s le passage dans ce destructeur
	//On ne fait que remettre les pointeurs � NULL puis les sortir du vecteur
	while(!transactionsEffectuees.empty())
	{
		transactionsEffectuees.back().article = NULL;
		transactionsEffectuees.back().client = NULL;
		transactionsEffectuees.back().marche = NULL;
		transactionsEffectuees.pop_back();
	}
	delete compte;
}

const vector<Article*> MarcheAuxPuces::getArticlesEnVente()
{
	return articlesEnVente;
}

void MarcheAuxPuces::ajouterArticle(Article* a)
{
	articlesEnVente.push_back(a);
}

void MarcheAuxPuces::ajouterEmploye(Employe* e)
{
	personnel.push_back(e);
}

const vector<Employe*> & MarcheAuxPuces::getPersonnel()
{
	return personnel;
}

bool MarcheAuxPuces::validerCompte(const float m) const
{
	return compte->validerMontant(m);
}

void MarcheAuxPuces::enleverArticle(int pos)
{
	articlesEnVente.erase(articlesEnVente.begin()+pos); //On enl�ve l'article du vecteur des articles selon la position
}

void MarcheAuxPuces::ajouterTransaction(int pos,Client* c,Article* a)
{
	struct Date date;

	//Utilisation de ctime pour obtenir la date d'aujourd'hui
	time_t maintenant;
	struct tm *local;

	time(&maintenant);				// On prend l'heure courante
	local = localtime(&maintenant);	// On change l'heure courante selon l'heure locale

	date.jour = local->tm_mday;
	date.mois = local->tm_mon + 1;		// Les mois sont de 0 � 11
	date.annee = local->tm_year + 1900; //Les ann�es commencent � 1900 (0 -> 1900)
	

	struct Transaction trans;
	trans.client = c;
	trans.article = a;
	trans.marche = this;
	trans.date = date;
	transactionsEffectuees.push_back(trans); //On ajoute la Transaction dans le vecteur des transactions
	Employe* emp;
	if (emp = dynamic_cast<Employe*>(c))
	{
		float pourcentage = (100-emp->getRabais())/100;
		compte->ajouterMontant(a->getPrix()*pourcentage); //On augmente le revenu avec le prix de l'article vendu
	}
	else
	{
		compte->ajouterMontant(a->getPrix()); //On augmente le revenu avec le prix de l'article vendu
	}
	enleverArticle(pos); //On appelle la fonction qui enl�ve l'article du vecteur des articles disponibles
}

string MarcheAuxPuces::getNom() const
{
	return nom;
}

string MarcheAuxPuces::getAdresse() const
{
	return adresse;
}

float MarcheAuxPuces::getRevenu() const
{
	return compte->getSolde();
}

struct Transaction MarcheAuxPuces::getDerniereTransaction() const
{
	return transactionsEffectuees.back();
}

void MarcheAuxPuces::acheter(Article* a)
{
	compte->soustraireMontant(a->getPrix());	
	ajouterArticle(a);
}