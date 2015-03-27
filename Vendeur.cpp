#include <ctime>

#include "Vendeur.h"
#include "Date.h"
#include "Compte.h"
#include "Transaction.h"
#include "Article.h"

Vendeur::Vendeur(void):Client()
{
}

Vendeur::Vendeur(string n,string p,string a,Compte* c):Client(n,p,a,c)
{
}

Vendeur::Vendeur(Client* cli):Client(cli)
{

}

Vendeur::~Vendeur(void) //Pas besoin de s'occuper des transactions car les informations qu'elle contiennent vont se faire supprimer ailleurs de toute manière (évite la double libération de la mémoire qui cause des problèmes)
{
}

void Vendeur::ajouterTransaction(int pos,MarcheAuxPuces* c,Article* a)
{
	struct Date date;

	//Utilisation de ctime pour obtenir la date d'aujourd'hui
	time_t maintenant;
	struct tm *local;

	time(&maintenant);				// On prend l'heure courante
	local = localtime(&maintenant);	// On change l'heure courante selon l'heure locale

	date.jour = local->tm_mday;
	date.mois = local->tm_mon + 1;		// Les mois sont de 0 à 11
	date.annee = local->tm_year + 1900; //Les années commencent à 1900 (0 -> 1900)
	

	struct Transaction trans;
	trans.client = this;
	trans.article = a;
	trans.marche = c;
	trans.date = date;
	transactionsEffectuees.push_back(trans); //On ajoute la Transaction dans le vecteur des transactions
	getCompte()->ajouterMontant(a->getPrix()); //On augmente le revenu avec le prix de l'article vendu
	enleverArticle(pos); //On appelle la fonction qui enlève l'article du vecteur des articles disponibles
}

string Vendeur::afficher() const
{
	string a = "Vendeur";
	return a;
}

struct Transaction Vendeur::getDerniereTransaction() const
{
	return transactionsEffectuees.back();
}