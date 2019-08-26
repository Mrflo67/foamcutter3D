/**
*	FILE APPLICATION.CPP
*	MAIN FILE OF THE PROJECT
*	PROJECT BTS SN 2019 - FOAM CUTTER
*/

#include "SceneOpenGL.h"

int main()
{
	
	SceneOpenGL scene("FoamCutter Simulation");

	if (!scene.initWindow())
		return 0;

	if (!scene.initGL())
		return 0;

	if (!scene.initImGUI())
		return 0;
	
	scene.mainLoop();

	return 0;
}