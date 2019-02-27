/**
*	FILE APPLICATION.CPP
*	MAIN FILE OF THE PROJECT
*	PROJECT BTS SN 2019 - FOAM CUTTER
*/

/*C Libraries*/
#include <stdio.h>
#include <stdlib.h>

/* C++ Libraries */
#include <iostream>

#ifndef __win32__
#include <windows.h>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif // !__win32__


// Personal Libraries
#include "Header\ReadGCode.h"
#include "Header\Renderer.h"
#include "Header\Thread.h"
#include "Header\ImGui_Menu.h"
#include "Header\SelecteurFichier.h"
#include "Header\Gcode.h"
#include "Header\GcodeLoader.h"
#include "Header\SceneOpenGL.h"



// DEFINES
#define WINDOW_WIDTH 1600
#define WINDOW_HEIGTH 900

// Global variable
bool checkDialog = true;
bool checkReadGCode = true;
bool waitSelectPath = false;
std::string filePath;
extern std::string line;

/* Functions 
void openGCode();
void readGCode();
void MenuFile(); */

using namespace std;


int main()
{ 

	/////// GCODE LOADING ////////
	SelecteurFichier sf;
	std::string filename = sf.select();

	if (filename == "")
		exit(EXIT_SUCCESS);
	
	Gcode gcode(filename);
	GcodeLoader gcl;
	gcl.load(gcode);

	if (gcode.isLoaded()==0)
		exit(EXIT_SUCCESS);
	
	

	//////// SIMULATION ///////////

	//Creation of the scene
	SceneOpenGL scene("FoamCutter Simulation", WINDOW_WIDTH, WINDOW_HEIGTH);

	//Initialisation of the scene
	if (scene.initWindow() == false)
		return -1;

	if (scene.initGL() == false)
		return -1;
	
	//Main loop
	scene.mainLoop();


	//End of program
	return 0;
}
