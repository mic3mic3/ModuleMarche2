#ifndef SUPERCLIENT_H
#define SUPERCLIENT_H

#include "Acheteur.h"
#include "Vendeur.h"

//H�rite de deux classes qui h�ritent de Client, c'est pourquoi Vendeur et Acheteur h�ritent virtuellement de Client
class Superclient : public Vendeur, public Acheteur
{
public:
	Superclient(void);
	Superclient(string,string,string,Compte*);
	Superclient(Client*);
	string afficher() const;
	~Superclient(void);
};

#endif