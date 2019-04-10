#include <GL/glew.h>

#include <GLFW/glfw3.h>
#ifdef __unix
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#include <GLFW/glfw3native.h>
#endif
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include "SceneOpenGL.h"
#include "Struct.h"
#include "Cube.h"
#include "Fil.h"
#include "Simulation.h"
#include "tinyfiledialogs.h"
#include "ImGui_Menu_Windows.h"
#include "Mesh.h"
#include "Shader.h"
#include "Foam.h"


#define BACKGROUND_COLOR 1.0f, 1.0f, 1.0f

#define LARGEUR_CUBE 100.0f
#define HAUTEUR_CUBE 100.0f
#define PROFONDEUR_CUBE 100.0f

#define ECART_X_BASE_FIL 400.0f
#define ECART_MOTEURS_FIL 600.0f
#define HAUTEUR_MIN_FIL 0.0f
#define HAUTEUR_MAX_FIL 500.0f

////////////////////////////////
#define VERTEX_SHADER_FIL_PATH "shaders/Fil/FilVertexShader.txt"
#define FRAGMENT_SHADER_FIL_PATH "shaders/Fil/FilFragmentShader.txt"

#define VERTEX_SHADER_CUBE_PATH "shaders/Cube/CubeVertexShader.txt"
#define FRAGMENT_SHADER_CUBE_PATH "shaders/Cube/CubeFragmentShader.txt"

#define VERTEX_SHADER_BASE_PATH "shaders/Base/BaseVertexShader.txt"
#define FRAGMENT_SHADER_BASE_PATH "shaders/Base/BaseFragmentShader.txt"

SceneOpenGL::SceneOpenGL(std::string windowTitle, int width, int height):
m_windowTitle(windowTitle), m_window(NULL),
m_gui(), m_simu(),
m_windowWidth(width), m_windowHeight(height)
{	
	m_ratio = (float)width / height;
}

SceneOpenGL::SceneOpenGL()
{
}

SceneOpenGL::~SceneOpenGL()
{
	
}


