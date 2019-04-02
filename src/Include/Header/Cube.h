#pragma once

#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"


class Cube
{
public:
	Cube(float l, float h, float p,
		std::string const vertexShader, std::string const fragmentShader);
	~Cube();
	void afficher(glm::mat4 &mvpMatrix);
	void rotationY(float rotationAngle);
	float getRotationAngle();

private:

	float m_longueur;
	float m_hauteur;
	float m_profondeur;
	float m_rotationY = 0.0f;
	GLfloat m_vertex[12*3*3];
	GLfloat m_color[12 * 3 * 3];
	GLuint m_vertexbuffer;
	GLuint m_colorbuffer;
	GLuint m_VertexArrayID;
	Shader m_shader;
};

