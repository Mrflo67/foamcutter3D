#pragma once

#include "Gcode.h"


class Simulation
{
public:
	Simulation();
	~Simulation();
	bool ChargerGcode();
	void simuler();

private:

	Gcode m_gcode;

	int m_etat;
	float m_vitesse;


};

