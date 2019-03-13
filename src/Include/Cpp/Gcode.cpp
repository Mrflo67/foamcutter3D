#include "Gcode.h"


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

std::string Gcode::getCommandes()
{
	return this->commandes;
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
