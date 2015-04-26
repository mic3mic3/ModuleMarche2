#include "Bijou.h"
#include "Date.h"
#include <ostream>
#include <sstream>
using namespace std;

const float Bijou::PRIX_MIN = 10;
const float Bijou::PRIX_MAX = 3000;

Bijou::Bijou(void)
{
}

Bijou::Bijou(string n, float p, string d, Etat* e, struct Date date,string m,int pu):Article(n,p,d,e,date)
{
	materiau = m;
	purete = pu;
}

Bijou::~Bijou(void)
{
}

string Bijou::getMateriau() const
{
	return materiau;
}

float Bijou::getPrixEtat() const
{
	return getEtat()->calculerPrixEtat(getPrix(),'B');
}

int Bijou::getPurete() const
{
	return purete;
}

const string& Bijou::afficherDetails() const
{
	ostringstream conversion;
	int x = 0;
	conversion << getDate().jour << "/" << getDate().mois << "/" << getDate().annee; 
	string date = conversion.str();
	return "Nom: " + getNom() + "\n"
		+ "Categorie: Bijou" + "\n"
		+ "Prix: " + to_string(getPrix()) + "\n"
		+ "Description: " + getDescription() + "\n"
		+ "Etat: " + getEtat()->getDescription() + "\n"
		+ "Date: " + date + "\n"
		+ "Materiau: " + materiau + "\n"
		+ "Purete: " + to_string(purete) + "\n";
	
}