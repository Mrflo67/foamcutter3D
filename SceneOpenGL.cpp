#include "Header\SceneOpenGL.h"

#define WINDOW_WIDTH 1600 //in pixels
#define WINDOW_HEIGTH 900

#define BACKGROUND_COLOR 0.0f, 0.0f, 0.4f, 0.0f
//************************ R     G     B    ALPHA


SceneOpenGL::SceneOpenGL(std::string windowTitle, int windowWidth, int windowHeigth):
m_windowTitle(windowTitle), m_windowWidth(windowWidth), m_windowHeigth(windowHeigth), m_window(NULL)
{
}


SceneOpenGL::~SceneOpenGL()
{
	glfwTerminate();
}

void SceneOpenGL::mainLoop()
{
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);

	do {


		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		//glUseProgram(programID);


		/* Draw */


		// 1st attribute buffer : vertices
		/*glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
										  //glDrawArrays(GL_TRIANGLES, 3, 3);

		glDisableVertexAttribArray(0);

		*/


		// Swap buffers
		glfwSwapBuffers(m_window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(m_window) == 0);

	// Cleanup VBO
	/*glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);
	*/
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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

																   // Open a window and create its OpenGL context
	m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGTH, m_windowTitle.c_str(), NULL, NULL);
	if (m_window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(m_window); // Initialize GLEW
	

	// Dark blue background
	glClearColor(BACKGROUND_COLOR);

	return true;
}

bool SceneOpenGL::initGL()
{
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		std::cout << "GLEW Initialisation failed : " << std::endl;
		return false;
	}

	return true;
}

