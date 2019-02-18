#include "Header/Gcode.h"


Gcode::Gcode(std::string n)
{
	this->name = n;
	this->commandes = "";
	this->taille = 0;
}

Gcode::~Gcode()
{}

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
