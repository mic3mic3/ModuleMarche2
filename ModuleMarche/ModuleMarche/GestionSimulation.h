#ifndef GESTION_SIMULATION_H
#define GESTION_SIMULATION_H

#include <vector>
#include <Windows.h>
#include <random>

// Cette classe gère le contenu d'un simulation (nombre d'employé, nombre d'articles, etc.)
class ClientApp;
class Article;
class GestionSimulation
{
private:
	ClientApp* clientApp;

public:
	static std::default_random_engine generator;

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