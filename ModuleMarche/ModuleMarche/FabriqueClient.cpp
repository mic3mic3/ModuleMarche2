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
		solde -= Acheteur::FORFAIT;
		return new Acheteur(nom, prenom, adresse, new Compte(solde));
	}
	else if (forfait == 'V' && solde >= Vendeur::FORFAIT)
	{
		solde -= Vendeur::FORFAIT;
		return new Vendeur(nom, prenom, adresse, new Compte(solde));
	}
	else if (forfait == 'S' && solde >= Superclient::FORFAIT)
	{
		solde -= Superclient::FORFAIT;
		return new Superclient(nom, prenom, adresse, new Compte(solde));
	}
	else
	{
		return nullptr;
	}
}

Client* FabriqueClient::modificationForfaitClient(Client* & client,char forfait)
{
	if (forfait == 'A' && client->getCompte()->getSolde() >= Acheteur::FORFAIT && !dynamic_cast<Acheteur*>(client))
	{
		client->getCompte()->soustraireMontant(Acheteur::FORFAIT);
		return new Acheteur(client);
	}
	else if (forfait == 'V' && client->getCompte()->getSolde() >= Vendeur::FORFAIT && !dynamic_cast<Vendeur*>(client))
	{
		client->getCompte()->soustraireMontant(Vendeur::FORFAIT);
		return new Vendeur(client);
	}
	else if (forfait == 'S' && client->getCompte()->getSolde() >= Superclient::FORFAIT && !dynamic_cast<Superclient*>(client))
	{
		client->getCompte()->soustraireMontant(Superclient::FORFAIT);
		return new Superclient(client);
	}
	else
	{
		return nullptr;
	}
}