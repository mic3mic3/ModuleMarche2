#ifndef ETATUSAGE_H
#define ETATUSAGE_H

#include <string>
#include "Etat.h"

using std::string;

class EtatUsage: public Etat
{
public:
	static const string DESCRIPTION;
	EtatUsage();
	~EtatUsage();

	float calculerPrixEtat(float,char);
	string getDescription();
};

#endif