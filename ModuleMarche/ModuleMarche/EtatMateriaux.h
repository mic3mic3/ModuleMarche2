#ifndef ETATMATERIAUX_H
#define ETATMATERIAUX_H

#include <string>
#include "Etat.h"

using std::string;

class EtatMateriaux: public Etat
{
private:
	static const float PRIX_VOITURE;
	static const float PRIX_DIVERS;
	static const float MULTIPLICATEUR_BIJOU;
public:
	static const string DESCRIPTION;

	EtatMateriaux();
	~EtatMateriaux();

	float calculerPrixEtat(float,char);
	string getDescription();
};

#endif