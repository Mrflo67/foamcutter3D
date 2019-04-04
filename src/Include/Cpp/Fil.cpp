#include "Fil.h"
#include "Shader.h"
#include <iostream>

Fil::Fil(float ecartCubeFil, float hauteurFilOrigine, float ecartMoteursFil,
	std::string const vertexShader, std::string const fragmentShader) :
	m_hauteurOrigine(hauteurFilOrigine), m_ecartMoteurs(ecartMoteursFil), m_ecartCubeFil(ecartCubeFil),
	m_shader(vertexShader, fragmentShader)
{
		// An array of 3 vectors which represents 3 vertices
	GLfloat vertex_data[] = {
		-ecartCubeFil, 0.0f + m_hauteurOrigine, m_ecartMoteurs / 2,
		-ecartCubeFil, 0.0f + m_hauteurOrigine, -m_ecartMoteurs / 2,

	};

	GLuint indices_data[] = {
		0,1,
	};
	
	for (int i = 0; i < 2 * 3; i++)
	{
		m_vertex[i] = vertex_data[i];
	}

	for (int i = 0; i < 2; i++)
		m_indices[i] = indices_data[i];

	for (int i = 0; i < 4; i++)
	{
		m_currentPos[i] = 0.0f;
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertex), m_vertex, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices),
		&m_indices[0], GL_STATIC_DRAW);

	// 1st attribute buffer : vertices

	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);
}


Fil::~Fil()
{

}


void Fil::majPos(float newPos_X, float newPos_Y, float newPos_U, float newPos_V)
{
	GLfloat newVertexData[] = {
		-m_ecartCubeFil + newPos_X, newPos_Y + m_hauteurOrigine, m_ecartMoteurs / 2,
		-m_ecartCubeFil + newPos_U, newPos_V + m_hauteurOrigine, -m_ecartMoteurs / 2,
	};

	m_currentPos[0] = newPos_X;
	m_currentPos[1] = newPos_Y;
	m_currentPos[2] = newPos_U;
	m_currentPos[3] = newPos_V;

	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER,
		0,
		sizeof(m_vertex),
		newVertexData);
}

//returns array which contains X,Y,U,V current positions in this order
void Fil::getCurrentPos(float pos[])
{
	for (int i = 0; i < 4; i++)
		pos[i] = m_currentPos[i];
}

//returns array which contains X,Y,U,V origin positions in this order
void Fil::getOriginPos(float pos[])
{
	pos[0] = -m_ecartCubeFil;
	pos[1] = m_hauteurOrigine;
	pos[2] = pos[1];
	pos[3] = pos[0];
}

void Fil::setOriginPos(float X, float Y, float U, float V)
{}



void Fil::afficher(glm::mat4 &mvpMatrix)
{

	// Use our shader
	glUseProgram(m_shader.getProgramID());

	GLuint MatrixID = glGetUniformLocation(m_shader.getProgramID(), "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvpMatrix[0][0]);


	glBindVertexArray(VAO);
	glDrawElements(GL_LINES, sizeof(m_indices), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
}
