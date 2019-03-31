#include "GL/glew.h"

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Cube.h"
//shaders
#include "loadShader.hpp"

Cube::Cube( float l, float h, float p) : m_longueur(l), m_hauteur(h), m_profondeur(p)
{
	glGenVertexArrays(1, &m_VertexArrayID);
	glBindVertexArray(m_VertexArrayID);

	m_programID = LoadShaders("shadersCube/CubeVertexShader.txt", "shadersCube/CubeFragmentShader.txt");
	m_MatrixID = glGetUniformLocation(m_programID, "MVP");


	// An array of 3 vectors which represents 3 vertices
	GLfloat vertex_data[] = {
	 -m_longueur/2, 0.0f,-m_profondeur/2, // Face avant
	-m_longueur/2, 0.0f, m_profondeur/2,
	-m_longueur/2, m_hauteur, m_profondeur/2, 
	-m_longueur / 2,0.0f,-m_profondeur / 2, 
	-m_longueur / 2, m_hauteur, m_profondeur / 2, 
	-m_longueur / 2, m_hauteur,-m_profondeur / 2, 

	m_longueur/2, m_hauteur,-m_profondeur/2, // Face Gauche
	-m_longueur/2,0.0f,-m_profondeur/2, 
	-m_longueur/2, m_hauteur,-m_profondeur/2, 
	m_longueur / 2, m_hauteur,-m_profondeur / 2, 
	m_longueur / 2, 0.0f,-m_profondeur / 2, 
	-m_longueur / 2,0.0f,-m_profondeur / 2, 

	m_longueur/2, 0.0f, m_profondeur/2, //Face dessous
	-m_longueur/2,0.0f,-m_profondeur/2,
	m_longueur/2, 0.0f,-m_profondeur/2,
	m_longueur/2, 0.0f, m_profondeur/2,
	-m_longueur/2, 0.0f, m_profondeur/2,
	-m_longueur/2, 0.0f,-m_profondeur/2, 

	m_longueur / 2, 0.0f,-m_profondeur / 2, //Face arrière
	m_longueur / 2, m_hauteur, m_profondeur / 2,
	m_longueur / 2, 0.0f, m_profondeur / 2,
	m_longueur/2, m_hauteur, m_profondeur/2,
	m_longueur/2,0.0f,-m_profondeur/2,
	m_longueur/2, m_hauteur,-m_profondeur/2, 

	m_longueur/2, m_hauteur, m_profondeur/2, //Face droite
	m_longueur/2, m_hauteur,-m_profondeur/2,
	-m_longueur/2, m_hauteur,-m_profondeur/2,
	m_longueur/2, m_hauteur, m_profondeur/2,
	-m_longueur/2, m_hauteur,-m_profondeur/2,
	-m_longueur/2, m_hauteur, m_profondeur/2, 
	
	m_longueur/2, m_hauteur, m_profondeur/2, //Face dessus
	-m_longueur/2, m_hauteur, m_profondeur/2,
	m_longueur/2, 0.0f, m_profondeur/2 ,
	- m_longueur / 2, m_hauteur, m_profondeur / 2,
	-m_longueur / 2, 0.0f, m_profondeur / 2,
	m_longueur / 2, 0.0f, m_profondeur / 2, 

	};


	static const GLfloat g_color_buffer_data[] = {
		0.0f, 1.0f, 1.0f, 
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,


		0.0f, 0.9f, 0.9f,
		0.0f, 0.9f, 0.9f,
		0.0f, 0.9f, 0.9f,
		0.0f, 0.9f, 0.9f,
		0.0f, 0.9f, 0.9f,
		0.0f, 0.9f, 0.9f,

		0.0f, 0.8f, 0.8f, 
		0.0f, 0.8f, 0.8f,
		0.0f, 0.8f, 0.8f,
		0.0f, 0.8f, 0.8f,
		0.0f, 0.8f, 0.8f,
		0.0f, 0.8f, 0.8f,
		
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,

		0.0f, 0.9f, 0.9f,
		0.0f, 0.9f, 0.9f,
		0.0f, 0.9f, 0.9f,
		0.0f, 0.9f, 0.9f,
		0.0f, 0.9f, 0.9f,
		0.0f, 0.9f, 0.9f,

		0.0f, 0.8f, 0.8f,
		0.0f, 0.8f, 0.8f,
		0.0f, 0.8f, 0.8f,
		0.0f, 0.8f, 0.8f,
		0.0f, 0.8f, 0.8f,
		0.0f, 0.8f, 0.8f,

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


void Cube::rotationY(float rotationAngle)
{
	m_rotationY = rotationAngle;
	
}

float Cube::getRotationAngle()
{
	return m_rotationY;
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
