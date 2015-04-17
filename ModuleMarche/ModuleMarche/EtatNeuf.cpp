#include "EtatNeuf.h"

const string EtatNeuf::DESCRIPTION = "Neuf";

EtatNeuf::EtatNeuf()
{
}


EtatNeuf::~EtatNeuf()
{
}

float EtatNeuf::calculerPrixEtat(float prix,char type)
{
	return prix;
}

string EtatNeuf::getDescription()
{
	return DESCRIPTION;
}