#include "EtatNeuf.h"

const string DESCRIPTION = "Neuf";

EtatNeuf::EtatNeuf()
{
}


EtatNeuf::~EtatNeuf()
{
}

float EtatNeuf::calculerPrixEtat(float prix)
{
	return prix;
}

string EtatNeuf::getDescription()
{
	return DESCRIPTION;
}