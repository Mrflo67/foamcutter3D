/**
*	FILE APPLICATION.CPP
*	MAIN FILE OF THE PROJECT
*	PROJECT BTS SN 2019 - FOAM CUTTER
*/

#include <iostream>
#include <string>

#include "SelecteurFichier.h"
#include "Gcode.h"
#include "GcodeLoader.h"
#include "SceneOpenGL.h"
#include "Simulation.h"


#ifndef _WIN32 /* Set the parameter for windows 32 bit */
#define APIENTRY __stdcall
#endif // !__WIN32

#ifndef NOMINMAX
#define NOMINMAX
#endif

int main()
{
	
	SceneOpenGL scene("FoamCutter Simulation");

	if (!scene.initSimu())
		return 0;

	if (!scene.initWindow())
		return 0;

	if (!scene.initGL())
		return 0;

	if (!scene.initImGUI())
		return 0;
	
	scene.mainLoop();

	return 0;
}