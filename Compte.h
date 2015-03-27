#ifndef COMPTE_H
#define COMPTE_H

//La classe Compte est très simple, elle représente l'argent d'un client et elle contient les opérations qu'on effectue sur son solde

class Compte
{
private:
	float solde;
public:
	Compte(void);
	Compte(float);
	~Compte(void);

	void ajouterMontant(float);
	void soustraireMontant(float);
	bool validerMontant(float) const;

	//Seul accesseur
	float getSolde() const;

};

#endif