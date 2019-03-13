#include "GL/glew.h"

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Cube.h"
//shaders
#include "loadShader.hpp"

Cube::Cube( float largeur, float hauteur, float profondeur)
{
	glGenVertexArrays(1, &m_VertexArrayID);
	glBindVertexArray(m_VertexArrayID);

	m_programID = LoadShaders("shaders/VertexShader.txt", "shaders/FragmentShader.txt");
	m_MatrixID = glGetUniformLocation(m_programID, "MVP");


	// An array of 3 vectors which represents 3 vertices
	GLfloat vertex_data[] = {
	 -largeur/2, 0.0f,-largeur/2, // triangle 1 : begin
	-largeur/2, 0.0f, largeur/2,
	-largeur/2, hauteur, largeur/2, // triangle 1 : end
	largeur/2, hauteur,-largeur/2, // triangle 2 : begin
	-largeur/2,0.0f,-largeur/2,
	-largeur/2, hauteur,-largeur/2, // triangle 2 : end
	largeur/2, 0.0f, largeur/2,
	-largeur/2,0.0f,-largeur/2,
	largeur/2, 0.0f,-largeur/2,
	largeur/2, hauteur,-largeur/2,
	largeur/2, 0.0f,-largeur/2,
	-largeur/2,0.0f,-largeur/2,
	-largeur/2,0.0f,-largeur/2,
	-largeur/2, hauteur, largeur/2,
	-largeur/2, hauteur,-largeur/2,
	largeur/2, 0.0f, largeur/2,
	-largeur/2, 0.0f, largeur/2,
	-largeur/2, 0.0f,-largeur/2,
	-largeur/2, hauteur, largeur/2,
	-largeur/2, 0.0f, largeur/2,
	largeur/2, 0.0f, largeur/2,
	largeur/2, hauteur, largeur/2,
	largeur/2,0.0f,-largeur/2,
	largeur/2, hauteur,-largeur/2,
	largeur/2, 0.0f,-largeur/2,
	largeur/2, hauteur, largeur/2,
	largeur/2, 0.0f, largeur/2,
	largeur/2, hauteur, largeur/2,
	largeur/2, hauteur,-largeur/2,
	-largeur/2, hauteur,-largeur/2,
	largeur/2, hauteur, largeur/2,
	-largeur/2, hauteur,-largeur/2,
	-largeur/2, hauteur, largeur/2,
	largeur/2, hauteur, largeur/2,
	-largeur/2, hauteur, largeur/2,
	largeur/2, 0.0f, largeur/2 
	};


	static const GLfloat g_color_buffer_data[] = {
	0.583f,  0.771f,  0.014f,
	0.609f,  0.115f,  0.436f,
	0.327f,  0.483f,  0.844f,
	0.822f,  0.569f,  0.201f,
	0.435f,  0.602f,  0.223f,
	0.310f,  0.747f,  0.185f,
	0.597f,  0.770f,  0.761f,
	0.559f,  0.436f,  0.730f,
	0.359f,  0.583f,  0.152f,
	0.483f,  0.596f,  0.789f,
	0.559f,  0.861f,  0.639f,
	0.195f,  0.548f,  0.859f,
	0.014f,  0.184f,  0.576f,
	0.771f,  0.328f,  0.970f,
	0.406f,  0.615f,  0.116f,
	0.676f,  0.977f,  0.133f,
	0.971f,  0.572f,  0.833f,
	0.140f,  0.616f,  0.489f,
	0.997f,  0.513f,  0.064f,
	0.945f,  0.719f,  0.592f,
	0.543f,  0.021f,  0.978f,
	0.279f,  0.317f,  0.505f,
	0.167f,  0.620f,  0.077f,
	0.347f,  0.857f,  0.137f,
	0.055f,  0.953f,  0.042f,
	0.714f,  0.505f,  0.345f,
	0.783f,  0.290f,  0.734f,
	0.722f,  0.645f,  0.174f,
	0.302f,  0.455f,  0.848f,
	0.225f,  0.587f,  0.040f,
	0.517f,  0.713f,  0.338f,
	0.053f,  0.959f,  0.120f,
	0.393f,  0.621f,  0.362f,
	0.673f,  0.211f,  0.457f,
	0.820f,  0.883f,  0.371f,
	0.982f,  0.099f,  0.879f
	};


	for (int i = 0; i < 12*3*3; i++)
	{
		m_vertex[i] = vertex_data[i];
		m_color[i] = g_color_buffer_data[i];
	}

	
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &m_vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertex), m_vertex, GL_STATIC_DRAW);

	
	glGenBuffers(1, &m_colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);


}


Cube::~Cube()
{
	// Cleanup VBO
	glDeleteBuffers(1, &m_vertexbuffer);
	glDeleteBuffers(1, &m_colorbuffer);
	glDeleteVertexArrays(1, &m_VertexArrayID);
	glDeleteProgram(m_programID);
}


glm::mat4 Cube::rotationY(float rotationAngle)
{
	m_rotationY = rotationAngle;
	glm::vec3 rotation = glm::vec3(0, m_rotationY, 0);
	glm::quat cubeQuaternion = glm::quat(rotation);


	return glm::mat4_cast(cubeQuaternion);
}



void Cube::afficher(glm::mat4 &mvpMatrix)
{
	

	// Use our shader
	glUseProgram(m_programID);


	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(m_MatrixID, 1, GL_FALSE, &mvpMatrix[0][0]);


	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 12*3);//sizeof(m_vertex)); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
