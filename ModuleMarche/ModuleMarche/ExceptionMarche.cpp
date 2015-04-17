#include "ExceptionMarche.h"

#include <iostream>
#include <string>

using namespace std;

ExceptionMarche::ExceptionMarche(string& psMessage, bool pbFatale)
{
	if (message.size() == 0)
		message = "D�sol�, une erreur est survenue.";
	else
		message = psMessage;

	fatale = pbFatale;
	if (pbFatale)
		message += "L'application doit fermer...";
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
	// Affichage du message sous le format: 
	// �Erreur fatale: 
	// Le march� aux puces n'a pas pu �tre cr��.
	// L'application doit fermer...�
	string lsMessageAAfficher = "Erreur";
	if (fatale)
		lsMessageAAfficher += " fatale";
	lsMessageAAfficher += ": ";
	if (message.size() == 0)
		lsMessageAAfficher += "D�sol�, une erreur s'est produite dans l'application.";
	else
		lsMessageAAfficher += message;
	lsMessageAAfficher += "\n";
	if (fatale)
		lsMessageAAfficher += "L'application doit fermer...\n";
	cout << lsMessageAAfficher;

	// Si l'erreur est fatale et emp�che le programme de fonctionner, on quitte.
	if (fatale)
		exit(EXIT_FAILURE);
}