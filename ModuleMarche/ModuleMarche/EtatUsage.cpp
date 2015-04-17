#include "EtatUsage.h"

const string EtatUsage::DESCRIPTION = "Usage";

EtatUsage::EtatUsage()
{
}


EtatUsage::~EtatUsage()
{
}

float EtatUsage::calculerPrixEtat(float prix,char type)
{
	return 0.5f * prix;
}

string EtatUsage::getDescription()
{
	return DESCRIPTION;
}