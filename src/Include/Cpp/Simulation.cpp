#include "Simulation.h"
#include "Cube.h"
#include "Fil.h"
#include "Struct.h"
#include "SelecteurFichier.h"
#include "Gcode.h"
#include "GcodeLoader.h"
#include "ImGui_Menu_Windows.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



#define _X 0
#define _Y 1
#define _U 2
#define _V 3
#define _B 4

#define VITESSE_RAPIDE 1.0f

Simulation::Simulation() : m_etat(0), m_absolu(1), m_finCmd(1),
m_priseOrigineMachine(0), m_priseOrigineProgramme(0)
{
}


Simulation::~Simulation()
{
}

int Simulation::SimulerDecoupe(float vitesse, glm::mat4 &modelMatrix)
{
	m_vitesseSimulation = vitesse;
	
	if (m_finCmd)
	{
		m_currentCmd = m_gcode.getlineCommand();
		m_priseOrigineMachine = 0;
		m_priseOrigineProgramme = 0;

		AnalyzeCmd();
		
	}

	m_finCmd = ExecuteCmd(modelMatrix);


	return 1;
}


int Simulation::AnalyzeCmd()
{
	std::stringstream ssCmd;
	std::string sCmd("");
	char cmdLetter;
	float cmdValue;

	for (int i = 0; i < 5; i++)
	{
		m_optionsCmd[i] = 0.0f;
	}

	ssCmd << m_currentCmd;

	while (ssCmd >> sCmd)
	{
		std::cout << sCmd << std::endl;

		cmdLetter = sCmd[0];
	
		sCmd.erase(0,1);
		
		if (sCmd.length() > 0)
		{
			cmdValue = std::stoi(sCmd);
		}
	
		int axisID=-1;

		switch (cmdLetter)
		{
		case 'G': {
			switch ((int)cmdValue)
			{
			case 0: m_vitesseDecoupe = VITESSE_RAPIDE;
				break;
			case 1: break;
			case 4: break;
			case 28: m_priseOrigineMachine = 1;
				break;
			case 90: m_absolu = 1;
				break;
			case 91: m_absolu = 0;
				break;
			case 92: m_priseOrigineProgramme = 1; break;
			default: break;
			}
		}break;

		case 'M': {
			switch ((int)cmdValue)
			{
			case 0: break;
			case 1: break;
			case 2: Arreter();
				break;
			case 3: break;
			case 5: break;

			case 45: /*todo*/break;
			case 46: /*todo*/break;
			case 47: /*todo*/break;
			case 48: /*todo*/break;
			default: break;
			}
		}break;

		case 'P': break; //todo
		case 'S': break; //todo

		case 'X': axisID = _X; break;
		case 'Y': axisID = _Y; break;
		case 'U': axisID = _U; break;
		case 'V': axisID = _V; break;
		case 'B': axisID = _B; break;

		default: std::cout << "Commande inconnue, passage � la suivante" << std::endl;
		}
	
		if (axisID >= 0)
		{
			if (m_priseOrigineMachine)
			{
				//todo : mettre les options des axes � leurs valeurs d'origine
				//m_optionsCmd[axisID] = origine.x
			}
			else if (m_priseOrigineProgramme)
			{
				//m_optionsCmd[axisID] = cmdValue;
					//todo : changer la valeur de l'origine
			}
			else
			{
				m_optionsCmd[axisID] = cmdValue;
			}
		}

		
		
		
		sCmd.clear();
	}

	return 1;
}



int Simulation::ExecuteCmd(glm::mat4 &modelMatrix)
{
	m_cube->rotationY(m_optionsCmd[_B]);

	m_fil->majPos(m_optionsCmd[_X], m_optionsCmd[_Y], m_optionsCmd[_U], m_optionsCmd[_V]);


	return 1;
}

int Simulation::isRunning()
{
	return m_etat;
}

void Simulation::BindObjects(Cube *cube, Fil *fil)
{
	m_cube = cube;
	m_fil = fil;

}

void Simulation::Demarrer()
{
	m_etat = 1;
}

void Simulation::Arreter()
{
	m_etat = 0;
}

bool Simulation::ChargerGcode()
{
	SelecteurFichier sf;
	std::string filename = sf.select();

	if (filename == "")
		return 0;

	Gcode gcode(filename);
	GcodeLoader gcl;
	gcl.load(gcode);


	if (!gcode.isLoaded())
		return false;

	content.filePath = filename;
	
	m_gcode = gcode;

	return true;
}