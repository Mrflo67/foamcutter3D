/**
*	FILE APPLICATION.CPP
*	MAIN FILE OF THE PROJECT
*	PROJECT BTS SN 2019 - FOAM CUTTER
*/

#include <iostream>
#include <string>

#include "Struct.h"
#include "SelecteurFichier.h"
#include "Gcode.h"
#include "GcodeLoader.h"
#include "SceneOpenGL.h"

#include "ImGui_Menu_Windows.h"


#ifndef _WIN32 /* Set the parameter for windows 32 bit */
#define APIENTRY __stdcall
#endif // !__WIN32

#ifndef NOMINMAX
#define NOMINMAX
#endif

int main()
{
	/////// GCODE LOADING ////////
	SelecteurFichier sf;
	std::string filename = sf.select();

	
	if (filename == "")
		return 0;

	Gcode gcode(filename);
	
	
	GcodeLoader gcl;
	gcl.load(gcode);

	
	if (gcode.isLoaded() == 0)
		return 0;

	/////// SIMULATION ////////
		
	
	getScreenResolution();

	//Creation of the OpenGL scene
	SceneOpenGL scene("FoamCutter Simulation", info.WINDOW_WIDTH*0.9, info.WINDOW_HEIGHT*0.9);

	

	//Initialisation of the scene
	if (scene.initWindow() == false)
		return -1;

	if (scene.initGL() == false)
		return -1;

	if (scene.initImGUI() == false)
		return -1;


	//Main loop
	scene.mainLoop();

	/**
	//End of program
	*/
	return 0;
}