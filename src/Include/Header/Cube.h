#pragma once

#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"


class Cube
{
public:
	Cube(float l, float h, float L,
		std::string const vertexShader, std::string const fragmentShader);
	~Cube();
	void afficher(glm::mat4 &mvpMatrix);
	void rotationY(float rotationAngle);
	float getRotationAngle();
	int getLargeur();

private:

	float m_rotationY = 0.0f;
	int m_largeur;

	GLfloat m_vertex[3*8];
	GLuint m_indices[12 * 3];
	
	GLuint VBO;
	GLuint VAO;
	GLuint EBO;

	Shader m_shader;
};

