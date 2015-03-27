#include "Acheteur.h"
#include "Article.h"
#include "Compte.h"

#include <string>

using std::string;

Acheteur::Acheteur(void):Client()
{
}

Acheteur::Acheteur(string n, string p, string a, Compte* c):Client(n,p,a,c)
{
}

Acheteur::Acheteur(Client* cli):Client(cli)
{
}

Acheteur::~Acheteur(void)
{
}

string Acheteur::afficher() const
{
	string a = "Acheteur";
	return a;
}

void Acheteur::acheter(Article* a)
{
	getCompte()->soustraireMontant(a->getPrix());	
	ajouterArticle(a);
}