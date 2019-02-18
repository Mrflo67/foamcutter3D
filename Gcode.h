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
	~Gcode();
	std::string getCommandes();
	size_t getTaille();
	int isLoaded();
};




