#include <GL/glew.h>

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Cube.h"

#include "Shader.h"

Cube::Cube(float l, float h, float L,
	std::string const vertexShader, std::string const fragmentShader) :
	m_shader(vertexShader, fragmentShader)
{
	l /= 2;
	L /= 2;

	// An array of 3 vectors which represents 3 vertices
	GLfloat vertex_data[] = {
		l, h, L,     //0 top right //coins face avant
		l, 0.0f, L,  //1 bottom right
		-l, 0.0f, L, //2 bottom left
		-l, h, L,    //3 top left
		
		-l, h, -L,   //4 top left //coins face arriere
		-l, 0.0f, -L,//5 bottom left
		l, 0.0f, -L, //6 bottom right
		l, h, -L,    //7 top right

	};

	

	GLuint indices_data[] =
	{
		0, 3, 1, // face avant
		1, 3, 2,
		2, 3, 5, // gauche
		3, 4, 5,
		5, 2, 6, // dessous
		6, 2, 1,
		1, 6, 0, // droite
		0, 6, 7,
		7, 4, 3, // dessus
		3, 0, 7,
		7, 4, 5, // arriere
		5, 7, 0
	};


	for (int i = 0; i < sizeof(m_vertex) / sizeof(float); i++)
	{
		m_vertex[i] = vertex_data[i];
	}
	for (int i = 0; i < sizeof(m_indices) / sizeof(unsigned int); i++)
	{
		m_indices[i] = indices_data[i];
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertex), m_vertex, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices),
		&m_indices[0], GL_STATIC_DRAW);


	// vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

}


Cube::~Cube()
{
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
	glUseProgram(m_shader.getProgramID());

	GLuint MatrixID = glGetUniformLocation(m_shader.getProgramID(), "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvpMatrix[0][0]);

	glBindVertexArray(VAO);
	
	// Draw the triangle !
	glDrawElements(GL_TRIANGLES, sizeof(m_indices) / sizeof(GLuint), GL_UNSIGNED_INT, (void*)0);
	
	glBindVertexArray(0);

}
