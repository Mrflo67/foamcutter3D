
#include "GL/glew.h"

#include <GLFW/glfw3.h>

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include "SceneOpenGL.h"
#ifndef RESOLUTION_AUTOSET
#define RESOLUTION_AUTOSET 1
#endif
#include "ImGui_Menu_Windows.h"
#include "Struct.h"
#include "Cube.h"
#include "Fil.h"
#include "Simulation.h"

#define BACKGROUND_COLOR 0.9f, 0.9f, 0.9f

#define LARGEUR_CUBE 1.0f
#define HAUTEUR_CUBE 1.0f
#define PROFONDEUR_CUBE 1.0f

#define ECART_X_BASE_FIL 2.0f
#define ECART_MOTEURS_FIL 2.0f
#define HAUTEUR_MIN_FIL 0.2f
#define HAUTEUR_MAX_FIL 2.0f



//cam limits
#define LEFT_LIMIT -20.0f
#define RIGHT_LIMIT 20.0f
#define BOTTOM_LIMIT -20.0f
#define TOP_LIMIT 20.0f


////////////////////////////////
#define filPosX posX
#define filPosY posY
#define filPosZ posZ
#define filPosU posU
#define filPosV posV
#define filPosW posW


SceneOpenGL::SceneOpenGL(std::string windowTitle, int width, int height):
m_windowTitle(windowTitle), m_window(NULL)
{
	Simulation simu;
	m_simu = simu;

	if (!RESOLUTION_AUTOSET)
	{
		m_windowHeight = height;
		m_windowWidth = width;
	}
	else
	{
		getScreenResolution();
		m_windowWidth = info.WINDOW_WIDTH;
		m_windowHeight = info.WINDOW_HEIGHT;
	}

	m_ratio = (float)m_windowWidth / m_windowHeight;
}


SceneOpenGL::~SceneOpenGL()
{
	
}


void SceneOpenGL::mainLoop()
{
	
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
	const glm::vec3 camPosDefault = glm::vec3(cubeCentrePos.x - 6.0f, cubeCentrePos.y + 6.0f, cubeCentrePos.z + 6.0f);
	glm::vec3 camPos = camPosDefault; 
	float zoomFactor = 0.4f;

	Cube cube(tCubeX, tCubeY, tCubeZ);
	Fil fil(ecartCubeFil, hauteurFilOrigine, ecartMoteursFil);
	
	m_simu.BindObjects(&cube, &fil);

	float posX=0.0f,posY=0.0f, posU=0.0f, posV=0.0f;


	
	/* INIT MATRIX*/
	
	// Projection matrix : Field of View, ratio, display range : 0.1 unit <-> 100 units

	glm::mat4 Projection = glm::perspective(glm::radians(70.0f), m_ratio , 0.1f, 100.0f);
	//glm::mat4 ProjectionDefault = Projection;
	glm::mat4 View  = glm::lookAt(
		glm::vec3(camPos), // Camera is at (4,3,3), in World Space
		glm::vec3(camTarget), // and looks at the cube
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);


	// ModelCube matrix : ModelCube position in the 3D world
	glm::mat4 ModelCube = glm::mat4(1.0f); 
	//ModelCube = glm::scale(ModelCube, tailleCube);
	glm::mat4 ModelCubeFil = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 mvp = Projection * View * ModelCube; // Remember, matrix multiplication is the other way around
	glm::mat4 mvpFil = Projection * View * ModelCubeFil;

	/* ROTATION CUBE*/
	
	float rotationAngle = 0.0f;
	float rotationAngleInit = rotationAngle;

	/* SIMULATION*/

	///std::string commandeLue;

	float posInit = 0.0f;
	posX = posInit;
	float posDebut = posX;
	float posFin = 0.0f;
	float vitesse = 0.1f;
	float pas = fabs((posFin - posDebut) * (vitesse / 2));


	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);
	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		ImGui_ImplGlfwGL3_NewFrame();

		glm::mat4 saveModelCube = ModelCube;

		///! SIMULATION !///
		if (m_simu.isRunning())
		{
			m_simu.SimulerDecoupe(vitesse, ModelCube);
		}

		glm::mat4 Projection = glm::perspective(glm::radians(70.0f * zoomFactor), m_ratio, 0.1f, 100.0f);

		View = glm::lookAt(
			glm::vec3(camPos), // Camera is at (4,3,3), in World Space
			glm::vec3(camTarget), // and looks at the target
			glm::vec3(0,1,0));

		mvp = Projection * View * ModelCube;
		mvpFil = Projection * View * ModelCubeFil;
		
		//fil.majPos(posX, posY, posU, posV);

		//////////////////////

		//draw 3D objects
		fil.afficher(mvpFil);
		cube.afficher(mvp);
	
		
		ModelCube = saveModelCube;
		mvp = Projection * View * ModelCube;
		rotationAngle = 0.0f;


		//imgui
		ImGui::Begin("Transforms");
		{
			if (ImGui::Button("Valeurs par defaut"))
			{
				posX = 0;
				posY = 0;
				posU = 0;
				posV = 0;

				
				rotationAngleInit = 0;
			}

			ImGui::SliderFloat("TranslationXFil", &filPosX, 0, ecartCubeFil * 2 , "%.3f", 1.0f);
			ImGui::SliderFloat("TranslationYFil", &filPosY, 0, hauteurMaxFil, "%.3f", 1.0f);
			ImGui::SliderFloat("TranslationUFil", &filPosU, 0, ecartCubeFil * 2, "%.3f", 1.0f);
			ImGui::SliderFloat("TranslationVFil", &filPosV, 0, hauteurMaxFil, "%.3f", 1.0f);
			ImGui::SliderFloat("Vitesse", &vitesse, 1.0f, 0.0f, "%.3f", 0.1f);
			
			ImGui::SliderFloat("Rotation Cube Y", &rotationAngle, -0.1f, 0.1f, "%.3f", 1.0f);

			
		}ImGui::End();

		ImGui::Begin("Simulation");
		{
			if (ImGui::Button("Start"))
			{
				m_simu.Demarrer();
			}
			if (ImGui::Button("Stop"))
			{
				m_simu.Arreter();
			}
		}ImGui::End();
		
		ImGui::Begin("Camera pos");
		{

			if (ImGui::Button("Vue par defaut "))
			{
				camPos = glm::vec3(camPosDefault);
				zoomFactor = 0.4f;
			}

			if (ImGui::Button("Vue dessus "))
			{
				camPos = glm::vec3(0, 10.0f, 0.01f);
			}

			if (ImGui::Button("Vue face "))
			{
				camPos = glm::vec3(-10.0f, 0.0f, 0.1f);
			}
			ImGui::SliderFloat("CamPos X", &camPos.x, -10.0f, 10.0f, "%.3f", 1.0f);
			ImGui::SliderFloat("CamPos Y", &camPos.y, -10.0f, 10.0f, "%.3f", 1.0f);
			ImGui::SliderFloat("CamPos Z", &camPos.z, -10.0f, 10.0f, "%.3f", 1.0f);

			ImGui::SliderFloat("Zoom", &zoomFactor,1.0f, 0.05f, "%.3f", 1.0f);
		}ImGui::End();


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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
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

	glfwMakeContextCurrent(m_window); // Initialize GLEW
	glfwSwapInterval(1); // Enable vsync

	// Dark blue background
	glClearColor(BACKGROUND_COLOR, 0.0f);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	

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
	if (!m_simu.ChargerGcode())
		return false;

	return true;
}

