#pragma once

class Cube
{
public:
	Cube();
	~Cube();
	void afficher(glm::mat4 &modelViewController);

private:

	GLfloat m_vertex[12*3*3];
	GLfloat m_color[12 * 3 * 3];
	GLuint m_programID=NULL;
	// This will identify our vertex buffer
	GLuint m_vertexbuffer;
	GLuint m_colorbuffer;

	GLuint m_VertexArrayID;

	GLuint m_MatrixID;
};

