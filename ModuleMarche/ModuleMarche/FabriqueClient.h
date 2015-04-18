#ifndef FABRIQUECLIENT_H
#define FABRIQUECLIENT_H


#include <string>
#include "Compte.h"
#include "Acheteur.h"
#include "Client.h"
#include "Vendeur.h"
#include "Superclient.h"

using std::string;

class FabriqueClient
{
public:
	FabriqueClient();
	~FabriqueClient();

	static Client* creationClient(string, string, string, float, char);
	static Client* modificationForfaitClient(Client* &,char);
};

#endif