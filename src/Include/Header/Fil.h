#pragma once
#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Fil
{
public:
	Fil(float ecartCubeFil, float hauteurFilOrigine, float ecartMoteursFil);
	~Fil();
	void afficher(glm::mat4 &mvpMatrix);
	void majPos(float, float, float, float);
	void setOriginPos(float, float, float, float);
	void getCurrentPos(float pos[]);
	void getOriginPos(float pos[]);

private:

	float m_hauteurOrigine;
	float m_ecartMoteurs;
	float m_ecartCubeFil;
	float m_currentPos[4];
	GLfloat m_vertex[2*3];
	GLfloat m_color[6];
	GLuint m_programID;
	GLuint m_vertexbuffer;
	GLuint m_colorbuffer;
	GLuint m_VertexArrayID;
	GLuint m_MatrixID;
};

