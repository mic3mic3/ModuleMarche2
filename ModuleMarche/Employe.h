#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <string>

#include "Acheteur.h"

using std::string;

class Compte;
//Hérite d'Acheteur qui hérite de Client, elle possède deux nouveaux attributs
class Employe : public Acheteur
{
private:
	float salaire;
	float rabais;
public:
	Employe(void);
	Employe(string,string,string,Compte*,float,float);
	Employe(Client*);

	virtual bool validerCompte(const float) const;
	void acheter(Article*);
	string afficher() const;

	float getSalaire() const;
	float getRabais() const;
	~Employe(void);
};

#endif