void SceneOpenGL::mainLoop()
{
	m_gui.openFileDialog(m_simu); //charger le gcode

	//A Changer via fichier config
	const float ecartCubeFil = ECART_X_BASE_FIL; 
	const float ecartMoteursFil = ECART_MOTEURS_FIL;
	const float hauteurFilOrigine = HAUTEUR_MIN_FIL;
	float tCubeX = LARGEUR_CUBE;
	float tCubeY = HAUTEUR_CUBE;
	float tCubeZ = PROFONDEUR_CUBE;
	
	float hauteurMaxFil = HAUTEUR_MAX_FIL; 

	const glm::vec3 cubeCentrePos = glm::vec3(0, tCubeY/2, 0);

	/*CAMERA*/
	const glm::vec3 camTarget = cubeCentrePos;
	const glm::vec3 camPosDefault = glm::vec3(cubeCentrePos.x - 700.0f, cubeCentrePos.y + 700.0f, cubeCentrePos.z + 700.0f);
	glm::vec3 camPos = camPosDefault; 

	float zoomDefault = 1.0f;
	float zoomFactor = zoomDefault;

	
	//create 3D objects 
	//Cube base(ecartCubeFil*2, -1.0f, ECART_MOTEURS_FIL, VERTEX_SHADER_BASE_PATH, FRAGMENT_SHADER_BASE_PATH);
	//Cube cube(tCubeX, tCubeY, tCubeZ, VERTEX_SHADER_CUBE_PATH, FRAGMENT_SHADER_CUBE_PATH);
	Fil fil(ecartCubeFil, hauteurFilOrigine, ecartMoteursFil, VERTEX_SHADER_FIL_PATH, FRAGMENT_SHADER_FIL_PATH);
	

	Mesh cutSurface;
	Shader cutShader(VERTEX_SHADER_BASE_PATH, FRAGMENT_SHADER_BASE_PATH);
	Foam foam(tCubeX, tCubeY, tCubeZ, 0.0f);
	Shader foamShader(VERTEX_SHADER_CUBE_PATH, FRAGMENT_SHADER_CUBE_PATH);
	

	//add cutter and foam objects to the simulation
	m_simu.BindObjects(foam, fil, cutSurface);

	/* MATRIX */
	
	// Projection matrix : Field of View, ratio, display range : 0.1 unit <-> 100 units

	glm::mat4 Projection = glm::perspective(glm::radians(50.0f / zoomFactor), m_ratio , 200.0f, 1500.0f);
	//glm::mat4 ProjectionDefault = Projection;
	glm::mat4 View  = glm::lookAt(
		glm::vec3(camPos), // Camera is at (4,3,3), in World Space
		glm::vec3(camTarget), // and looks at the cube
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);



	glm::mat4 ModelCube = glm::mat4(1.0f); 
	glm::mat4 ModelBase = glm::mat4(1.0f);
	glm::mat4 ModelFil = glm::mat4(1.0f);


	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 mvpCube = Projection * View * ModelCube; // Remember, matrix multiplication is the other way around
	glm::mat4 mvpFil = Projection * View * ModelFil;
	glm::mat4 mvpBase =Projection * View * ModelBase;

	/* ROTATION CUBE*/
	
	float rotationAngle = 0.0f;
	float rotationAngleInit = rotationAngle;
	float vitesse = 1.0f;
	
	int etatSimu = 2;//imgui
	int draw = 0;
	

	do {
		
		//adjust ratio when the window is resized by the user
		glfwGetWindowSize(m_window, &m_windowWidth, &m_windowHeight);
		
		m_ratio = (float)m_windowWidth / m_windowHeight;

		info.WINDOW_HEIGHT = m_windowHeight;
		info.WINDOW_WIDTH = m_windowWidth;


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ImGui_ImplGlfwGL3_NewFrame();

		zoomFactor += ImGui::GetIO().MouseWheel * 0.1f;
		
		if (zoomFactor < 0.4f)
			zoomFactor = 0.4f;

		glm::mat4 saveModelCube = ModelCube;
		glm::mat4 saveModelBase = ModelBase;




		///! SIMULATION !///
		if (m_simu.isRunning())
		{
			draw = 1;
			m_simu.SimulerDecoupe(vitesse, ImGui::GetIO().Framerate);
		}
		else if (!(etatSimu == -1 || etatSimu==3 || etatSimu==2))
		{
			etatSimu = 0;
		}
		m_gui.axisPos(m_simu);


		glm::mat4 Projection = glm::perspective(glm::radians(50.0f / zoomFactor), m_ratio, 200.0f, 1500.0f);

		View = glm::lookAt(
			glm::vec3(camPos), 
			glm::vec3(camTarget), 
			glm::vec3(0,1,0));

		mvpCube = Projection * View * ModelCube;
		mvpBase = Projection * View * ModelBase;
		mvpFil = Projection * View * ModelFil;

		//draw 3D objects
		
		//base.afficher(mvpBase);
		//cube.afficher(mvpCube);
		

		if (draw)
			cutSurface.Draw(cutShader, mvpBase, 1);
		fil.afficher(mvpFil);

		foam.mesh.Draw(foamShader, mvpBase, 1);
		
		ModelCube = saveModelCube;
		ModelBase = saveModelBase;

		mvpCube = Projection * View * ModelCube;
	
		//imgui
		
		ImGui::Begin("Simulation");
		{
			if (etatSimu == 1)
			{
				if (ImGui::Button("Pause"))
				{
					if (etatSimu != 0 && etatSimu != -1)
					{
						m_simu.Arreter();
						etatSimu = 3;
					}
				}
			}
			else
			{
				if (ImGui::Button("Start"))
				{
					m_simu.Demarrer();
					etatSimu = 1;
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Reset"))
			{
				etatSimu = -1;
				m_simu.Init();
			}
			
			
			if (vitesse > 10)
				ImGui::SliderFloat("Vitesse", &vitesse, 0.1f, 10.1f, "Max", 2.0f);
			else
				ImGui::SliderFloat("Vitesse", &vitesse, 0.1f, 10.1f, "%.1f", 2.0f);

			if (etatSimu == 2)
			{
				ImGui::TextUnformatted("En attente...");
			}
			else if (etatSimu == 0)
			{
				ImGui::TextUnformatted("Simulation terminee");
			}
			else if (etatSimu == 1)
			{
				ImGui::TextUnformatted("Simulation en cours...");
			}
			else if(etatSimu == -1)
			{
				ImGui::TextUnformatted("Simulation reinitialisee");
			}
			else
			{
				ImGui::TextUnformatted("Simulation en pause");
			}
			ImGui::Separator();
			ImGui::TextUnformatted(m_simu.getCurrentCmd().c_str());
			ImGui::Text("");
			ImGui::Separator();
			if(m_simu.hasGcode())
			ImGui::TextUnformatted(content.recentFileName.c_str());

		}ImGui::End();
		
		ImGui::Begin("Camera pos");
		{

			if (ImGui::Button("Vue par defaut "))
			{
				camPos = glm::vec3(camPosDefault);
				zoomFactor = zoomDefault;
			}
			ImGui::SameLine();
			if (ImGui::Button("Vue dessus "))
			{
				camPos = glm::vec3(0, 1000.0f, 0.01f);
			}
			ImGui::SameLine();
			if (ImGui::Button("Vue face "))
			{
				camPos = glm::vec3(-1000.0f, HAUTEUR_CUBE/2, 0.1f);
			}
			if (ImGui::Button("Zoom par defaut"))
			{
				zoomFactor = zoomDefault;
			}
			ImGui::SameLine();
			ImGui::Text("x%.1f", zoomFactor);


			ImGui::SliderFloat("CamPos X", &camPos.x, -1000.0f, 1000.0f, "%.3f", 1.0f);
			ImGui::SliderFloat("CamPos Y", &camPos.y, 0.0f, 1000.0f, "%.3f", 1.0f);
			ImGui::SliderFloat("CamPos Z", &camPos.z, -1000.0f, 1000.0f, "%.3f", 1.0f);
			
		}ImGui::End();

				

		/* Display the menu bar at the top of the window */
		m_gui.AppMainMenuBar(m_simu);
		/* Display all the Imgui windows */
		m_gui.ImguiRender();

		/* Render Imgui */
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap buffers
		glfwSwapBuffers(m_window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(m_window) == 0);

	
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

}



bool SceneOpenGL::initWindow()
{
	glewExperimental = true; // Needed in core profile
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
#ifdef _WIN32
	
		//glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);
#endif

	glfwWindowHint(GLFW_REFRESH_RATE, 60); //fullscreen only
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
	#if __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	// Open a window and create its OpenGL context
	m_window = glfwCreateWindow(m_windowWidth, 
				m_windowHeight, m_windowTitle.c_str(), NULL, NULL);

	if (m_window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_window);

	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	

	//glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE); //enable keyboard capture
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); //enable mouse capture
	
	glfwSwapInterval(1); // Enable vsync


	glClearColor(BACKGROUND_COLOR, 0.0f); //back color
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glDisable(GL_BLEND);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE); //should be enabled by default by the driver
	//glEnable(GL_LINE_SMOOTH);

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

bool SceneOpenGL::initSimu()
{
	m_simu.Init();

	return true;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


