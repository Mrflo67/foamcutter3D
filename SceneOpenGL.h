#pragma once

/*C Libraries*/
#include <stdio.h>
#include <stdlib.h>

/* C++ Libraries */
#include <iostream>
#include <string>

/* OpenGL Libraries */
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/* ImGUI Libraries */
#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_glfw_gl3.h>



class SceneOpenGL
{
public:

	SceneOpenGL(std::string windowTitle, int windowWidth, int windowHeigth);
	~SceneOpenGL();

	bool initWindow();
	bool initGL();
	bool initImGUI();
	void mainLoop();


private:

	std::string m_windowTitle;
	int m_windowWidth;
	int m_windowHeigth;

	GLFWwindow* m_window;
};

