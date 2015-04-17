#include "EtatMateriaux.h"

const string DESCRIPTION = "Pour les matériaux";

EtatMateriaux::EtatMateriaux()
{
}


EtatMateriaux::~EtatMateriaux()
{
}

float EtatMateriaux::calculerPrixEtat(float prix)
{
	return 1;
}

string EtatMateriaux::getDescription()
{
	return DESCRIPTION;
}