#ifndef VENDEUR_H
#define VENDEUR_H

#include "Client.h"

#include <vector>

using std::vector;
struct Transaction;
class MarcheAuxPuces;
//Il peut maintenant vendre, les fonctions ressemblent grandement à celles du MarcheAuxPuces
class Vendeur : virtual public Client
{
private:
	vector<Transaction> transactionsEffectuees; //On stocke toutes les transactions faites (les ventes)
public:
	Vendeur(void);
	Vendeur(string,string,string,Compte*);
	Vendeur(Client*);

	virtual string afficher() const;
	struct Transaction getDerniereTransaction() const;
	void ajouterTransaction(int,MarcheAuxPuces*,Article*);
	virtual ~Vendeur(void);
};

#endif