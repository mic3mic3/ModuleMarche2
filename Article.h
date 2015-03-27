#ifndef ARTICLE_H
#define ARTICLE_H

//Cette classe sert à la création d'articles qui se transfèrent du magasin au client lors d'un achat

#include <string> //Pour utiliser les string
#include <iostream>

#include "Date.h"

using std::string; //Pour utiliser les string sans std::
using std::ostream;

struct Date; //Pour que le compilateur sache que la structure Date existe

class Article
{
private:
	string nom;
	float prix;
	string description;
	string etat;
	struct Date dateFabrication;
public:
	Article(void);
	Article(string,float,string,string,struct Date);
	~Article(void);

	void mettreAJourPrix(const float);
	void mettreAJourEtat(const string &);

	virtual void afficherDetails() const = 0;

	//Tous les accesseurs requis
	string getNom() const;
	float getPrix() const;
	string getDescription() const;
	string getEtat() const;
	struct Date getDate() const;

	friend ostream & operator<< (ostream & out,Article*);
};

#endif