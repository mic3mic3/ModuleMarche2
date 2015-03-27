#include "Employe.h"
#include "Article.h"
#include "Compte.h"

#include <string>

using std::string;

Employe::Employe(void)
{
}

Employe::Employe(string n, string p, string a, Compte* c, float s, float r):Client(n,p,a,c),Acheteur()
{
	salaire = s;
	rabais = r;
}

Employe::Employe(Client* cli):Client(cli)
{
}

Employe::~Employe(void)
{
}

string Employe::afficher() const
{
	string a = "Employé";
	return a;
}

void Employe::acheter(Article* a)
{
	float pourcentage= (100-rabais)/100;

	getCompte()->soustraireMontant(a->getPrix()*pourcentage);	
	ajouterArticle(a);
}

float Employe::getRabais() const
{
	return rabais;
}

float Employe::getSalaire() const
{
	return salaire;
}

bool Employe::validerCompte(const float m) const
{
	float pourcentage= (100-rabais)/100; //Prend en compte le rabais
	return getCompte()->validerMontant(m*pourcentage);
}