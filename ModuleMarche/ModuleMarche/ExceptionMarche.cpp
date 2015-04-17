#include "ExceptionMarche.h"

#include <string>

using std::string;

ExceptionMarche::ExceptionMarche(string& psMessage)
{
	if (message.size() == 0)
		message = "Désolé, une erreur est survenue.";
	else
		message = psMessage;
}

string ExceptionMarche::getMessage() const
{
	return message;
}