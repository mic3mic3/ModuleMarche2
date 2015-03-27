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

class ClientApp
{
private:
	Client* client;
	MarcheAuxPuces* marcheAuxPuces;
	Affichage* affichage;
	string compte;
	vector<string> comptesEmployes;//Le nom du compte (inscription) du client (pas la classe Compte)
public:
	ClientApp(void);
	ClientApp(Affichage*);
	~ClientApp(void);

	//Différentes fonctions qui appellent des fonctions d'Affichage pour ensuite faire des manipulations sur les inputs de l'utilisateur retourné par les fonctions d'Affichage
	void demarrer();
	void inscription();
	void connexion();
	void creationClient(const string &);
	void creationMarche(const string &);
	void selection();
	void voirAchats();
	void voirArticles();
	void voirForfaits();
	void voirVenteArticles();

};

#endif