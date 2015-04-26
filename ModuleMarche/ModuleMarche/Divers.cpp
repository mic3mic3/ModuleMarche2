#include "Divers.h"
#include "Date.h"
#include <ostream>
#include <sstream>

using namespace std;

const float Divers::PRIX_MIN = 1;
const float Divers::PRIX_MAX = 15000;

Divers::Divers(void)
{
}

Divers::Divers(string n, float p, string d, Etat* e, struct Date date):Article(n,p,d,e,date)
{
}

Divers::~Divers(void)
{
}

const string& Divers::afficherDetails() const
{
	ostringstream conversion;
	int x = 0;
	conversion << getDate().jour << "/" << getDate().mois << "/" << getDate().annee; 
	string date = conversion.str();
	return + "Nom: " + getNom() + "\n"
		+ "Categorie: Divers" + "\n"
		+ "Prix: " + to_string(getPrix()) + "\n"
		+ "Description: " + getDescription() + "\n"
		+ "Etat: " + getEtat()->getDescription() + "\n"
		+ "Date: " + date + "\n";
	
}

float Divers::getPrixEtat() const
{
	return getEtat()->calculerPrixEtat(getPrix(), 'D');
}