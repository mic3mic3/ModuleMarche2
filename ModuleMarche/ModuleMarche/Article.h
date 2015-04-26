#ifndef ARTICLE_H
#define ARTICLE_H

//Cette classe sert à la création d'articles qui se transfèrent du magasin au client lors d'un achat

#include <string> //Pour utiliser les string
#include <iostream>

#include "Date.h"
#include "Etat.h"
#include "EtatNeuf.h"
#include "EtatUsage.h"
#include "EtatMateriaux.h"

using std::string; //Pour utiliser les string sans std::
using std::ostream;

struct Date; //Pour que le compilateur sache que la structure Date existe

class Article
{
private:
	string nom;
	float prix;
	string description;
	Etat* etat;
	struct Date dateFabrication;
public:
	Article(void);
	Article(string,float,string,Etat*,struct Date);
	~Article(void);

	void mettreAJourPrix(const float);
	void mettreAJourEtat(Etat*);

	virtual const string& afficherDetails() const = 0;

	//Tous les accesseurs requis
	string getNom() const;
	float getPrix() const;
	virtual float getPrixEtat() const;
	string getDescription() const;
	Etat* getEtat() const;
	struct Date getDate() const;

	friend ostream & operator<< (ostream & out,Article*);
};

#endif