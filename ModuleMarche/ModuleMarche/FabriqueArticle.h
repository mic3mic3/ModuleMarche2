#ifndef FABRIQUEARTICLE_H
#define FABRIQUEARTICLE_H

#include "Article.h"

class FabriqueArticle
{
public:
	FabriqueArticle();
	~FabriqueArticle();

	static Article* creationArticle();
};

#endif