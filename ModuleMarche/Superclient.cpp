#include "Superclient.h"


Superclient::Superclient(void)
{
}

Superclient::Superclient(string n,string p,string a,Compte* c):Vendeur(),Client(n,p,a,c),Acheteur()
{
}

Superclient::Superclient(Client* cli):Client(cli)
{
}

Superclient::~Superclient(void)
{
}

string Superclient::afficher() const
{
	string a = "Superclient";
	return a;
}