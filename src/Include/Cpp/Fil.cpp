
#include "Fil.h"
#include "loadShader.hpp"
#include <iostream>

Fil::Fil(float ecartCubeFil, float hauteurFilOrigine, float ecartMoteursFil)
{
	m_hauteurOrigine = hauteurFilOrigine;
	m_ecartMoteurs = ecartMoteursFil;
	m_ecartCubeFil = ecartCubeFil;


	glGenVertexArrays(1, &m_VertexArrayID);
	glBindVertexArray(m_VertexArrayID);

	m_programID = LoadShaders("shadersFil/FilVertexShader.txt", "shadersFil/FilFragmentShader.txt");
	m_MatrixID = glGetUniformLocation(m_programID, "MVP");


	// An array of 3 vectors which represents 3 vertices
	GLfloat vertex_data[] = {
		-ecartCubeFil, 0.0f + m_hauteurOrigine, m_ecartMoteurs / 2,
		-ecartCubeFil, 0.0f + m_hauteurOrigine, -m_ecartMoteurs / 2,

	};

	static const GLfloat g_color_buffer_data[] = {
	0.1f,  0.0f,  0.0f,
	0.1f,  0.0f,  0.0f,
	};


	for (int i = 0; i < 2 * 3; i++)
	{
		m_vertex[i] = vertex_data[i];
		m_color[i] = g_color_buffer_data[i];
	}

	for (int i = 0; i < 4; i++)
	{
		m_currentPos[i] = 0.0f;
	}

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &m_vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertex), m_vertex, GL_DYNAMIC_DRAW);


	glGenBuffers(1, &m_colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);


}


Fil::~Fil()
{
	// Cleanup VBO
	glDeleteBuffers(1, &m_vertexbuffer);
	glDeleteBuffers(1, &m_colorbuffer);
	glDeleteVertexArrays(1, &m_VertexArrayID);
	glDeleteProgram(m_programID);
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

	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
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
	glDrawArrays(GL_LINE_STRIP, 0, 2); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
