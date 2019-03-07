#pragma once

#include <string>




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
	float m_ratio;

	GLFWwindow* m_window;
};

