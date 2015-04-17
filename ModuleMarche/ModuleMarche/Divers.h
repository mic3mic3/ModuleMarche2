#ifndef DIVERS_H
#define DIVERS_H

struct Date;

#include <string>
#include "Article.h"

using std::string;
//H�rite d'Article, sans aucun attribut suppl�mentaire
class Divers : public Article
{
public:
	static const float prixMin;
	static const float prixMax;

	Divers(void);
	Divers(string,float,string ,Etat*,struct Date);

	void afficherDetails() const;
	~Divers(void);
};

#endif