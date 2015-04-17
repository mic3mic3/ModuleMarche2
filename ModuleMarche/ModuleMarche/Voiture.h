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
	static const float PRIX_MIN;
	static const float PRIX_MAX;

	Voiture(void);
	Voiture(string,float,string ,Etat*,struct Date,int,string,int);
	~Voiture(void);

	void afficherDetails() const;

	float getPrixEtat() const;
	int getKilometrage() const;
	string getCouleur() const;
	int getAnnee() const;
};

#endif