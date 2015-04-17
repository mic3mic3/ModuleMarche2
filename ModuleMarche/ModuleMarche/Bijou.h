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
	static const float prixMin;
	static const float prixMax;

	Bijou(void);
	Bijou(string,float,string ,Etat*,struct Date,string,int);
	~Bijou(void);

	void afficherDetails() const;

	float getPrixEtat() const;

	string getMateriau() const;
	int getPurete() const;
};

#endif
