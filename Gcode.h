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

	std::string getName();
	std::string getCommandes();
	void setCommandes(std::string);
	size_t getTaille();
	int isLoaded();
};




