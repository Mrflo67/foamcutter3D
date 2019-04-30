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

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <tinyfiledialogs.h>

#include "SceneOpenGL.h"
#include "Struct.h"
#include "Cube.h"
#include "Fil.h"
#include "Simulation.h"
#include "ImGui_Menu_Windows.h"
#include "Mesh.h"
#include "Shader.h"
#include "Foam.h"
#include "Config.h"

#include <iostream>


#define BACKGROUND_COLOR 1.0f, 1.0f, 1.0f

#define VERTEX_SHADER_FIL_PATH "shaders/Fil/FilVertexShader.txt"
#define FRAGMENT_SHADER_FIL_PATH "shaders/Fil/FilFragmentShader.txt"

#define VERTEX_SHADER_CUBE_PATH "shaders/Cube/CubeVertexShader.txt"
#define FRAGMENT_SHADER_CUBE_PATH "shaders/Cube/CubeFragmentShader.txt"

#define VERTEX_SHADER_BASE_PATH "shaders/Base/BaseVertexShader.txt"
#define FRAGMENT_SHADER_BASE_PATH "shaders/Base/BaseFragmentShader.txt"

#define VERTEX_SHADER_CUT_PATH "shaders/Cutsurface/CutVertexShader.txt"
#define FRAGMENT_SHADER_CUT_PATH "shaders/Cutsurface/CutFragmentShader.txt"

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
	static Config config;                                                                                                                                                                                                                                                           

	m_gui.recentPath();

	if (!config.Read()) //charger le fichier de config
	{
		config.Write();//on le crée s'il est introuvable
	}
	
	//recuperation des donnees du fichier config
	float ecartCubeFil = config.plate.longueur / 2; 
	float ecartMoteursFil = config.plate.largeur;
	float tCubeX = config.foam.longueur;
	float tCubeY = config.foam.hauteur;
	float tCubeZ = config.foam.largeur;
	float hauteurMaxFil = config.hauteurMaxFil; 

	const glm::vec3 cubeCentrePos = glm::vec3(0, tCubeY/2, 0);

	/*CAMERA*/
	const glm::vec3 camTarget = cubeCentrePos;
	const glm::vec3 camPosDefault = glm::vec3(cubeCentrePos.x - 700.0f, cubeCentrePos.y + 700.0f, cubeCentrePos.z + 700.0f);
	glm::vec3 camPos = camPosDefault; 

	float zoomDefault = 1.0f;
	float zoomFactor = zoomDefault;

	//create 3D objects 
	Cube base(1.0f, -1.0f, 1.0f, VERTEX_SHADER_BASE_PATH, FRAGMENT_SHADER_BASE_PATH);
	//Cube cube(tCubeX, tCubeY, tCubeZ, VERTEX_SHADER_CUBE_PATH, FRAGMENT_SHADER_CUBE_PATH);
	Fil fil(ecartCubeFil, 0.0f, ecartMoteursFil, VERTEX_SHADER_FIL_PATH, FRAGMENT_SHADER_FIL_PATH);
	
	Mesh cutSurface;
	Shader cutShader(VERTEX_SHADER_CUT_PATH, FRAGMENT_SHADER_CUT_PATH);
	Foam foam(tCubeX, tCubeY, tCubeZ, 0.0f, config.foam.posX, config.foam.posZ);
	Shader foamShader(VERTEX_SHADER_CUBE_PATH, FRAGMENT_SHADER_CUBE_PATH);
	

	//add cutter and foam objects to the simulation
	m_simu.BindObjects(foam, fil, cutSurface);

	/* MATRIX */
	
	// Projection matrix : Field of View, ratio, display range : 0.1 unit <-> 100 units

	glm::mat4 Projection = glm::perspective(glm::radians(50.0f / zoomFactor), m_ratio , 200.0f, 2000.0f);
	//glm::mat4 ProjectionDefault = Projection;
	glm::mat4 View  = glm::lookAt(
		glm::vec3(camPos), // Camera is at (4,3,3), in World Space
		glm::vec3(camTarget), // and looks at the cube
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);


	glm::mat4 Mat1 = glm::mat4(1.0f);
	glm::mat4 ModelCube = Mat1; 
	glm::mat4 ModelBase = Mat1;
	glm::mat4 ModelFil = Mat1;
	glm::mat4 ModelPlane = Mat1;

	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 mvpCube = Projection * View * ModelCube; // Remember, matrix multiplication is the other way around
	glm::mat4 mvpFil = Projection * View * ModelFil;
	glm::mat4 mvpBase =Projection * View * ModelBase;
	glm::mat4 mvp1 = Projection * View * Mat1;

	/* ROTATION CUBE*/
	
	float rotationAngle = 0.0f;
	float rotationAngleInit = rotationAngle;
	float vitesse = 1.0f;
	
	int etatSimu = 2;//imgui
	int draw = 0;
	bool vsync = true;

	glm::vec4 planes[5];

	glEnable(GL_CLIP_DISTANCE0);
	glEnable(GL_CLIP_DISTANCE1);
	glEnable(GL_CLIP_DISTANCE2);
	glEnable(GL_CLIP_DISTANCE3);
	glEnable(GL_CLIP_DISTANCE4);

	glm::vec4 normalTop(0, -1, 0, 1);
	glm::vec4 normalBack(0, 0, -1, 1);
	glm::vec4 normalFront(0, 0, 1,1);
	glm::vec4 normalLeft(-1, 0, 0, 1);
	glm::vec4 normalRight(1, 0, 0, 1);

	GLint params[4];

	do {
		//update objects size based on config
		if(vitesse >= 10.0f && etatSimu == 1)
		{	
			vsync = false;
			glfwSwapInterval((int)vsync);
		}
		else
		if (config.graphics.vsync != vsync)
		{
			vsync = config.graphics.vsync;
			glfwSwapInterval((int)vsync);

		}

		glGetIntegerv(GL_VIEWPORT, params);

		m_windowWidth = params[2];
		m_windowHeight = params[3];

		m_ratio = (float)m_windowWidth / m_windowHeight;

		info.WINDOW_WIDTH = m_windowWidth;
		info.WINDOW_HEIGHT = m_windowHeight;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ImGui_ImplGlfwGL3_NewFrame();

		zoomFactor += ImGui::GetIO().MouseWheel * 0.1f;
		
		if (zoomFactor < 0.4f)
			zoomFactor = 0.4f;


		//save model matrix
		glm::mat4 saveModelCube = ModelCube;
		glm::mat4 saveModelBase = ModelBase;


		//apply transformations
		ModelCube = glm::translate(ModelCube, glm::vec3(config.foam.posX, 1.0f, config.foam.posZ));
		ModelCube = glm::rotate(ModelCube, glm::radians(config.foam.angleY), glm::vec3(0,1,0));
		ModelCube = glm::scale(ModelCube, glm::vec3(config.foam.longueur, config.foam.hauteur, config.foam.largeur));
		ModelBase = glm::scale(ModelBase, glm::vec3(config.plate.longueur, 0.0f, config.plate.largeur));
		

		//ModelPlane = glm::translate(ModelPlane, glm::vec3(config.foam.posX, 1.0f, config.foam.posZ));
		ModelPlane = glm::rotate(ModelPlane, glm::radians(config.foam.angleY), glm::vec3(0, 1, 0));


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


		//update mvp matrix

		Projection = glm::perspective(glm::radians(50.0f / zoomFactor), m_ratio, 200.0f, 2000.0f);

		View = glm::lookAt(
			glm::vec3(camPos), 
			glm::vec3(camTarget), 
			glm::vec3(0,1,0));

		mvpCube = Projection * View * ModelCube;
		mvpBase = Projection * View * ModelBase;
		mvpFil = Projection * View * ModelFil;
		mvp1 = Projection * View * Mat1;
	
		//clip planes
		normalTop.w = foam.getHauteur();
		normalBack.w = foam.getPosZ() + foam.getLargeur() / 2;


		normalFront.w = -foam.getPosZ() + foam.getLargeur() / 2;
		normalLeft.w = foam.getPosX() + foam.getLongueur() / 2;
		normalRight.w = -foam.getPosX() + foam.getLongueur() / 2;

		planes[0] = glm::vec4(normalTop);
		planes[1] = glm::vec4(ModelPlane * normalBack);
		planes[2] = glm::vec4(ModelPlane * normalFront);
		planes[3] = glm::vec4(ModelPlane * normalLeft);
		planes[4] = glm::vec4(ModelPlane * normalRight);


		//draw 3D objects
		
		if (draw == 0)
			draw = 1;


		if (cutSurface.load)
		{
			cutShader.load();
			cutSurface.load = 0;
		}
		

		

		base.afficher(mvpBase);
		cutSurface.Draw(cutShader, Mat1, View, Projection, 1, planes);
		foam.mesh.Draw(foamShader, ModelCube, View, Projection, 0);
		fil.afficher(ModelFil, View, Projection);
	
		//reset model matrix

		ModelCube = saveModelCube;
		ModelBase = saveModelBase;
		ModelPlane = Mat1;


		mvpCube = Projection * View * ModelCube;
		mvpBase = Projection * View * ModelBase;

		//reset plane normal 
		normalTop.w = 1;
		normalBack.w = 1;
		normalFront.w = 1;
		normalLeft.w = 1;
		normalRight.w = 1;

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
				draw = 0;
				etatSimu = -1;
				m_simu.Arreter();
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
			ImGui::Text("%d. %s", m_simu.getCurrentLineNb(), m_simu.getCurrentCmd().c_str());
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
				camPos = glm::vec3(0.0f, tCubeY / 2, 1000.0f);
			}
			ImGui::SameLine();
			if (ImGui::Button("Vue cote "))
			{
				camPos = glm::vec3(-1000.0f, tCubeY / 2, 0.1f);
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
		m_gui.AppMainMenuBar(m_simu, config);
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

	config.Write();
	
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

}



bool SceneOpenGL::initWindow()
{
	
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

	glClearColor(BACKGROUND_COLOR, 0.0f); //back color
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDisable(GL_BLEND);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE); //should be enabled by default by the driver
	//glEnable(GL_LINE_SMOOTH);

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
	if(height > 0)
	glViewport(0, 0, width, height);
}


