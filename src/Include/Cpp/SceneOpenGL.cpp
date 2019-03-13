
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
#include "ImGui_Menu_Windows.h"
#include "Struct.h"
#include "Cube.h"
#include "Fil.h"

#define BACKGROUND_COLOR 1.0f, 1.0f, 1.0f

#define TAILLE_CUBE 1.0f, 1.0f, 1.0f
#define ECART_BASE_FIL 5.0f
#define ECART_MOTEURS_FIL 10.0f
#define HAUTEUR_FIL_INIT 0.2f

#define HAUTEUR_MAX_FIL 5.0f





//orthogonal view limits
#define LEFT_LIMIT -10.0f
#define RIGHT_LIMIT 10.0f
#define BOTTOM_LIMIT -10.0f
#define TOP_LIMIT 10.0f


////////////////////////////////
#define filPosX posX
#define filPosY posY
#define filPosZ posZ
#define filPosU posU
#define filPosV posV
#define filPosW posW


SceneOpenGL::SceneOpenGL(std::string windowTitle, int windowWidth, int windowHeigth):
m_windowTitle(windowTitle), m_windowWidth(windowWidth), m_windowHeigth(windowHeigth), m_ratio((float)m_windowWidth / m_windowHeigth), m_window(NULL)
{
}


SceneOpenGL::~SceneOpenGL()
{
	
}

void SceneOpenGL::mainLoop()
{
	//A Changer via fichier config
	const float ecartCubeFil = ECART_BASE_FIL; 
	const float ecartMoteursFil = ECART_MOTEURS_FIL;
	const float hauteurFilOrigine = HAUTEUR_FIL_INIT;
	glm::vec3 tailleCube = glm::vec3(TAILLE_CUBE);
	float hauteurMaxFil = HAUTEUR_MAX_FIL; 


	const glm::vec3 cubePos = glm::vec3(ecartCubeFil, 0.0f, 0.0f);

	/*CAMERA*/
	const glm::vec3 camTarget = cubePos;
	const glm::vec3 camPosDefault = glm::vec3(cubePos.x -1.0f, 1.0f, 1.0f);
	glm::vec3 camPos = camPosDefault;
	float zoomFactor = 0.6f;


	/* Add 3D Objects to the scene*/
	Cube cube(1.0f,1.0f,1.0f);
	Fil fil(hauteurFilOrigine, ecartMoteursFil);
	
	float posX=0.0f,posY=0.0f, posU=0.0f, posV=0.0f;


	
	/* INIT MATRIX*/
	
	// Projection matrix : Field of View, ratio, display range : 0.1 unit <-> 100 units
	//glm::mat4 Projection = glm::perspective(glm::radians(70, ratio , 0.1f, 100.0f);

	glm::mat4 Projection = glm::ortho(LEFT_LIMIT*m_ratio*zoomFactor, RIGHT_LIMIT*m_ratio*zoomFactor, BOTTOM_LIMIT*zoomFactor, TOP_LIMIT*zoomFactor,  -100.0f, 100.0f);
	glm::mat4 View  = glm::lookAt(
		glm::vec3(camPos), // Camera is at (4,3,3), in World Space
		glm::vec3(camTarget), // and looks at the cube
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);


	// ModelCube matrix : ModelCube position in the 3D world
	glm::mat4 ModelCube = glm::mat4(1.0f); 
	ModelCube = glm::translate(ModelCube, glm::vec3(cubePos));
	ModelCube = glm::scale(ModelCube, tailleCube);
	glm::mat4 ModelCubeFil = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 mvp = Projection * View * ModelCube; // Remember, matrix multiplication is the other way around
	glm::mat4 mvpFil = Projection * View * ModelCubeFil;

	
	
	/* ROTATION CUBE*/
	
	float rotationAngle = 0.0f;
	float rotationAngleInit = rotationAngle;

	/* SIMULATION*/

	///std::string commandeLue;

	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);
	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplGlfwGL3_NewFrame();

		glm::mat4 saveModelCube = ModelCube;
		
		ModelCube *= cube.rotationY(rotationAngleInit += rotationAngle);
		

		glm::mat4 Projection = glm::ortho(LEFT_LIMIT*m_ratio*zoomFactor, RIGHT_LIMIT*m_ratio*zoomFactor, BOTTOM_LIMIT*zoomFactor, TOP_LIMIT*zoomFactor, -100.0f, 100.0f);
		View = glm::lookAt(
			glm::vec3(camPos), // Camera is at (4,3,3), in World Space
			glm::vec3(camTarget), // and looks at the target
			glm::vec3(0,1,0));

		mvp = Projection * View * ModelCube;
		mvpFil = Projection * View * ModelCubeFil;
		



		fil.majPos(posX, posY, posU, posV);





		//draw 3D objects
		fil.afficher(mvpFil);
		cube.afficher(mvp);
		
		
		ModelCube = saveModelCube;
		mvp = Projection * View * ModelCube;
		rotationAngle = 0.0f;

		ImGui::Begin("Transforms");
		{
			if (ImGui::Button("Valeurs par defaut"))
			{
				

				
				rotationAngleInit = 0;
			}

			ImGui::SliderFloat("TranslationXFil", &filPosX, 0, ecartCubeFil * 2 , "%.3f", 1.0f);
			ImGui::SliderFloat("TranslationYFil", &filPosY, 0, hauteurMaxFil, "%.3f", 1.0f);
			ImGui::SliderFloat("TranslationUFil", &filPosU, 0, ecartCubeFil * 2, "%.3f", 1.0f);
			ImGui::SliderFloat("TranslationVFil", &filPosV, 0, hauteurMaxFil, "%.3f", 1.0f);
			
			ImGui::SliderFloat("Rotation Cube Y", &rotationAngle, -0.1f, 0.1f, "%.3f", 1.0f);

			
		}ImGui::End();

		ImGui::Begin("Camera pos");
		{

			if (ImGui::Button("Vue par defaut "))
			{
				camPos = glm::vec3(camPosDefault);
				zoomFactor = 0.5f;
			}

			if (ImGui::Button("Vue dessus "))
			{
				camPos = glm::vec3(ecartCubeFil, TOP_LIMIT, 0.01f);
			}

			if (ImGui::Button("Vue face "))
			{
				camPos = glm::vec3(ecartCubeFil-10.0f, 0.0f, 0.01f);
			}
			ImGui::SliderFloat("CamPos X", &camPos.x, LEFT_LIMIT-ecartCubeFil, RIGHT_LIMIT+ecartCubeFil, "%.3f", 1.0f);
			ImGui::SliderFloat("CamPos Y", &camPos.y, -10.0f, 10.0f, "%.3f", 1.0f);
			ImGui::SliderFloat("CamPos Z", &camPos.z, -1.0f, 1.0f, "%.3f", 1.0f);

			ImGui::SliderFloat("Zoom", &zoomFactor, 0.1f, 1.0f, "%.3f", 1.0f);
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
	glClearColor(BACKGROUND_COLOR, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

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

