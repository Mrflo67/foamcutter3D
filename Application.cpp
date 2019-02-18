/**
*	FILE APPLICATION.CPP
*	MAIN FILE OF THE PROJECT
*	PROJECT BTS SN 2019 - FOAM CUTTER
*/

/* OpenGL Libraries */
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/* ImGUI Libraries */
#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_glfw_gl3.h>

/*Math libraries*/



/*C Libraries*/
#include <stdio.h>
#include <stdlib.h>

/* C++ Libraries */
#include <iostream>
#include <fstream>
#include <thread>

#ifndef __win32__
#include <Windows.h>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif // !__win32__


/* Personal Libraries */
#include "Header/ReadGCode.h"
#include "Header/Renderer.h"
#include "Header/Thread.h"
#include "Header/ImGui_Menu.h"
#include "Header/SelecteurFichier.h"
#include "Header/Gcode.h"
#include "Header/GcodeLoader.h"


/* DEFINE */
const int WINDOW_WIDTH = 1600;
const int WINDOW_HEIGTH = 900;

/* Global variable */
bool checkDialog = true;
bool checkReadGCode = true;
bool waitSelectPath = false;
std::string filePath;
extern std::string line;

/* Functions */
void openGCode();
void readGCode();
void MenuFile();




int main(void)
{
	SelecteurFichier sf;
	std::string filename = sf.select();
	if (filename == "")
	{
		exit(EXIT_SUCCESS);
	}
	
	Gcode gcode(filename);
	GcodeLoader gcl;
	gcl.load(gcode);

	if (!gcode.isLoaded())
	{
		exit(EXIT_SUCCESS);
	}
	
	
	std::cout << filename << std::endl;

	//lancement simu 3D






	

	return 0;
}
