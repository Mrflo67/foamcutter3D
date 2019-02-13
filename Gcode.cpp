#include "Header/Gcode.h"


Gcode::Gcode()
{
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

void Gcode::setCommandes(std::string s)
{
	if (this->taille == 0)
	{
		this->commandes = s;
		this->taille = strlen(s.c_str());
	}
}
