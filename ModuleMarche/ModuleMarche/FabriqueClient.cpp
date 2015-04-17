#include "FabriqueClient.h"


FabriqueClient::FabriqueClient()
{
}


FabriqueClient::~FabriqueClient()
{
}

Client* FabriqueClient::creationClient(string nom, string prenom, string adresse, float solde, char forfait)
{
	if (forfait == 'A' && solde >= Acheteur::FORFAIT)
	{
		return new Acheteur(nom, prenom, adresse, new Compte(solde));
	}
	else if (forfait == 'V' && solde >= Vendeur::FORFAIT)
	{
		return new Vendeur(nom, prenom, adresse, new Compte(solde));
	}
	else if (forfait == 'S' && solde >= Superclient::FORFAIT)
	{
		return new Superclient(nom, prenom, adresse, new Compte(solde));
	}
	else
	{
		return nullptr;
	}
}

/*Client* FabriqueClient::modificationForfaitClient(Client* client,char forfait)
{
	if (forfait == 'A' && client->get >= Acheteur::FORFAIT)
	{
		return new Acheteur(nom, prenom, adresse, new Compte(solde));
	}
	else if (forfait == 'V' && solde >= Vendeur::FORFAIT)
	{
		return new Vendeur(nom, prenom, adresse, new Compte(solde));
	}
	else if (forfait == 'S' && solde >= Superclient::FORFAIT)
	{
		return new Superclient(nom, prenom, adresse, new Compte(solde));
	}
	else
	{
		return nullptr;
	}
}*/