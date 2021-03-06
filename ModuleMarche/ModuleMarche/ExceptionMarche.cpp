#include "ExceptionMarche.h"

#include <iostream>
#include <string>

#include "Affichage.h"

using namespace std;

// Les exceptions sont g�r�es par l'interface pour afficher un feedback au client

ExceptionMarche::ExceptionMarche(string& psMessage, bool pbFatale)
{
	// Cr�ation du message d'erreur sous le format: 
	// �Erreur fatale: 
	// Le march� aux puces n'a pas pu �tre cr��.
	// L'application doit fermer...�
	message = "Erreur";
	if (fatale)
		message += " fatale";
	message += ": ";
	if (psMessage.size() == 0)
		message += "D�sol�, une erreur est survenue dans l'application.";
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

	// Si l'erreur est fatale et emp�che le programme de fonctionner, on quitte.
	if (fatale)
	{
		cout << endl << "Fermeture de l'application" << endl << endl;
		Affichage::attendreTouche();
		
		exit(EXIT_FAILURE);
	}
}