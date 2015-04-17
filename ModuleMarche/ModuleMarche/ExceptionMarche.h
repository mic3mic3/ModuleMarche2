#ifndef EXCEPTION_MARCHE_H
#define EXCEPTION_MARCHE_H

#include <string>

using std::string;

class ExceptionMarche
{
private:
	string message;
public:
	ExceptionMarche(string&);

	string getMessage() const;

	~ExceptionMarche(void);
};

#endif