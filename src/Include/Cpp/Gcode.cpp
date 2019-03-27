#include "Gcode.h"
#include <iostream>
#include <string>
#include <sstream>



Gcode::Gcode(std::string n) : name(n), commandes(""), taille(0)
{}

Gcode::Gcode()
{}

Gcode::~Gcode()
{}


std::string Gcode::getName()
{
	return this->name;
}

std::string Gcode::getlineCommand(bool reset)
{
	std::string sCmdline = std::string("");
	static unsigned int i = this->taille;

	if (reset)
	{
		i = 0;
		sCmdline = std::string("");
	}

	if (i >= taille)
		i = 0;
	if (commandes[i] == '\n')
		i++;

	while (commandes[i] != '\n')
	{
		sCmdline += commandes[i];
		i++;
		if (i >= taille)
		{
			sCmdline.append(" end\n");
			break;
		}
			
	}

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
	
	commandes = cmd;
	taille = cmd.size();

}
