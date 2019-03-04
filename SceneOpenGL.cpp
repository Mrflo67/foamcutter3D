#include "pch.h"
#include "Header/SceneOpenGL.h"
#include "Header/ImGui_Menu_windows.h"
#include "Header/Struct.h"
#include "Header/Cube.h"
#include "Header/Fil.h"

#define BACKGROUND_COLOR 1.0f, 1.0f, 1.0f, 0.0f
//************************ R     G     B    ALPHA

#define FIELD_OF_VIEW 70.0f
#define CAM_POS 0, 0, 7
//*************X, Y, Z


#define filPosX filVertexData[0]
#define filPosY filVertexData[1]
#define filPosZ filVertexData[2]
#define filPosU filVertexData[3]
#define filPosV filVertexData[4]
#define filPosW filVertexData[5]



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
	float vitesseSimulation(0.1); //min 0.1 max 1.0

	Cube cube;
	Fil fil;
	
	GLfloat filVertexData[] = {
		0.0f, 0.0f, 0.0f,
		5.0f,0.0f, 0.0f,

	};

	// Projection matrix : Field of View, ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(FIELD_OF_VIEW), (float)info.WINDOW_WIDTH / (float)info.WINDOW_HEIGTH, 0.1f, 100.0f);

	//glm::mat4 Projection = glm::ortho(0.0f, 400.0f, 0.0f, 400.0f, -10.0f, 10.0f);
	glm::mat4 View  = glm::lookAt(
		glm::vec3(CAM_POS), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// ModelCube matrix : ModelCube position in the 3D world
	glm::mat4 ModelCube = glm::mat4(1.0f); 
	glm::mat4 ModelCubeFil = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 mvp = Projection * View * ModelCube; // Remember, matrix multiplication is the other way around
	glm::mat4 mvpFil = Projection * View * ModelCubeFil;

	glm::vec3 translation(0.0f, 0.0f, 0.0f);
	


	/* ROTATION CUBE*/
	
	//glm::vec3 EulerAngles(RotationAroundXInRadians, RotationAroundYInRadians, RotationAroundZInRadians);
	glm::vec3 rotation(0, 0, 0);
	
	glm::quat myQuaternion;
	
	myQuaternion = glm::quat(rotation);
	
	



	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplGlfwGL3_NewFrame();

		glm::mat4 saveModelCube = ModelCube;
		
		ModelCube = glm::translate(ModelCube, translation);
		myQuaternion = glm::quat(rotation);
		ModelCube *= glm::mat4_cast(myQuaternion);
		
		mvp = Projection * View * ModelCube;

		
		fil.majPos(filVertexData);

		//draw the cubes
		fil.afficher(mvpFil);
		cube.afficher(mvp);
		
		
		ModelCube = saveModelCube;
		mvp = Projection * View * ModelCube;
		

		ImGui::Begin("Transforms");
		{
			ImGui::SliderFloat("TranslationCubeX", &translation.x, -5.0f, 5.0f, "%.3f", 1.0f);
			ImGui::SliderFloat("TranslationCubeY", &translation.y, -5.0f, 5.0f, "%.3f", 1.0f);
			ImGui::SliderFloat("TranslationCubeZ", &translation.z, -5.0f, 5.0f, "%.3f", 1.0f);

			ImGui::SliderFloat("TranslationXFil", &filPosX, -5.0f, 5.0f, "%.3f", 1.0f);
			ImGui::SliderFloat("TranslationYFil", &filPosY, -5.0f, 5.0f, "%.3f", 1.0f);
			ImGui::SliderFloat("TranslationZFil", &filPosZ, -5.0f, 5.0f, "%.3f", 1.0f);
			ImGui::SliderFloat("TranslationUFil", &filPosU, -5.0f, 5.0f, "%.3f", 1.0f);
			ImGui::SliderFloat("TranslationVFil", &filPosV, -5.0f, 5.0f, "%.3f", 1.0f);
			ImGui::SliderFloat("TranslationZ2Fil", &filPosW, -5.0f, 5.0f, "%.3f", 1.0f);

			ImGui::SliderFloat("Rotation Cube Y", &rotation.y, -3.14f, 3.14f, "%.3f", 1.0f);

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

