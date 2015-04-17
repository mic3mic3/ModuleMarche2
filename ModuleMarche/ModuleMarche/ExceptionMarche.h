#ifndef EXCEPTION_MARCHE_H
#define EXCEPTION_MARCHE_H

#include <string>

using std::string;

class ExceptionMarche
{
private:
	string message;
	bool fatale;

public:
	ExceptionMarche(string&, bool);

	string getMessage() const;
	bool getFatale() const;

	~ExceptionMarche(void);

	void Gerer() const;
};

#endif