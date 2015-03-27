#include "Voiture.h"
#include "Date.h"
#include "Article.h"

#include <string>
#include <ostream>
#include <sstream>
using namespace std;


Voiture::Voiture(void)
{
}

Voiture::Voiture(string n, float p, string d, string e, struct Date date,int k,string c,int a):Article(n,p,d,e,date)
{
	kilometrage = k;
	couleur = c;
	annee = a;
}

Voiture::~Voiture(void)
{
}

int Voiture::getKilometrage() const
{
	return kilometrage;
}

string Voiture::getCouleur() const
{
	return couleur;
}

int Voiture::getAnnee() const
{
	return annee;
}

void Voiture::afficherDetails() const
{
	ostringstream conversion;
	int x = 0;
	conversion << getDate().jour << "/" << getDate().mois << "/" << getDate().annee; 
	string date = conversion.str();
	cout << "Nom: " << getNom() << endl
		<< "Categorie: Voiture" << endl
		<< "Prix: " << getPrix() << endl
		<< "Description: " << getDescription() << endl
		<< "Etat: " << getEtat() << endl
		<< "Date: " << date << endl
		<< "Kilometrage: " << kilometrage << endl
		<< "Couleur: " << couleur << endl
		<< "Annee: " << annee << endl;
	
}