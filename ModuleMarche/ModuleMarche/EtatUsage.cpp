#include "EtatUsage.h"

const string DESCRIPTION = "Usag�";

EtatUsage::EtatUsage()
{
}


EtatUsage::~EtatUsage()
{
}

float EtatUsage::calculerPrixEtat(float prix)
{
	return 0.5 * prix;
}

string EtatUsage::getDescription()
{
	return DESCRIPTION;
}