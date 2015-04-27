#ifndef GESTION_SIMULATION_H
#define GESTION_SIMULATION_H

#include <vector>

// Cette classe gère le contenu d'un simulation (nombre d'employé, nombre d'articles, etc.)
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

	// Fonctionnement du faux marché simulé
	Article* genererNouvelArticleAleatoire();
	void ajouterNouvelEmployeAleatoire();
	void ajoutClient();
	void commanderArticlesManquantsMarche();
	void ajouterArticleManquantVendeur(Vendeur*);
};

#endif