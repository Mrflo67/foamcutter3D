#pragma once

#include <string>

class Gcode
{
private:
	std::string name;
	std::string commandes;
	size_t taille;

public:
	Gcode();
	Gcode(std::string n);
	~Gcode();

	void setCommandes(std::string);
	int isLoaded();
	size_t getTaille();
	std::string getName();
	std::string getlineCommand(bool reset);

};




