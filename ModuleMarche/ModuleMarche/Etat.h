#ifndef ETAT_H
#define ETAT_H

#include <string>

using std::string;

class Etat
{
public:
	Etat();
	~Etat();

	virtual float calculerPrixEtat(float) = 0;
	virtual string getDescription();
};

#endif
