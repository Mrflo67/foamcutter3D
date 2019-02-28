#include "pch.h"
#include "Header\Gcode.h"


Gcode::Gcode(std::string n)
{
	this->name = n;
	this->commandes = "";
	this->taille = 0;
}

Gcode::~Gcode()
{}


std::string Gcode::getName()
{
	return this->name;
}

std::string Gcode::getCommandes()
{
	return this->commandes;
}

unsigned int Gcode::getTaille()
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
