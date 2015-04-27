#ifndef GESTION_SIMULATION_H
#define GESTION_SIMULATION_H

#include <vector>

// Cette classe g�re le contenu d'un simulation (nombre d'employ�, nombre d'articles, etc.)
class ClientApp;
class Vendeur;
class Article;
class GestionSimulation
{
private:
	ClientApp* clientApp;

public:
	GestionSimulation();
	GestionSimulation(ClientApp*);
	~GestionSimulation();

	// Fonctionnement du faux march� simul�
	Article* genererNouvelArticleAleatoire();
	void ajouterNouvelEmployeAleatoire();
	void ajoutClient();
	void commanderArticlesManquantsMarche();
	void ajouterArticleManquantVendeur(Vendeur*);
};

#endif