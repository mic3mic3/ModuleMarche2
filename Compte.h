#ifndef COMPTE_H
#define COMPTE_H

//La classe Compte est tr�s simple, elle repr�sente l'argent d'un client et elle contient les op�rations qu'on effectue sur son solde

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