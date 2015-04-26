#include "ExceptionMarche.h"

#include <iostream>
#include <string>

#include "Affichage.h"

using namespace std;

ExceptionMarche::ExceptionMarche(string& psMessage, bool pbFatale)
{
	// Création du message d'erreur sous le format: 
	// «Erreur fatale: 
	// Le marché aux puces n'a pas pu être créé.
	// L'application doit fermer...»
	message = "Erreur";
	if (fatale)
		message += " fatale";
	message += ": ";
	if (psMessage.size() == 0)
		message += "Désolé, une erreur est survenue dans l'application.";
	else
		message += psMessage;

	fatale = pbFatale;
}

ExceptionMarche::~ExceptionMarche()
{

}

string ExceptionMarche::getMessage() const
{
	return message;
}

bool ExceptionMarche::getFatale() const
{
	return fatale;
}

void ExceptionMarche::Gerer() const
{
	cout << endl << endl << message << endl;

	// Si l'erreur est fatale et empêche le programme de fonctionner, on quitte.
	if (fatale)
	{
		Affichage::attendreTouche(string("\nFermeture de l'application...\n\nAppuyez sur Entree pour quitter..."));
		
		exit(EXIT_FAILURE);
	}
}