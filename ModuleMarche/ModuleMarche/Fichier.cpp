#include <string>
#include <fstream> //Pour les op�rations dans un fichier
#include <vector>

#include "Fichier.h"

using namespace std;

const char Fichier::CC_VALUE_SEPARATOR = ';';
const string Fichier::CC_LINE_SEPARATOR = "\n";

Fichier::Fichier(void)
{
}

Fichier::~Fichier(void)
{
}

///<summary>Pr�pare la mise en forme des donn�es avant l'�criture dans un fichier. (Valeurs s�par�es par des �;�, entr�es sur des lignes diff�rentes, etc.)</summary>
string Fichier::getContenuFormatePourPourEcriture(vector<vector<string>>* poEntrees)
{
	if (poEntrees == nullptr)
		return "";

	string lsContenu = "";
	for each (vector<string> loEntree in *poEntrees)
	{
		for each (string lsParametre in loEntree)
		{
			lsContenu += lsParametre + CC_VALUE_SEPARATOR;
		}

		// � la fin de la ligne du fichier, on remplace le dernier caract�re de s�paration de valeurs �;� par une caract�re de fin de ligne �\n�
		if (lsContenu[lsContenu.length() - 1] = CC_VALUE_SEPARATOR)
		{
			lsContenu.replace(lsContenu.end(), lsContenu.end(), CC_LINE_SEPARATOR);
		}
	}
	return lsContenu;
}

///<summary>Renvoie le contenu du fichier. Chaque ligne sera un vecteur de cha�nes, dont chaque valeur entre �;� sera une cha�ne.</summary>
vector<vector<string>>* Fichier::getContenu(string psNomCompletFichier)
{
	// Ouverture du fichier
	fstream loFStream(psNomCompletFichier + ".txt", ios::in);
	if (!loFStream.is_open())
		return nullptr;

	// Lecture de chaque ligne du fichier
	string ligne;
	vector<vector<string>>* loEntrees = new vector<vector<string>>();
	vector<string> loEntree;
	while (getline(loFStream, ligne) && ligne.length() != NULL)
	{
		// Lecture d'une ligne
		unsigned int nbPtsVirgs = 0;
		loEntree = vector<string>();
		for (size_t cpt = 0; cpt < ligne.length(); cpt++)
		{
			if (ligne[cpt] == CC_VALUE_SEPARATOR)
			{
				nbPtsVirgs++;
			}
			else
			{
				if (loEntree.size() <= nbPtsVirgs)
					loEntree.insert(loEntree.end(), "");

				loEntree.at(nbPtsVirgs) += ligne[cpt];
			}
		}
		loEntrees->insert(loEntrees->end(), loEntree);
	}
	return loEntrees;
}

///<summary>Remplace le contenu d'un fichier par les entr�es fournies en param�tre.</summary>
bool Fichier::setContenu(string psNomCompletFichier, vector<vector<string>>* poEntrees)
{
	fstream loFStream(psNomCompletFichier, ios::app);
	if (!loFStream.is_open())
		return false;

	loFStream << getContenuFormatePourPourEcriture(poEntrees);
	loFStream.close();
	return true;
}