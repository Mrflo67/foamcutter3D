#pragma once

#include "Gcode.h"
#include "Cube.h"
#include "Fil.h"


#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Simulation
{
public:
	Simulation();
	~Simulation();
	bool ChargerGcode();


	Cube *m_cube;
	Fil *m_fil;

	void BindObjects(Cube *cube, Fil *fil);

	void Demarrer();
	void Arreter();
	int isRunning();
	int SimulerDecoupe(float vitesse, glm::mat4 &modelMatrix);

private:

	Gcode m_gcode;
	int m_etat; //0 = stopped, 1=started
	float m_vitesseSimulation;
	int m_vitesseDecoupe;
	int m_absolu;
	int m_priseOrigineMachine;
	int m_priseOrigineProgramme;

	std::string m_currentCmd;
	float m_optionsCmd[5]; // [5] = {X, Y, U, V, B}
	int m_finCmd;

	int AnalyzeCmd();
	int ExecuteCmd(glm::mat4 &modelMatrix);

};

