#include "Header/Gcode.h"

Gcode::Gcode(std::string cmd)
{
	this->commandes=cmd;
	this->taille = strlen(cmd.c_str());
}

Gcode::~Gcode()
{
}

std::string Gcode::getCommandes()
{
	return this->commandes;
}

unsigned int Gcode::getTaille()
{
	return this->taille;
}

