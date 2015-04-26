#ifndef CLIENTAPP_H
#define CLIENTAPP_H
//Version 2 du programme
//Cette classe est ce qui fait le lien entre tous les composantes du programme: le client, le marché, l'affichage (contenant également les inputs)

#include <string>
#include <vector>

using std::vector;
using std::string;

class Affichage; //Pour que le compilateur sache que la classe Affichage existe
class Client; //Pour que le compilateur sache que la classe  Client existe
class MarcheAuxPuces; //Pour que le compilateur sache que la classe MarcheAuxPuces existe
class Article;
class Employe;
class Fichier;

class ClientApp
{
private:
	Client* client;
	MarcheAuxPuces* marcheAuxPuces;
	//Affichage* affichage;
	string compte;
	vector<string> comptesEmployes;//Le nom du compte (inscription) du client (pas la classe Compte)
public:
	ClientApp(void);
	~ClientApp(void);

	static const string CS_NOM_MARCHE_AUX_PUCES;

	Client* getClient() const;
	string getCompte() const;
	MarcheAuxPuces* getMarcheAuxPuces() const;
	void setClient(Client*);

	//Différentes fonctions qui appellent des fonctions d'Affichage pour ensuite faire des manipulations sur les inputs de l'utilisateur retourné par les fonctions d'Affichage
	void connexion(const Fichier&, const Fichier&, const vector<Fichier>&);
	Client* changementForfait(char);
	Client* inscription(string,string,string,float,string);
	void creationClient(const Fichier&);
	void creationMarche(const Fichier&, const vector<Fichier>&);
	void deconnexion();
	Employe* getEmployeExistant(string&);
	bool venteArticleDuClient(int);

	Article* getArticleFromStructure(const Fichier&, size_t);
	Employe* getEmployeFromStructure(const Fichier&, size_t);
	//Client* getClientFromStructure(vector<vector<string>>&, size_t);
};

#endif