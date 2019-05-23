#pragma once
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Struct.h"

class SceneOpenGL
{
public:

	SceneOpenGL(std::string windowTitle, int width = 1280, int height = 720);
	SceneOpenGL();
	~SceneOpenGL();

	bool initWindow();
	bool initGL();
	bool initImGUI();
	void mainLoop();

private:

	std::string m_windowTitle;
	int m_windowWidth;
	int m_windowHeight;
	float m_ratio;

	GLFWwindow* m_window;

};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);


