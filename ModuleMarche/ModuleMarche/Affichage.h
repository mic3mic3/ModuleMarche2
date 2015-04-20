#ifndef AFFICHAGE_H
#define AFFICHAGE_H

//Cette classe contient des méthodes statiques qui regroupent la totalité de l'input et l'output pour l'utilisateur.

#include <string> //Pour utiliser les string
#include <vector> //Pour utiliser les vector

using std::string; //Pour utiliser string sans std::
using std::vector; //Pour utiliser vector sans std::

class Article; //Pour que le compilateur sache que la classe Article existe
class Client;
class ClientApp;
class Fichier;

class Affichage
{
public:
	static const string CS_EXIT_INPUT;
	static ClientApp clientApp;

	Affichage(void);
	~Affichage(void);


	//Méthodes d'affichage
	static void menuDemarrer();
	static void menuInscription();
	static void menuConnexion();
	static void menuSelection();
	static void menuForfaits();
	static void menuAchats();
	static int menuMarche(float,const vector<Article*> &,char);
	static bool menuVerifAchat(bool);
	static void menuVenteArticles();
	static void menuCategories();
	
	//Méthodes de validation
	static string validationEntreesChaines(string);
	static bool validationFloat(const string &);
	static int validationChoixArticle(const string &,int);

	//Transformation du choix d'affichage dans le choix correspondant au tableau
	static int transformationEnPositionTableau(int,char,const vector<Article*> &);

	static void creationFichierCompte(const string &, const string &, const string &, const string &, float, const string &);

	static vector<Fichier> getEntreesEmploye(const Fichier&);
};

#endif
