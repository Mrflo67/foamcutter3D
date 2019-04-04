#pragma once
#include "Gcode.h"
#include "Cube.h"
#include "Fil.h"

class Simulation
{
public:
	Simulation();
	~Simulation();
	
	void BindObjects(Cube *cube, Fil *fil);
	void Demarrer();
	void Arreter();
	int Init();
	int isRunning();
	int hasGcode();
	int SimulerDecoupe(float vitesse, float framerate);
	bool ChargerGcode(std::string filename);
	std::string getCurrentCmd();

	Cube *m_cube;
	Fil *m_fil;

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
	std::string m_currentCmd;
	
	int AnalyzeCmd();
	int ExecuteCmd(float framerate);

};

