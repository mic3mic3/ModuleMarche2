#include "ExceptionMarche.h"

#include <iostream>
#include <string>

using namespace std;

ExceptionMarche::ExceptionMarche(string& psMessage, bool pbFatale)
{
	if (message.size() == 0)
		message = "Désolé, une erreur est survenue.";
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
	cout << message;

	if (fatale)
		exit(EXIT_FAILURE);
}