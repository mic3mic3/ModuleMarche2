#ifndef AFFICHAGE_H
#define AFFICHAGE_H

//Cette classe contient des méthodes statiques qui regroupent la totalité de l'input et l'output pour l'utilisateur.

#include <string> //Pour utiliser les string
#include <vector> //Pour utiliser les vector

using std::string; //Pour utiliser string sans std::
using std::vector; //Pour utiliser vector sans std::

class Article; //Pour que le compilateur sache que la classe Article existe
class Client;

class Affichage
{
public:
	Affichage(void);
	~Affichage(void);


	//Méthodes d'affichage
	static char menuDemarrer();
	static void menuInscription();
	static string menuConnexion();
	static char menuSelection(Client*);
	static char menuForfaits();
	static void menuAchats(const vector<Article*> &);
	static int menuMarche(float,const vector<Article*> &,char);
	static bool menuVerifAchat(bool);
	static int menuVenteArticles(float,const vector<Article*> &);
	static char menuCategories();
};

#endif
