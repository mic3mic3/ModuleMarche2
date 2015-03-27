#include "Bijou.h"
#include "Date.h"
#include <ostream>
#include <sstream>
using namespace std;

Bijou::Bijou(void)
{
}

Bijou::Bijou(string n, float p, string d, string e, struct Date date,string m,int pu):Article(n,p,d,e,date)
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

int Bijou::getPurete() const
{
	return purete;
}

void Bijou::afficherDetails() const
{
	ostringstream conversion;
	int x = 0;
	conversion << getDate().jour << "/" << getDate().mois << "/" << getDate().annee; 
	string date = conversion.str();
	cout << "Nom: " << getNom() << endl
		<< "Categorie: Bijou" << endl
		<< "Prix: " << getPrix() << endl
		<< "Description: " << getDescription() << endl
		<< "Etat: " << getEtat() << endl
		<< "Date: " << date << endl
		<< "Materiau: " << materiau << endl
		<< "Purete: " << purete << endl;
	
}