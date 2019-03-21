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
	static int i = 0;
	std::string cmd("");
	char c;
	
	if ((size_t)i < this->taille)
	{
		c = this->commandes.at(i);
		do {
			cmd += c;
			i++;
			c = this->commandes.at(i);
		} while (c != '\n');
		i++;
	}
	
	return cmd;
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
