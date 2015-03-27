#include "Compte.h"


Compte::Compte(void)
{
	solde = 0;
}

Compte::Compte(float s)
{
	solde = s;
}

Compte::~Compte(void)
{
}

void Compte::ajouterMontant(float s)
{
	solde += s;
}

void Compte::soustraireMontant(float s)
{
	solde -= s;
}

bool Compte::validerMontant(float s) const //On v�rifie si le solde est plus grand ou �gal au montant qu'on re�oit
{
	if (solde >= s)
	{
		return true;
	}
	return false;
}

float Compte::getSolde() const
{
	return solde;
}