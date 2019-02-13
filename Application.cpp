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


//#define chemin "C:\\Users\\Florian\\Desktop\\FoamCutter\\FoamCutter\\src\\GCODES\\DemoRapideV2.gco"

int main(void)
{
	SelecteurFichier selec;
	Gcode gcode;

	if (selec.selectionner(gcode))
	{
		

		// Initialise GLFW
		if (!glfwInit())
		{
			fprintf(stderr, "Failed to initialize GLFW\n");
			getchar();
			return -1;
		}

		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Open a window and create its OpenGL context
		GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)
		window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGTH, "Foam Cutter", NULL, NULL);
		if (window == NULL) {
			fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
			getchar();
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);

		// Initialize GLEW
		if (glewInit() != GLEW_OK) {
			fprintf(stderr, "Failed to initialize GLEW\n");
			getchar();
			glfwTerminate();
			return -1;
		}

		// Ensure we can capture the escape key being pressed below
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

		// Dark blue background
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		do {
			// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
			glClear(GL_COLOR_BUFFER_BIT);

			// Draw nothing, see you in tutorial 2 !


			// Swap buffers
			glfwSwapBuffers(window);
			glfwPollEvents();

		} // Check if the ESC key was pressed or the window was closed
		while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			glfwWindowShouldClose(window) == 0);

		// Close OpenGL window and terminate GLFW
		glfwTerminate();
	}
	else
	{
		//
	}


	

	return 0;
}
