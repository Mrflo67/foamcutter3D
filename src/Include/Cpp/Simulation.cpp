#include "Simulation.h"
#include "Cube.h"
#include "Fil.h"
#include "Struct.h"
#include "SelecteurFichier.h"
#include "Gcode.h"
#include "Mesh.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>

#define _X 0
#define _Y 1
#define _U 2
#define _V 3
#define _B 4

#define VITESSE_MAX_DECOUPE 10000.0f
#define VITESSE_MAX_SIMULATION 10.0f


Simulation::Simulation() : m_etat(0), m_absolu(1), m_finCmd(1), m_reset(1),
m_priseOrigineMachine(0), m_priseOrigineProgramme(0), m_cube(NULL), m_fil(NULL), m_cutSurface(NULL),
m_vitesseDecoupe(VITESSE_MAX_DECOUPE), m_vitesseSimulation(0), m_gcodeLoaded(0), m_gcode(""),
m_currentCmd(""), m_currentLineNb(0), m_moveCmd(false)
{
}


Simulation::~Simulation()
{
}

int Simulation::SimulerDecoupe(float vitesse, float framerate)
{
	m_vitesseSimulation = vitesse;
	
	if (m_finCmd)
	{
		if (!m_gcodeLoaded)
		{
			m_etat = 0;
			return 0;
		}

		m_currentCmd = m_gcode.getlineCommand(m_reset);
		m_currentLineNb++;

		m_priseOrigineMachine = 0;
		m_priseOrigineProgramme = 0;

		if (m_reset)
		{
			m_currentLineNb = 1;
			m_reset = false;
		}

		NextCmd();
	}

	if (m_vitesseSimulation > 0)
	{
		if (m_moveCmd)
			m_finCmd = MoveObjects(framerate);
		else
			m_finCmd = 1;
	}
	else
	{
		m_finCmd = 0;
	}

	return 1;
}


int Simulation::NextCmd()
{
	m_moveCmd = false;
	std::stringstream ssCmd;
	std::string sCmd("");
	char cmdLetter='G';
	float cmdValue=0.0f;

	m_fil->getCurrentPos(m_optionsCmd);
	m_optionsCmd[_B] = m_cube->getRotationDeg();

	ssCmd << m_currentCmd;

	while (ssCmd >> sCmd)
	{
		cmdLetter = sCmd[0];
		sCmd.erase(0,1);
		
		if (sCmd.length() > 0)
		{
			cmdValue = std::stof(sCmd);
		}
		else
		{
			cmdValue = 0.0f;
		}
		
	
		int axisID=-1;

		switch (cmdLetter)
		{
		case 'G': {
			switch ((int)cmdValue)
			{
			case 0: m_vitesseDecoupe = VITESSE_MAX_DECOUPE;
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
			case 2: Arreter(); m_reset = 1;
				m_cutSurface->genVertexNormals();
				m_cutSurface->load = 1;
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

		case 'P': break; 
		case 'S': break; 
		case 'X': axisID = _X; break;
		case 'Y': axisID = _Y; break;
		case 'U': axisID = _U; break;
		case 'V': axisID = _V; break;
		case 'B': axisID = _B; break;
		case 'F': m_vitesseDecoupe = cmdValue;
			break;

		default: std::cout << "Commande inconnue, passage à la suivante" << std::endl;
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
					float currentPos[5];
					m_fil->getCurrentPos(currentPos);
					currentPos[4] = m_cube->getRotationDeg();
					m_optionsCmd[axisID] = currentPos[axisID] + cmdValue;	
				}
			}

			m_moveCmd = true;
		}
		sCmd.clear();
	}

	return 1;
}



