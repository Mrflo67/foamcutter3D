#include "Gcode.h"
#include <iostream>
#include <string>
#include <sstream>



Gcode::Gcode(std::string n) : name(n), commandes(""), taille(0)
{

}

Gcode::Gcode() {}

Gcode::~Gcode()
{}


std::string Gcode::getName()
{
	return this->name;
}

std::string Gcode::getlineCommand()
{
	std::string sCmdline = std::string("");
	static unsigned int i = taille;

	if (commandes[i] == '\n')
		i++;
	if (i >= taille)
		i = 0;

	while (commandes[i] != '\n')
	{
		sCmdline += commandes[i];
		i++;
	}
	//sCmdline += '\n';
	return sCmdline;
}

size_t Gcode::getTaille()
{
	return this->taille;
}



int Gcode::isLoaded()
{

	if (this->taille > 0)
		return 1;

	return 0;
}

void Gcode::setCommandes(std::string cmd)
{
	if (this->taille == 0)
	{
		this->commandes = cmd;
		taille = cmd.size();
	}

}
