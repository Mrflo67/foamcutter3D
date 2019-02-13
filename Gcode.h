#pragma once

#include <string>
class Gcode
{
private:
	std::string commandes;
	size_t taille;

public:
	Gcode();
	~Gcode();
	std::string getCommandes();
	size_t getTaille();

	void setCommandes(std::string s);
};

