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

void Divers::afficherDetails() const
{
	ostringstream conversion;
	int x = 0;
	conversion << getDate().jour << "/" << getDate().mois << "/" << getDate().annee; 
	string date = conversion.str();
	cout << "Nom: " << getNom() << endl
		<< "Categorie: Divers" << endl
		<< "Prix: " << getPrix() << endl
		<< "Description: " << getDescription() << endl
		<< "Etat: " << getEtat()->getDescription() << endl
		<< "Date: " << date << endl;
	
}

float Divers::getPrixEtat() const
{
	return getEtat()->calculerPrixEtat(getPrix(), 'D');
}