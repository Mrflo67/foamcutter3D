#include "Simulation.h"
#include "Cube.h"
#include "Fil.h"
#include "Struct.h"
#include "SelecteurFichier.h"
#include "Gcode.h"
#include "GcodeLoader.h"
#include "ImGui_Menu_Windows.h"

#include <fstream>
#include <string>

Simulation::Simulation()
{
}


Simulation::~Simulation()
{
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
	
	recentPath();
	m_gcode = gcode;

	return true;
}