#ifndef CLIENT_H
#define CLIENT_H

//Cette classe sert à créer le client qui pourra acheter les articles d'un marché

#include <string>
#include <vector>

using std::string;
using std::vector;

class Compte; //Pour que le compilateur sache que la classe Compte existe
class Article; //Pour que le compilateur sache que la classe Article existe

class Client
{
private:
	string nom;
	string prenom;
	string adresse;
	Compte* compte; //Le client doit obligatoirement posséder un Compte
	vector<Article*> articles; //On stocke tous les achats du Client
public:
	Client(void);
	Client(string,string,Compte*);
	Client(string,string,string,Compte*);
	Client(Client*);
	virtual ~Client(void);

	virtual bool validerCompte(const float) const;
	void ajouterArticle(Article*);
	void enleverArticle(int);
	virtual string afficher() const = 0;
	//Tous les accesseurs requis
	vector<Article*> getArticles() const;
	float getSolde() const;
	string getNom() const;
	string getPrenom() const;
	string getAdresse() const;
	Compte* getCompte() const;


};

#endif