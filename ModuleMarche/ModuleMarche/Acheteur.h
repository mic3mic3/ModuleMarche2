#ifndef ACHETEUR_H
#define ACHETEUR_H

#include "Client.h"
//Il poss�de la fonction acheter qui �tait autrefois dans Client
class Acheteur : virtual public Client //h�rite virtuellement de client (caus� par Superclient)
{
public:
	Acheteur(void);
	Acheteur(string,string,string,Compte*);
	Acheteur(Client*);

	virtual void acheter(Article*);
	virtual string afficher() const;
	virtual ~Acheteur(void);
};

#endif