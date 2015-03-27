#include "Client.h"
#include "Compte.h"
#include "Article.h"

Client::Client(void)
{
}

Client::Client(string n, string p, Compte* c)
{
	nom = n;
	prenom = p;
	adresse = "Non spécifiée";
	compte = c;
}


//Le constructeur qui sera utilisé dans cette application
Client::Client(string n, string p, string a, Compte* c)
{
	nom = n;
	prenom = p;
	adresse = a;
	compte = c;
}

Client::Client(Client* cli)
{
	nom = cli->getNom();
	prenom = cli->getPrenom();
	adresse = cli->getAdresse();
	compte = new Compte(cli->getCompte()->getSolde());
	while(!cli->getArticles().empty())
	{
		articles.push_back(cli->getArticles().front());
		cli->enleverArticle(0);
	}
	delete cli;
}

Client::~Client(void)
{
	//On détruit le compte ainsi que tous les articles dans le vecteur d'achats
	delete compte;
	compte = NULL;
	while(!articles.empty())
	{
		delete articles.back();
		articles.back() = NULL;
		articles.pop_back();
	}
}

bool Client::validerCompte(const float m) const
{
	return compte->validerMontant(m);
}

void Client::ajouterArticle(Article* a)
{
	articles.push_back(a); //On ajoute l'article au vecteur des achats
}

void Client::enleverArticle(int pos)
{
	articles.erase(articles.begin()+pos); //On enlève l'article du vecteur des articles selon la position
}

vector<Article*> Client::getArticles() const
{
	return articles;
}

float Client::getSolde() const
{
	return compte->getSolde();
}

string Client::getNom() const
{
	return nom;
}

string Client::getPrenom() const
{
	return prenom;
}

string Client::getAdresse() const
{
	return adresse;
}

Compte* Client::getCompte() const
{
	return compte;
}