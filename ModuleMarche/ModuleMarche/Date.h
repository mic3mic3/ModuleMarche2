#ifndef DATE_H
#define DATE_H

//Une structure pour stocker les dates de manière simple
#include <string>

using namespace std;

struct Date
{
	int jour;
	int mois;
	int annee;

	//static Date getDateFromString(string);
	static Date getDateFromString(string psDate)
	{
		struct Date lsDateReturn;
		string jourStr = "";
		string moisStr = "";
		string anneeStr = "";
		int nbSlashs = 0;
		for (size_t cpt = 0; cpt < psDate.length(); cpt++)
		{
			if (psDate[cpt] == '/')
			{
				nbSlashs++;
			}
			else
			{
				switch (nbSlashs)
				{
				case 0:
					jourStr += psDate[cpt];
					break;
				case 1:
					moisStr += psDate[cpt];
					break;
				case 2:
					anneeStr += psDate[cpt];
					break;
				default:
					break;
				}
			}
		}
		lsDateReturn.jour = atoi(jourStr.c_str());
		lsDateReturn.mois = atoi(moisStr.c_str());
		lsDateReturn.annee = atoi(anneeStr.c_str());
		return lsDateReturn;
	}
};

//Date Date::getDateFromString(string psDate)
//{
//	struct Date lsDateReturn;
//	string jourStr = "";
//	string moisStr = "";
//	string anneeStr = "";
//	int nbSlashs = 0;
//	for (size_t cpt = 0; cpt < psDate.length(); cpt++)
//	{
//		if (psDate[cpt] == '/')
//		{
//			nbSlashs++;
//		}
//		else
//		{
//			switch (nbSlashs)
//			{
//			case 0:
//				jourStr += psDate[cpt];
//				break;
//			case 1:
//				moisStr += psDate[cpt];
//				break;
//			case 2:
//				anneeStr += psDate[cpt];
//				break;
//			default:
//				break;
//			}
//		}
//	}
//	lsDateReturn.jour = atoi(jourStr.c_str());
//	lsDateReturn.mois = atoi(moisStr.c_str());
//	lsDateReturn.annee = atoi(anneeStr.c_str());
//	return lsDateReturn;
//}

#endif