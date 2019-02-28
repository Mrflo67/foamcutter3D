/**
*	FILE APPLICATION.CPP
*	MAIN FILE OF THE PROJECT
*	PROJECT BTS SN 2019 - FOAM CUTTER




/*C Libraries*/
#include <stdio.h>
#include <stdlib.h>

#include "pch.h"

/* Personal Libraries */
#include "Header/Headers.h"
#include "Header/Struct.h"

#ifndef _WIN32 /* Set the parameter for windows 32 bit */
#define APIENTRY __stdcall
#endif // !__WIN32

#ifndef NOMINMAX
#define NOMINMAX
#endif

int main(void)
{
	/////// GCODE LOADING ////////
	SelecteurFichier sf;
	std::string filename = sf.select();

	if (filename == "")
		exit(EXIT_SUCCESS);

	Gcode gcode(filename);
	GcodeLoader gcl;
	gcl.load(gcode);

	if (gcode.isLoaded() == 0)
		exit(EXIT_SUCCESS);

	//recentPath();

	/////// SIMULATION ////////
		
	/* Retreive the screen resolution in pixel */
	getScreenResolution();

	//Creation of the scene
	SceneOpenGL scene("FoamCutter Simulation", info.WINDOW_WIDTH * 0.9, info.WINDOW_HEIGTH * 0.9);

	//Initialisation of the scene
	if (scene.initWindow() == false)
		return -1;

	if (scene.initGL() == false)
		return -1;

	if (scene.initImGUI() == false)
		return -1;

	//Main loop
	scene.mainLoop();


	//End of program

	return 0;
}