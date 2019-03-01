#pragma once

class Cube
{
public:
	Cube();
	~Cube();
	void afficher();

private:

	GLfloat m_vertex[8];
	GLuint m_programID=NULL;
	// This will identify our vertex buffer
	GLuint m_vertexbuffer;
	GLuint m_VertexArrayID;
};

