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
	static const float PRIX_MIN;
	static const float PRIX_MAX;

	Divers(void);
	Divers(string,float,string ,Etat*,struct Date);

	float getPrixEtat() const;
	void afficherDetails() const;
	~Divers(void);
};

#endif