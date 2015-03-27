#ifndef ACHETEUR_H
#define ACHETEUR_H

#include "Client.h"
//Il possède la fonction acheter qui était autrefois dans Client
class Acheteur : virtual public Client //hérite virtuellement de client (causé par Superclient)
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