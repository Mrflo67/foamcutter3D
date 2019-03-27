#pragma once

#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Cube
{
public:
	Cube(float l, float h, float p);
	~Cube();
	void afficher(glm::mat4 &mvpMatrix);
	glm::mat4 rotationY(float rotationAngle);

private:

	float m_longueur;
	float m_hauteur;
	float m_profondeur;
	float m_rotationY = 0.0f;
	GLfloat m_vertex[12*3*3];
	GLfloat m_color[12 * 3 * 3];
	GLuint m_programID;
	GLuint m_vertexbuffer;
	GLuint m_colorbuffer;
	GLuint m_VertexArrayID;
	GLuint m_MatrixID;

};

