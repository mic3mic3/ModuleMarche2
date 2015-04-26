#include "Voiture.h"
#include "Date.h"
#include "Article.h"

#include <string>
#include <ostream>
#include <sstream>
using namespace std;

const float Voiture::PRIX_MIN = 15000;
const float Voiture::PRIX_MAX = 100000;

Voiture::Voiture(void)
{
}

Voiture::Voiture(string n, float p, string d, Etat* e, struct Date date,int k,string c,int a):Article(n,p,d,e,date)
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

const string Voiture::afficherDetails() const
{
	ostringstream conversion;
	int x = 0;
	conversion << getDate().jour << "/" << getDate().mois << "/" << getDate().annee; 
	string date = conversion.str();
	return "Nom: " + getNom() + "\n"
		+ "Categorie: Voiture" + "\n"
		+ "Prix: " + to_string(getPrix()) + "\n"
		+ "Description: " + getDescription() + "\n"
		+ "Etat: " + getEtat()->getDescription() + "\n"
		+ "Date: " + date + "\n"
		+ "Kilometrage: " + to_string(kilometrage) + "\n"
		+ "Couleur: " + couleur + "\n"
		+ "Annee: " + to_string(annee) + "\n";	
}

float Voiture::getPrixEtat() const
{
	return getEtat()->calculerPrixEtat(getPrix(), 'V');
}