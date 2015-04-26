#ifndef JOURNEE_H
#define JOURNEE_H

//Une structure pour stocker les dates de manière simple
#include <string>

using namespace std;

struct Journee
{
	int nbrArticlesVendus;
	int nbrArticlesAchetes;
	float totalVentes;
	float totalAchats;
};

#endif