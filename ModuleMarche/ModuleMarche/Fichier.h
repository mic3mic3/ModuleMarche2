#ifndef FICHIER_H
#define FICHIER_H

#include <string>
#include <vector>

using std::string;
using std::vector;

///<summary>Gestion des entrées/sorties de l'application via des fichiers.</summary>
class Fichier
{
public:
	static const char CC_VALUE_SEPARATOR;
	static const string CC_LINE_SEPARATOR;

	Fichier(void);
	~Fichier(void);


	static string getContenuFormatePourPourEcriture(vector<vector<string>>&);
	static vector<vector<string>> getContenu(const string&);
	static bool setContenu(string&, vector<vector<string>>&);
	static bool fichierExistant(string&);
};

#endif
