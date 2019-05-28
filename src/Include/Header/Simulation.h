/**
*	FILE SIMULATION.H
*	SIMULATION CLASS DECLARATION
*	PROJECT BTS SN 2019 - FOAM CUTTER
*/

#pragma once
#include "Gcode.h"
#include "Fil.h"
#include "Mesh.h"
#include "Base.h"
#include "Foam.h"


class Simulation
{
public:
	Simulation();
	~Simulation();

	void BindObjects(Foam & cube, Fil & fil, Mesh & cutSurface);
	void Demarrer();
	void Arreter();
	int Init();
	int isRunning();
	int hasGcode();
	int SimulerDecoupe(float vitesse, float framerate);
	bool ChargerGcode(std::string filename);
	void setBaseAngleInit(float r);
	std::string getCurrentCmd();
	int getCurrentLineNb();

	Foam *m_cube;
	Fil *m_fil;
	Mesh *m_cutSurface;
	

private:

	Gcode m_gcode;
	int m_gcodeLoaded;
	int m_etat; //0 = stopped, 1=started
	int m_absolu;
	int m_priseOrigineMachine;
	int m_priseOrigineProgramme;
	int m_finCmd;
	float m_vitesseSimulation;
	float m_vitesseDecoupe;
	float m_optionsCmd[5]; // [5] = {X, Y, U, V, B}
	bool m_reset;
	bool m_moveCmd;
	std::string m_currentCmd;
	int m_currentLineNb;

	float m_baseAngleInit;	//angle in radians
	
	int NextCmd();
	int MoveObjects(float framerate);

};

