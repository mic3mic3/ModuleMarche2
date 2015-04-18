#include "EtatMateriaux.h"

const string EtatMateriaux::DESCRIPTION = "Materiaux";
const float EtatMateriaux::PRIX_VOITURE = 300;
const float EtatMateriaux::PRIX_DIVERS = 0;
const float EtatMateriaux::MULTIPLICATEUR_BIJOU = 0.05f;

EtatMateriaux::EtatMateriaux()
{
}


EtatMateriaux::~EtatMateriaux()
{
}

float EtatMateriaux::calculerPrixEtat(float prix,char type)
{
	if (type == 'B')
	{
		return MULTIPLICATEUR_BIJOU * prix;
	}
	else if (type == 'V')
	{
		return PRIX_VOITURE;
	}
	else
	{
		return PRIX_DIVERS;
	}
}

string EtatMateriaux::getDescription()
{
	return DESCRIPTION;
}