int Simulation::MoveObjects(float framerate)
{
	static float posDebut[5] = { 0.0f };
	static float posFin[5] = { 0.0f };
	static float pos[5] = { 0.0f };
	float pas = 0.0f;
	int endCmd = 0;
	int fini = 0;
	static std::array<float, 3> v0{ {0.0f, 0.0f, m_fil->posZ_XY} }; //debut XY
	static std::array<float, 3> v1 = { {0.0f, 0.0f, m_fil->posZ_UV } }; //debut UV
	static std::array<float, 3> v2 = v0; //fin XY
	static std::array<float, 3> v3 = v1; //fin UV
	static int ecartCentre = m_fil->getEcartX();

	float posSurface[4];

	if (m_finCmd)
	{

		m_fil->getInterFoamPos(pos, m_cube->getLargeur());


		//vertices positions array for the cutter surface 
		m_fil->getCurrentPos(posDebut);
		posDebut[_B] = 0;

		for (int i = _X; i < _B+1; i++)
		{
			posFin[i] = m_optionsCmd[i];
		}

		m_fil->getInterFoamPos(posSurface, m_cube->getLargeur());

		v0[0] = posSurface[_X] - ecartCentre;
		v0[1] = posSurface[_Y];
		//v0[2] = m_cube->getLargeur() / 2;
		v1[0] = posSurface[_U]- ecartCentre;
		v1[1] = posSurface[_V];
		//v1[2] = - (m_cube->getLargeur() / 2);

	}


	m_fil->getCurrentPos(pos);
	pos[_B] = m_cube->getRotationDeg();

	fini = 0;
	

	for (int i = _X; i < _B+1; i++)
	{
		pas = ((m_vitesseSimulation > VITESSE_MAX_SIMULATION) * fabs(posFin[i] - posDebut[i])) + 
			m_vitesseSimulation *
			(m_vitesseDecoupe / VITESSE_MAX_DECOUPE) * (VITESSE_MAX_DECOUPE / 60.0f / framerate);

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
	//std::cout << fini << std::endl;
	if (fini == 5)
	{
		endCmd = 1;
	
		m_fil->majPos(pos[_X], pos[_Y], pos[_U], pos[_V]);
		m_fil->getInterFoamPos(posSurface, m_cube->getLargeur());

		v2[0] = posSurface[_X] - ecartCentre;
		v2[1] = posSurface[_Y];
		//v2[2] = v0[2];
		v3[0] = posSurface[_U] - ecartCentre;
		v3[1] = posSurface[_V];
		//v3[2] = v1[2];
		unsigned int offset = m_cutSurface->m_vertices.size();

		float longueurCube = m_cube->getLongueur();
		float limitInfX = -longueurCube / 2  + m_cube->getPosX();
		float limitSupX =  m_cube->getPosX() + longueurCube / 2 ;

		float hauteurCube = m_cube->getHauteur();
		

		/*if ((v0[0] > limitInfX || v2[0] > limitInfX) && (v1[0] < limitSupX || v3[0] < limitSupX) && 
			(v0[1] < hauteurCube || v1[1] < hauteurCube || v2[1] < hauteurCube || v3[1] < hauteurCube))
		{

			m_cutSurface->m_vertices.push_back(v0);
			m_cutSurface->m_vertices.push_back(v1);
			m_cutSurface->m_vertices.push_back(v2);
			m_cutSurface->m_vertices.push_back(v3);
			m_cutSurface->m_indices.push_back(offset);
			m_cutSurface->m_indices.push_back(offset + 2);
			m_cutSurface->m_indices.push_back(offset + 1);
			m_cutSurface->m_indices.push_back(offset + 1);
			m_cutSurface->m_indices.push_back(offset + 2);
			m_cutSurface->m_indices.push_back(offset + 3);
		}*/

		m_cutSurface->m_vertices.push_back(v0);
		m_cutSurface->m_vertices.push_back(v1);
		m_cutSurface->m_vertices.push_back(v2);
		m_cutSurface->m_vertices.push_back(v3);
		m_cutSurface->m_indices.push_back(offset + 1);
		m_cutSurface->m_indices.push_back(offset + 0);
		m_cutSurface->m_indices.push_back(offset + 3);
		m_cutSurface->m_indices.push_back(offset + 0);
		m_cutSurface->m_indices.push_back(offset + 2);
		m_cutSurface->m_indices.push_back(offset + 3);

		return endCmd;
	}
	
	//m_cube->rotationY(pos[_B]);

	m_fil->majPos(pos[_X], pos[_Y], pos[_U], pos[_V]);
	

	return endCmd;
}

int Simulation::isRunning()
{
	return m_etat==1;
}

void Simulation::BindObjects(Foam &cube, Fil &fil, Mesh &cutSurface)
{
	m_cube = &cube;
	m_fil = &fil;
	m_cutSurface = &cutSurface;
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

int Simulation::getCurrentLineNb()
{
	return m_currentLineNb;
}

int Simulation::Init()
{
	m_reset = true; 
	m_finCmd = 1;
	m_absolu = 1;
	m_etat = 0;
	m_currentCmd.clear();
	m_currentLineNb = 0;

	if (m_cube != NULL && m_fil != NULL)
	{
		//m_cube->rotationY(0.0f);
		m_fil->majPos(0.0f, 0.0f, 0.0f, 0.0f);
		m_fil->trajectory.m_vertices.clear();
		m_fil->trajectory.m_indices.clear();
	}

	if (m_cutSurface != NULL)
	{
		m_cutSurface->m_indices.clear();
		m_cutSurface->m_vertices.clear();
		m_cutSurface->m_normals.clear();
	}

	return 1;
}

int Simulation::hasGcode()
{
	return m_gcodeLoaded;
}

bool Simulation::ChargerGcode(std::string filename)
{
	m_gcodeLoaded = 0;

	if (filename == "")
	{
		
		return 0;
	}
	else
	{
		Gcode gcode(filename);
		m_gcode = gcode;

		if(m_gcode.isLoaded())
		m_gcodeLoaded = 1;

		content.filePath = filename;
		content.fileName = filename;
		content.commands = std::string(gcode.getCommandes());
	}

	return true;
}