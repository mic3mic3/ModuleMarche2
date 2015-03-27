#include "Article.h"

#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;


Article::Article(void)
{
}


//Le constructeur qui sera utilisé
Article::Article(string n, float p, string d, string e, struct Date date)
{
	nom = n;
	prix = p;
	description = d;
	etat = e;
	dateFabrication = date;
}

Article::~Article(void)
{
}

void Article::mettreAJourPrix(float p)
{
	prix = p;
}

void Article::mettreAJourEtat(const string &e) //Par référence pour une string
{
	etat = e;
}

string Article::getNom() const
{
	return nom;
}

float Article::getPrix() const
{
	return prix;
}

string Article::getDescription() const
{
	return description;
}

string Article::getEtat() const
{
	return etat;
}

struct Date Article::getDate() const
{
	return dateFabrication;
}
ostream & operator<< (ostream & out, Article* a) //Surcharge de l'opérateur << qui est utilisée comme affichage des l'informations sommaires
{
	ostringstream conversion;
	int x = 0;
	conversion << a->getDate().jour << "/" << a->getDate().mois << "/" << a->getDate().annee; 
	string date = conversion.str();
	out << setw(13) << left << a->getNom() << setw(10) << left << fixed << setprecision(2) << a->getPrix() << setw(25) << left  << a->getDescription() << setw(17) << left  << a->getEtat() << setw(10) << left  << date << endl;
	return out;
}

