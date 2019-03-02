#include "pch.h"
#include "Header/SceneOpenGL.h"
#include "Header/ImGui_Menu_windows.h"
#include "Header/Struct.h"
#include "Header/Cube.h"

#define BACKGROUND_COLOR 1.0f, 1.0f, 1.0f, 0.0f
//************************ R     G     B    ALPHA

#define FIELD_OF_VIEW 70.0f
#define CAM_POS 0, 0, 7
//*************X, Y, Z


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
	Cube cube2;
	Cube cube3;

	// Projection matrix : Field of View, ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(FIELD_OF_VIEW), (float)info.WINDOW_WIDTH / (float)info.WINDOW_HEIGTH, 0.1f, 100.0f);

	//glm::mat4 Projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f);
	glm::mat4 View  = glm::lookAt(
		glm::vec3(CAM_POS), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// Model matrix : model position in the 3D world
	glm::mat4 Model = glm::mat4(1.0f); 
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around


	//generate mvp for second model

	glm::mat4 Model2 = glm::mat4(1.0f);
	glm::mat4 mvp2 = Projection * View * Model2;

	//generate mvp for third model

	glm::mat4 Model3 = glm::mat4(1.0f);
	glm::mat4 mvp3 = Projection * View * Model3;

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




		glm::mat4 saveModel = Model;
		glm::mat4 saveModel2 = Model2;
		glm::mat4 saveModel3 = Model3;

		Model = glm::translate(Model, glm::vec3(0.0f, 0.0f, 0.0f));
		Model2 = glm::translate(Model2, glm::vec3(2.0f, 3.0f, 0.0f));
		Model3 = glm::translate(Model3, glm::vec3(-2.0f, -3.0f, -2.0f));
		mvp = Projection * View * Model;
		mvp2 = Projection * View * Model2;
		mvp3 = Projection * View * Model3;

		//draw the cubes
		cube.afficher(mvp);
		cube2.afficher(mvp2);
		cube3.afficher(mvp3);
		

		Model = saveModel;
		Model2 = saveModel2;
		Model3 = saveModel3;
		mvp = Projection * View * Model;
		mvp2 = Projection * View * Model2;
		mvp3 = Projection * View * Model3;



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

