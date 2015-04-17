#ifndef ETATNEUF_H
#define ETATNEUF_H

#include <string>
#include "Etat.h"

using std::string;
class EtatNeuf: public Etat
{
public:
	static const string DESCRIPTION;

	EtatNeuf();
	~EtatNeuf();

	float calculerPrixEtat(float);
	string getDescription();
};

#endif