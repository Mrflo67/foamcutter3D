#pragma once

#include <string>

class Gcode
{
private:
	std::string m_name;
	std::string m_commandes;
	size_t m_taille;

public:
	Gcode(std::string n);
	~Gcode();

	int isLoaded();
	size_t getSize();
	std::string getName();
	std::string getlineCommand(bool reset);
	std::string getCommandes();

};




