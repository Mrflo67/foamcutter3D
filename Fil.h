#pragma once
class Fil
{
public:
	Fil();
	~Fil();
	void afficher(glm::mat4 &mvpMatrix);
	void majPos(GLfloat newVertexData[]);

private:

	GLfloat m_vertex[2*3];
	GLfloat m_color[6];
	GLuint m_programID = NULL;
	// This will identify our vertex buffer
	GLuint m_vertexbuffer;
	GLuint m_colorbuffer;

	GLuint m_VertexArrayID;

	GLuint m_MatrixID;
};

