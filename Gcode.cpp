#include "Header/Gcode.h"


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
	int loadState = 0;

	if (this->taille > 0)
		loadState = 1;

	return loadState;
}

void Gcode::setCommandes(std::string cmd)
{
	if (this->taille == 0)
	{
		this->commandes = cmd;
		taille = cmd.size();
	}

}
