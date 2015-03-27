#include "Divers.h"
#include "Date.h"
#include <ostream>
#include <sstream>

using namespace std;

Divers::Divers(void)
{
}

Divers::Divers(string n, float p, string d, string e, struct Date date):Article(n,p,d,e,date)
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
		<< "Etat: " << getEtat() << endl
		<< "Date: " << date << endl;
	
}