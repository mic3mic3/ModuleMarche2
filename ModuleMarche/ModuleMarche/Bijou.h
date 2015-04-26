#ifndef BIJOU_H
#define BIJOU_H

#include <string>

#include "Article.h"

using std::string;

struct Date;
//Hérite d'Article et possède deux nouveaux attributs
class Bijou : public Article
{
private:
	string materiau;
	int purete;
public:
	static const float PRIX_MIN;
	static const float PRIX_MAX;

	Bijou(void);
	Bijou(string,float,string ,Etat*,struct Date,string,int);
	~Bijou(void);

	const string afficherDetails() const;

	float getPrixEtat() const;

	string getMateriau() const;
	int getPurete() const;
};

#endif
