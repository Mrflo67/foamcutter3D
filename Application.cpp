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
	/* DO NOT CHANGE THIS PART */
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#if __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGTH, "Foam Cutter", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	// Setup ImGui binding
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();
	bool displayGcode = false;
	bool show_demo_window = true;

	/* Thread */
	//std::thread t_selectPath(waitSelectPath);
	//std::thread t_readGcode(openGCode);

	/* Verify if glewInit is valid and initilize it */
	if (glewInit() != GLEW_OK)
		std::cout << "Error" << std::endl;
	/**************************/

	std::cout << glGetString(GL_VERSION) << std::endl;
	{
		Renderer renderer;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();
			ImGui_ImplGlfwGL3_NewFrame();

			ImGui::Begin("Informations");
			{
				// Menu
				if (ImGui::BeginMenu("File"))
				{
					MenuFile();
					ImGui::EndMenu();
				}
				ImGui::Checkbox("Display GCode", &displayGcode);
				ImGui::Checkbox("Demo Window", &show_demo_window);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}
			ImGui::End(); // END Test

			if (displayGcode)
			{
				ImGui::Begin("GCode File", &displayGcode);
				openGCode();
				displayGcode = false;
				ImGui::End();
			}

			if (show_demo_window)
			{
				ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
				ImGui::ShowDemoWindow(&show_demo_window);
			}

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

	return 0;
}