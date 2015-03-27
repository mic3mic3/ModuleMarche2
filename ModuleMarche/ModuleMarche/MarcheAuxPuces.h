#ifndef MARCHEAUXPUCES_H
#define MARCHEAUXPUCES_H

//Cette classe sert � cr�er un march� aux puces qui contiendra des articles qui pourront �tre achet�s par un client

#include <string>
#include <vector>

using std::string;
using std::vector;

class Article;
class Client;
class Compte;
class Employe;
struct Transaction; //Pour que le compilateur sache que la structure Transaction existe

class MarcheAuxPuces
{
private:
	string nom;
	string adresse;
	Compte* compte; //Ajout du compte au lieu du revenu
	vector<Employe*> personnel;
	vector<Article*> articlesEnVente; //On stocke tous les articles disponibles du MarcheAuxPuces
	vector<Transaction> transactionsEffectuees; //On stocke toutes les transactions faites (les ventes)
public:
	MarcheAuxPuces(void);
	MarcheAuxPuces(string,string,Compte*);
	~MarcheAuxPuces(void);

	void ajouterEmploye(Employe*);
	void ajouterArticle(Article*);
	void enleverArticle(int);
	void acheter(Article*);
	void ajouterTransaction(int,Client*,Article*);
	
	//Tous les accesseurs n�cessaires
	bool validerCompte(const float) const;
	string getNom() const;
	string getAdresse() const;
	float getRevenu() const;
	struct Transaction getDerniereTransaction() const; //Utilis�e pour �crire la derni�re transaction faite dans un fichier apr�s chaque transaction
	const vector<Article*> getArticlesEnVente();
	const vector<Employe*> & getPersonnel();
};

#endif