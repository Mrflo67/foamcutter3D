/**
*	SCENEOPENGL.CPP FILE
*	INITIALIZE AND RENDER OUR PROJECT
*	PROJECT BTS SN 2019 - FOAM CUTTER
*/

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
#include "Base.h"
#include "Fil.h"
#include "Simulation.h"
#include "ImGui_Menu_Windows.h"
#include "Mesh.h"
#include "Shader.h"
#include "Foam.h"
#include "Config.h"
#include "ClipPlane.h"

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

#define VERTEX_SHADER_GUIDE_PATH "shaders/Guide/GuideVertexShader.txt"
#define FRAGMENT_SHADER_GUIDE_PATH "shaders/Guide/GuideFragmentShader.txt"

//normals for clip planes
#define N_TOP 0, -1, 0
#define N_BACK 0, 0, 1
#define N_FRONT 0, 0, -1
#define N_LEFT 1, 0, 0
#define N_RIGHT -1, 0, 0

#define ZFAR 3000.0f
#define ZNEAR 300.0f

SceneOpenGL::SceneOpenGL(std::string windowTitle, int width, int height):
m_windowTitle(windowTitle), m_window(NULL),
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
	Simulation simu;
	ImguiMenuWindow gui;
	static Config config;                                                                                                                                                                                                                                                           

	gui.recentPath();

	if (!config.Read()) //charger le fichier de config
	{
		std::cout << "Creating configuration file ..." << std::endl;
		config.Write();//on le crée s'il est introuvable
	}
	
	//recuperation des donnees du fichier config
	float ecartCubeFil = config.Plate.length / 2; 
	float ecartMoteursFil = config.Plate.width;
	float tCubeX = config.sFoam.length;
	float tCubeY = config.sFoam.height;
	float tCubeZ = config.sFoam.width;

	const glm::vec3 cubeCentrePos = glm::vec3(0, 50, 0);

	/*CAMERA*/
	glm::vec3 camTarget = cubeCentrePos;
	const glm::vec3 camPosDefault = glm::vec3(cubeCentrePos.x - 700.0f, cubeCentrePos.y + 700.0f, cubeCentrePos.z + 700.0f);
	glm::vec3 camPos = camPosDefault; 

	float zoomDefault = 1.0f;
	float zoomFactor = zoomDefault;
	float fov = 50.0f;
	

	//add 3D objects to the scene
	//Base, Wire, Foam and shaders
	Base base(1.0f, -1.0f, 1.0f);
	Fil fil(ecartCubeFil, 0.0f, ecartMoteursFil);
	Shader filShader(VERTEX_SHADER_FIL_PATH, FRAGMENT_SHADER_FIL_PATH);

	Mesh cutSurface;
	Shader cutShader(VERTEX_SHADER_CUT_PATH, FRAGMENT_SHADER_CUT_PATH);
	Foam foam(tCubeX, tCubeY, tCubeZ, glm::radians(config.sFoam.angleY), config.sFoam.posX, config.sFoam.posZ);
	Shader foamShader(VERTEX_SHADER_CUBE_PATH, FRAGMENT_SHADER_CUBE_PATH);
	Shader baseShader(VERTEX_SHADER_BASE_PATH, FRAGMENT_SHADER_BASE_PATH);

	//3D axis guide
	GLfloat guide_vertex_data[] = {
		1, 0, 0, //X axis
		0, 0, 0, //origin
		0, 1, 0, //Y axis
		0, 0, 0, //origin
		0, 0, 1, //Z axis
		0, 0, 0  //origin
	};

	GLuint guide_indices_data[] = {
		0,1,
		2,3,
		4,5
	};

	GLfloat guide_color_data[] = {
		1, 0, 0,
		1, 0, 0,
		0, 0, 1,
		0, 0, 1,
		0, 1, 0,
		0, 1, 0,
	};

	Mesh guide(guide_vertex_data, guide_indices_data, 18, 6, guide_color_data, 18);
	Shader guideShader(VERTEX_SHADER_GUIDE_PATH, FRAGMENT_SHADER_GUIDE_PATH);



	//add cutter and foam objects to the simulation
	simu.BindObjects(foam, fil, cutSurface);

	/* MATRIX */
	
	// Projection matrix : Field of View, ratio, display range

	glm::mat4 Projection = glm::perspective(glm::radians(fov / zoomFactor), m_ratio , ZNEAR, ZFAR);
	//glm::mat4 ProjectionDefault = Projection;
	glm::mat4 View  = glm::lookAt(
		glm::vec3(camPos), 
		glm::vec3(camTarget),
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	glm::mat4 guideView = glm::lookAt(
		glm::vec3(camPos),
		glm::vec3(camTarget),
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	glm::mat4 guideProjection = glm::perspective(glm::radians(15.0f), m_ratio, ZNEAR, ZFAR);


	glm::mat4 Mat1 = glm::mat4(1.0f);
	glm::mat4 ModelCube = Mat1; 
	glm::mat4 ModelBase = Mat1;
	glm::mat4 ModelFil = Mat1;
	glm::mat4 ModelGuide = Mat1;

	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 mvp1 = Projection * View * Mat1;

	glm::mat4 mvpCube = Projection * View * ModelCube; // Remember, matrix multiplication is the other way around
	glm::mat4 mvpFil = Projection * View * ModelFil;
	glm::mat4 mvpBase = Projection * View * ModelBase;
	glm::mat4 mvpGuide = guideProjection * guideView * Mat1;

	/* ROTATION CUBE*/
	
	float vitesse = 1.0f;
	
	int etatSimu = 2;//imgui
	int draw = 0;
	bool vsync = true;

	//clip planes

	std::array<glm::vec3, 5> normals;

	normals[0] = glm::vec3(N_TOP);
	normals[1] = glm::vec3(N_BACK);
	normals[2] = glm::vec3(N_FRONT);
	normals[3] = glm::vec3(N_LEFT);
	normals[4] = glm::vec3(N_RIGHT);

	std::array<glm::vec3, 5> points;

	points[0] = glm::vec3(0, 1, 0); //top
	points[1] = glm::vec3(0, 0, -0.5); //back
	points[2] = glm::vec3(0, 0, 0.5); //front
	points[3] = glm::vec3(-0.5, 0, 0); //left
	points[4] = glm::vec3(0.5, 0,0); //right


	std::array<ClipPlane,5> originalPlanes;

	for (int i = 0; i < 5; ++i)
	{
		originalPlanes[i] = ClipPlane(normals[i], points[i]);
	}

	std::array<ClipPlane, 5> planes;

	for (int i = 0; i < 5; ++i)
	{
		planes[i] = originalPlanes[i];
	}

	GLint params[4];

	do {
		//update objects size based on config
		if(vitesse >= 10.0f && etatSimu == 1)
		{	
			vsync = false;
			glfwSwapInterval((int)vsync);
		}
		else
		if (config.Graphics.vsync != vsync)
		{
			vsync = config.Graphics.vsync;
			glfwSwapInterval((int)vsync);

		}


		glGetIntegerv(GL_VIEWPORT, params);

		m_windowWidth = params[2];
		m_windowHeight = params[3];

		m_ratio = (float)m_windowWidth / m_windowHeight;

		info.WINDOW_WIDTH = m_windowWidth;
		info.WINDOW_HEIGHT = m_windowHeight;


		zoomFactor += ImGui::GetIO().MouseWheel * 0.1f;
		
		if (zoomFactor < 0.4f)
			zoomFactor = 0.4f;


		//save model matrix
		glm::mat4 saveModelCube = ModelCube;
		glm::mat4 saveModelBase = ModelBase;
		glm::mat4 saveModelGuide = ModelGuide;

		//apply transformations
		ModelCube = glm::translate(ModelCube, glm::vec3(foam.getPosX(), 1.0f, foam.getPosZ()));
		ModelCube = glm::rotate(ModelCube, foam.getRotationRad(), glm::vec3(0,1,0));
		ModelCube = glm::scale(ModelCube, glm::vec3(foam.getLength(), foam.getHeight(), foam.getWidth()));
		ModelBase = glm::scale(ModelBase, glm::vec3(config.Plate.length, 0.0f, config.Plate.width));
		//ModelGuide = glm::translate(ModelGuide, glm::vec3(0, 0, 0));
		//ModelGuide = glm::translate(ModelGuide, glm::vec3(-camPos.x, -camPos.y, -camPos.z));
		ModelGuide = glm::scale(ModelGuide, glm::vec3(100,100, 100));
		
		//test plane transformations
		for (int i = 0; i < 5; ++i)
		{
			planes[i].transform(ModelCube);
		}
		
		///! SIMULATION !///
		if (simu.isRunning())
		{
			draw = 1;
			simu.SimulerDecoupe(vitesse, ImGui::GetIO().Framerate);
		}
		else if (!(etatSimu == -1 || etatSimu==3 || etatSimu==2))
		{
			etatSimu = 0;

		}

		//update mvp matrix

		Projection = glm::perspective(glm::radians(fov / zoomFactor), m_ratio, ZNEAR, ZFAR);
		glm::mat4 guideProjection = glm::perspective(glm::radians(15.0f), m_ratio, ZNEAR, ZFAR);

		View = glm::lookAt(
			glm::vec3(camPos), 
			glm::vec3(camTarget), 
			glm::vec3(0,1,0));

		glm::mat4 guideView = glm::lookAt(
			glm::vec3(camPos), // Camera is at (4,3,3), in World Space
			glm::vec3(0,0,0), // and looks at the cube
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

		mvpCube = Projection * View * ModelCube;
		mvpBase = Projection * View * ModelBase;
		mvpFil = Projection * View * ModelFil;
		mvp1 = Projection * View * Mat1;
		mvpGuide = guideProjection * guideView * ModelGuide;

		//draw 3D objects
		
		if (draw == 0)
			draw = 1;

		if (cutSurface.load)
		{
			cutShader.load();
			cutSurface.load = 0;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ImGui_ImplGlfwGL3_NewFrame();
		gui.axisPos(simu);
		
		glEnable(GL_CLIP_DISTANCE0);
		glEnable(GL_CLIP_DISTANCE1);
		glEnable(GL_CLIP_DISTANCE2);
		glEnable(GL_CLIP_DISTANCE3);
		glEnable(GL_CLIP_DISTANCE4);
		cutSurface.Draw(cutShader, Mat1, View, Projection, 1, planes);
		glDisable(GL_CLIP_DISTANCE0);
		glDisable(GL_CLIP_DISTANCE1);
		glDisable(GL_CLIP_DISTANCE2);
		glDisable(GL_CLIP_DISTANCE3);
		glDisable(GL_CLIP_DISTANCE4);
		
		base.mesh.Draw(baseShader, mvpBase, 1);
		foam.mesh.Draw(foamShader, mvpCube, 0);
		fil.Draw(filShader, mvpFil);
		
		//guide docked on the corner of the screen
		glViewport(0, m_windowHeight/10, 200*m_ratio, 200);
		glDisable(GL_DEPTH_TEST);
		glLineWidth(3.0f);

		guide.Draw(guideShader, mvpGuide, 2);

		glViewport(0, 0, m_windowWidth, m_windowHeight);
		glEnable(GL_DEPTH_TEST);
		glLineWidth(1.0f);
		//imgui
		
		ImGui::Begin("Simulation");
		{
			if (simu.hasGcode())
				ImGui::TextUnformatted(content.recentFileName.c_str());
			else
				ImGui::TextUnformatted("No gcode");
			ImGui::Separator();

			if (etatSimu == 1)
			{
				if (ImGui::Button("Pause"))
				{
					if (etatSimu != 0 && etatSimu != -1)
					{
						simu.Arreter();
						etatSimu = 3;
					}
				}
			}
			else
			{
				if (ImGui::Button("Start"))
				{

					simu.Demarrer();
					etatSimu = 1;
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Reset"))
			{
				draw = 0;
				etatSimu = -1;
				simu.Arreter();
				simu.Init();
			
			}
			
			
			if (vitesse > 10)
				ImGui::SliderFloat("Speed", &vitesse, 0.1f, 10.1f, "Max", 2.0f);
			else
				ImGui::SliderFloat("Speed", &vitesse, 0.1f, 10.1f, "%.1f", 2.0f);

			if (etatSimu == 2)
			{
				ImGui::TextUnformatted("Waiting...");
			}
			else if (etatSimu == 0)
			{
				ImGui::TextUnformatted("Simulation over");
			}
			else if (etatSimu == 1)
			{
				ImGui::TextUnformatted("Simulation in progress...");
			}
			else if(etatSimu == -1)
			{
				ImGui::TextUnformatted("Simulation reset");
			}
			else
			{
				ImGui::TextUnformatted("Simulation paused");
			}

			
			ImGui::Separator();
			static std::string prevpreviousCmd = "";
			static std::string previousCmd = "";
			static std::string currentCmd = simu.getCurrentCmd();
			static int line = 0;

			if (simu.getCurrentCmd() != currentCmd)
			{
				prevpreviousCmd = previousCmd;
				previousCmd = currentCmd;
				currentCmd = simu.getCurrentCmd();
				line = simu.getCurrentLineNb();
			}
			ImGui::TextColored(ImVec4(1, 0.7, 0, 1), "  %d. ", line); ImGui::SameLine();
			ImGui::Text("%s", currentCmd.c_str());
			ImGui::Separator();
			ImGui::Text("%d. %s", line -1, previousCmd.c_str());
			ImGui::Text("%d. %s", line - 2, prevpreviousCmd.c_str());
			

		}ImGui::End();
		
		ImGui::Begin("Camera");
		{

			if (ImGui::Button("Default"))
			{
				camPos = glm::vec3(camPosDefault);
				zoomFactor = zoomDefault;
				camTarget = cubeCentrePos;
			}
			ImGui::SameLine();
			if (ImGui::Button("Top"))
			{
				camTarget = cubeCentrePos;
				camPos = glm::vec3(0, 1000.0f, 0.01f);
			}
			ImGui::SameLine();
			if (ImGui::Button("Face"))
			{
				camTarget = cubeCentrePos;
				camPos = glm::vec3(0.0f, tCubeY / 2, 1000.0f);
			}
			ImGui::SameLine();
			if (ImGui::Button("Side "))
			{
				camTarget = cubeCentrePos;
				camPos = glm::vec3(-1000.0f, tCubeY / 2, 0.1f);
			}ImGui::SameLine();
			if (ImGui::Button("Isometric"))
			{
				camTarget = cubeCentrePos;
				camPos = glm::vec3(700.0f, 700.0f, 700.0f);
			}
			if (ImGui::Button("Default Zoom"))
			{
				zoomFactor = zoomDefault;
			}
			ImGui::SameLine();
			ImGui::Text("x%.1f", zoomFactor);


			ImGui::SliderFloat("Cam pos X", &camPos.x, -1000.0f, 1000.0f, "%.3f", 1.0f);
			ImGui::SliderFloat("Cam pos Y", &camPos.y, 0.0f, 1000.0f, "%.3f", 1.0f);
			ImGui::SliderFloat("Cam pos Z", &camPos.z, -1000.0f, 1000.0f, "%.3f", 1.0f);

			ImGui::SliderFloat("Cam target X", &camTarget.x, -1000.0f, 1000.0f, "%.3f", 1.0f);
			ImGui::SliderFloat("Cam target Y", &camTarget.y, 0.0f, 1000.0f, "%.3f", 1.0f);
			ImGui::SliderFloat("Cam target Z", &camTarget.z, -1000.0f, 1000.0f, "%.3f", 1.0f);


		}ImGui::End();

		/* Display the menu bar at the top of the window */
		gui.AppMainMenuBar(simu, config);
		/* Display all the Imgui windows */
		gui.ImguiRender();

		/* Render Imgui */
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());


		//reset model matrix

		ModelCube = saveModelCube;
		ModelBase = saveModelBase;
		ModelGuide = saveModelGuide;

		mvpCube = Projection * View * ModelCube;
		mvpBase = Projection * View * ModelBase;
		mvpGuide = guideProjection * guideView * ModelGuide;

		//reset planes 

		for (int i = 0; i < 5; ++i)
		{
			planes[i] = originalPlanes[i];
		}


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
	glEnable(GL_MULTISAMPLE); //should be enabled by default by the driver
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
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



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	if(height > 0)
	glViewport(0, 0, width, height);
}


