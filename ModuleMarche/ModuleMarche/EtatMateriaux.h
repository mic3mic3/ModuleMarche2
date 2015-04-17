#ifndef ETATMATERIAUX_H
#define ETATMATERIAUX_H

#include <string>
#include "Etat.h"

using std::string;

class EtatMateriaux: public Etat
{
public:
	static const string DESCRIPTION;

	EtatMateriaux();
	~EtatMateriaux();

	float calculerPrixEtat(float);
	string getDescription();
};

#endif