#include <string>
#include <fstream> //Pour les op�rations dans un fichier
#include <vector>

#include "Fichier.h"
#include "ExceptionMarche.h"

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
string Fichier::getContenuFormatePourPourEcriture(vector<vector<string>>& poEntrees)
{
	string lsContenu = "";
	for each (vector<string> loEntree in poEntrees)
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

/// <summary>Renvoie le contenu du fichier. Chaque ligne sera un vecteur de cha�nes, dont chaque valeur entre �;� sera une cha�ne.</summary>
/// <param name='psNomCompletFichier'>Chemin et nom du fichier, ne pas indiquer l'extension (.txt seulement pour l'instant)</param>
vector<vector<string>> Fichier::getContenu(const string& psNomCompletFichier)
{
	vector<vector<string>>& loEntrees = vector<vector<string>>();

	// Ouverture du fichier
	fstream loFStream(psNomCompletFichier + ".txt", ios::in);
	if (!loFStream.is_open())
	{
		throw ExceptionMarche(string("Le fichier �" + psNomCompletFichier + "� n'a pas pu �tre ouvert"), false);
		return loEntrees;
	}

	// Lecture de chaque ligne du fichier
	string ligne;
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
		loEntrees.insert(loEntrees.end(), loEntree);
	}
	return loEntrees;
}

///<summary>Remplace le contenu d'un fichier par les entr�es fournies en param�tre.</summary>
bool Fichier::setContenu(string& psNomCompletFichier, vector<vector<string>>& poEntrees)
{
	fstream loFStream(psNomCompletFichier, ios::app);
	if (!loFStream.is_open())
	{
		throw ExceptionMarche(string("Le fichier �" + psNomCompletFichier + "� n'a pas pu �tre ouvert"), false);
		return false;
	}

	loFStream << getContenuFormatePourPourEcriture(poEntrees);
	loFStream.close();
	return true;
}

///<summary>Retourne �true� si le compte existe, false sinon.</summary>
bool Fichier::fichierExistant(string& psNomCompletFichier)
{
	// ATTENTION: BUG!!!  CR�E LE FICHIER S'IL N'EXISTE PAS ET RENVOIE TOUJOURS VRAI
	fstream loFStream(psNomCompletFichier, ios::app);
	if (!loFStream.is_open())
		return false;

	loFStream.close();
	return true;
}