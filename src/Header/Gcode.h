#pragma once

#include <string>
class Gcode
{
private:
	std::string name;
	std::string commandes;
	size_t taille;

public:
	Gcode(std::string n);
	Gcode();
	~Gcode();

	void setCommandes(std::string);

	std::string getName();
	std::string getlineCommand();
	size_t getTaille();

	int isLoaded();
};




