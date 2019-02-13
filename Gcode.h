#pragma once

#include <string>
class Gcode
{
private:
	std::string commandes;
	size_t taille;

public:
	Gcode(std::string cmd);
	~Gcode();
	std::string getCommandes();
	size_t getTaille();
};

