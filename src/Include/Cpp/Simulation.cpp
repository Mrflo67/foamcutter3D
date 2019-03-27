#include "Simulation.h"
#include "Cube.h"
#include "Fil.h"
#include "Struct.h"
#include "SelecteurFichier.h"
#include "Gcode.h"
#include "GcodeLoader.h"

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

#define VITESSE_RAPIDE 10000.0f

Simulation::Simulation() : m_etat(0), m_absolu(1), m_finCmd(1),
m_priseOrigineMachine(0), m_priseOrigineProgramme(0), m_cube(NULL), m_fil(NULL),
m_vitesseDecoupe(VITESSE_RAPIDE), m_vitesseSimulation(0)
{
}


Simulation::~Simulation()
{
}

int Simulation::SimulerDecoupe(float vitesse, float framerate, glm::mat4 &modelMatrix)
{

	m_vitesseSimulation = vitesse;
	

	if (m_finCmd)
	{
		m_currentCmd = m_gcode.getlineCommand(m_reset);

		m_priseOrigineMachine = 0;
		m_priseOrigineProgramme = 0;

		if (m_reset)
			m_reset = false;

		AnalyzeCmd();
		
	}

	if (m_vitesseSimulation > 0)
	{
		m_finCmd = ExecuteCmd(modelMatrix, framerate);		
	}
	else
	{
		m_finCmd = 0;
	}

	return 1;
}


int Simulation::AnalyzeCmd()
{
	std::stringstream ssCmd;
	std::string sCmd("");
	char cmdLetter='G';
	float cmdValue=0.0f;

	m_fil->getCurrentPos(m_optionsCmd);
	m_optionsCmd[4] = 0.0f;

	ssCmd << m_currentCmd;

	while (ssCmd >> sCmd)
	{
		if (sCmd.rfind("end") != std::string::npos)
		{
			m_etat = 0;
			std::cout << "Arrive au bout du fichier, pas de m2 trouve, arret simulation" << std::endl;
			m_reset = 1;
			return 1;
		}

		cmdLetter = sCmd[0];
	
		sCmd.erase(0,1);
		
		if (sCmd.length() > 0)
		{
			cmdValue = std::stof(sCmd);
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
			case 92: m_priseOrigineProgramme = 1;
				break;
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
		case 'F': m_vitesseDecoupe = cmdValue;
			break;

		default: std::cout << "Commande inconnue, passage � la suivante" << std::endl;
		}
	
		if (axisID >= 0)
		{
			if (m_priseOrigineMachine)
			{
				m_optionsCmd[0] = m_optionsCmd[1] = m_optionsCmd[2]
					= m_optionsCmd[3] = m_optionsCmd[4] = 0.0f;

			}
			else if (m_priseOrigineProgramme)
			{
				//m_optionsCmd[axisID] = cmdValue;
					//todo : changer la valeur de l'origine
			}
			else
			{
				if (m_absolu)
				{
					m_optionsCmd[axisID] = cmdValue;

				}else
				{
					float currentPos[4];
					m_fil->getCurrentPos(currentPos);
					m_optionsCmd[axisID] = currentPos[axisID] + cmdValue;	
				}
			}
		}
		sCmd.clear();
	}

	return 1;
}



int Simulation::ExecuteCmd(glm::mat4 &modelMatrix, float framerate)
{
	static float posDebut[5] = { 0.0f };
	static float posFin[5] = { 0.0f };
	static float pos[5] = { 0.0f };
	float pas = 0;
	int endCmd = 0;
	int fini = 0;

	if (m_finCmd)
	{
		m_fil->getCurrentPos(posDebut);
		for (int i = _X; i < _B+1; i++)
		{
			posFin[i] = m_optionsCmd[i];
		}
	}


	m_fil->getCurrentPos(pos);

	fini = 0;

	for (int i = _X; i < _B+1; i++)
	{
		pas = m_vitesseSimulation * (m_vitesseDecoupe / VITESSE_RAPIDE) * (VITESSE_RAPIDE / 60. / framerate);
		
		if (posDebut[i] < posFin[i])
		{
			pos[i] += pas;
			if (pos[i] > posFin[i])
			{
				pos[i] = posFin[i];
				fini++;
			}	
		}
		else if(posDebut[i] > posFin[i])
		{
			pos[i] -= pas;
			if (pos[i] < posFin[i])
			{
				pos[i] = posFin[i];
				fini++;
			}
		}
		else
		{
			fini ++;
		}
	}

	std::cout << "fini : " << fini << std::endl;

	if (fini >= 4)
		endCmd = 1;
	//std::cout << "fini : " << fini << std::endl;
		

	m_cube->rotationY(pos[4]);

	m_fil->majPos(pos[_X], pos[_Y], pos[_U], pos[_V]);
	
	return endCmd;
}

int Simulation::isRunning()
{
	return m_etat==1;
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

std::string Simulation::getCurrentCmd()
{
	return m_currentCmd;
}

int Simulation::Init()
{

	m_reset = true; 
	m_finCmd = 1;
	m_absolu = 1;
	m_etat = 0;

	if (m_cube != NULL && m_fil != NULL)
	{
		m_cube->rotationY(0.0f);
		m_fil->majPos(0.0f, 0.0f, 0.0f, 0.0f);
	}

	return 1;
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