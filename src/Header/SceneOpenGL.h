#pragma once

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Struct.h"
#include "ImGui_Menu_Windows.h"
#include "Simulation.h"


class SceneOpenGL
{
public:

	SceneOpenGL(std::string windowTitle, int width = 1280, int height = 720);
	~SceneOpenGL();

	bool initWindow();
	bool initGL();
	bool initImGUI();
	bool initSimu();
	void mainLoop();


private:

	std::string m_windowTitle;
	int m_windowWidth;
	int m_windowHeight;
	float m_ratio;

	GLFWwindow* m_window;
	Simulation m_simu;
};

