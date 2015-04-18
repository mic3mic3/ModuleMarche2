#ifndef FABRIQUEARTICLE_H
#define FABRIQUEARTICLE_H

#include "Article.h"
#include "Bijou.h"
#include "Voiture.h"
#include "Divers.h"

class FabriqueArticle
{
public:
	FabriqueArticle();
	~FabriqueArticle();

	static Article* creationArticle(char,string, float, string, string, struct Date, int, string, int);
};

#endif