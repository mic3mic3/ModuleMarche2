#include "FabriqueArticle.h"


FabriqueArticle::FabriqueArticle()
{
}


FabriqueArticle::~FabriqueArticle()
{
}

Article* FabriqueArticle::creationArticle(char type,string nomArticle, float prix, string description, string etat, struct Date dateFabrication, int attribut1 = 0, string attribut2 = "", int attribut3 = 0)
{
	Etat* vraiEtat;
	if (etat == "Neuf")
	{
		vraiEtat = new EtatNeuf();
	}
	else if (etat == "Usage")
	{
		vraiEtat = new EtatUsage();
	}
	else
	{
		vraiEtat = new EtatMateriaux();
	}
	if (type == 'D' && prix >= Divers::PRIX_MAX && prix >= Divers::PRIX_MAX)
	{
		return new Divers(nomArticle, prix, description, vraiEtat,dateFabrication);
	}
	else if (type == 'B' && prix >= Bijou::PRIX_MAX && prix >= Bijou::PRIX_MAX)
	{
		return new Bijou(nomArticle, prix, description, vraiEtat, dateFabrication,attribut2,attribut1);
	}
	else if (type == 'V' && prix >= Voiture::PRIX_MAX && prix >= Voiture::PRIX_MAX)
	{
		return new Voiture(nomArticle, prix, description, vraiEtat, dateFabrication,attribut1,attribut2,attribut3);
	}
	else
	{
		delete vraiEtat;
		return nullptr;
	}
}