#ifndef TRANSACTION_H
#define TRANSACTION_H

//Cette structure est un moyen simple de stocker une transaction avec le client, l'article, le marché aux puces ainsi que la date de celle-ci

#include "Date.h"

class Client;
class Article;
class MarcheAuxPuces;
struct Date;

struct Transaction
{
	Client* client;
	Article* article;
	MarcheAuxPuces* marche;
	Date date;
};

#endif