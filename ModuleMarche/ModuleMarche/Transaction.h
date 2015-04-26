#ifndef TRANSACTION_H
#define TRANSACTION_H

//Cette structure est un moyen simple de stocker une transaction avec le client, l'article, le marché aux puces ainsi que la date de celle-ci

#include "Date.h"
#include <string>

class Client;
class Article;
class MarcheAuxPuces;
struct Date;

struct Transaction
{
	const static char CC_ACHAT_PAR_CLIENT = 'A';
	const static char CC_VENTE_PAR_CLIENT = 'V';

	Client* client;
	Article* article;
	MarcheAuxPuces* marche;
	Date date;
	char type;
};

#endif