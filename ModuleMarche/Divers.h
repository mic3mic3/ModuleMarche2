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
	Divers(void);
	Divers(string,float,string ,string,struct Date);

	void afficherDetails() const;
	~Divers(void);
};

#endif