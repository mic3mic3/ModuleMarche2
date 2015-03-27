#ifndef VOITURE_H
#define VOITURE_H

#include <string>
#include "Article.h"
using std::string;

struct Date;
//Hérite d'Article et possède trois nouveaux attributs
class Voiture : public Article
{
private:
	int kilometrage;
	string couleur;
	int annee;
public:
	Voiture(void);
	Voiture(string,float,string ,string,struct Date,int,string,int);
	~Voiture(void);

	void afficherDetails() const;

	int getKilometrage() const;
	string getCouleur() const;
	int getAnnee() const;
};

#endif