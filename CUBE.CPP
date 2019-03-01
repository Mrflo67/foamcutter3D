#include "pch.h"
#include "Header/Cube.h"
#include <common/loadShader.hpp>

Cube::Cube()
{
	
	glGenVertexArrays(1, &m_VertexArrayID);
	glBindVertexArray(m_VertexArrayID);

	m_programID = LoadShaders("src/VertexShader.txt", "src/FragmentShader.txt");

	// An array of 3 vectors which represents 3 vertices
	GLfloat vertex_data[] = {
	   -0.3f, -0.3f, 0.0f,
	   0.3f, -0.3f, 0.0f,
	   0.0f,  0.3f, 0.0f,
	};

	for (int i = 0; i < 8; i++)
	{
		m_vertex[i] = vertex_data[i];
	}


	
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &m_vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertex), m_vertex, GL_STATIC_DRAW);
}


Cube::~Cube()
{
	// Cleanup VBO
	glDeleteBuffers(1, &m_vertexbuffer);
	glDeleteVertexArrays(1, &m_VertexArrayID);
	glDeleteProgram(m_programID);
}

void Cube::afficher()
{
	


	// Use our shader
	glUseProgram(m_programID);

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
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
}
