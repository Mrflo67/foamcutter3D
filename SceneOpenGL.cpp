#include "pch.h"
#include "Header/SceneOpenGL.h"
#include "Header/ImGui_Menu_windows.h"
#include "Header/Struct.h"
#include "Header/Cube.h"

#define BACKGROUND_COLOR 0.0f, 0.0f, 0.4f, 0.0f
//************************ R     G     B    ALPHA


SceneOpenGL::SceneOpenGL(std::string windowTitle, int windowWidth, int windowHeigth):
m_windowTitle(windowTitle), m_windowWidth(windowWidth), m_windowHeigth(windowHeigth), m_window(NULL)
{
}


SceneOpenGL::~SceneOpenGL()
{
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

void SceneOpenGL::mainLoop()
{
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);

	std::string commandeLue("");
	int etatSimulation(0); //0=arrêt, 1=lancée, 2=pause

	Cube cube;




	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//draw the cubes
		cube.afficher();

		ImGui_ImplGlfwGL3_NewFrame();

		/* Display the menu bar at the top of the window */
		AppMainMenuBar();
		/* Display all the Imgui windows */
		ImguiRender();

		/* Render Imgui */
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		
		// Swap buffers
		glfwSwapBuffers(m_window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(m_window) == 0);

	
}




bool SceneOpenGL::initWindow()
{
	glewExperimental = true; // Needed for core profile
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
	#if __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	// Open a window and create its OpenGL context
	m_window = glfwCreateWindow(m_windowWidth, 
		m_windowHeigth, m_windowTitle.c_str(), NULL, NULL);
	if (m_window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_window); // Initialize GLEW
	glfwSwapInterval(1); // Enable vsync

	// Dark blue background
	glClearColor(BACKGROUND_COLOR);

	return true;
}

bool SceneOpenGL::initGL()
{
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		std::cout << "GLEW Initialisation failed : " << std::endl;
		return false;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	return true;
}

bool SceneOpenGL::initImGUI()
{
	ImGui::CreateContext();
	if (ImGui_ImplGlfwGL3_Init(m_window, true) == false)
	{
		return false;
	}
	ImGui::StyleColorsDark(); /* Display imgui in dark */

	return true;

}

