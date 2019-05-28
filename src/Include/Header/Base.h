/**
*	CUBE.H FILE
*	CUBE CLASS DECLARATION
*	PROJECT BTS SN 2019 - FOAM CUTTER
*/

#pragma once

#include "Mesh.h"

class Base
{
public:
	Base(float l, float h, float L);
	~Base();
	//void draw(glm::mat4 &mvpMatrix);
	void rotationY(float rotationAngle);
	float getRotationAngle();

	Mesh mesh;
private:

	float m_rotationY = 0.0f;
	int m_largeur;

	GLfloat m_vertex[3*8];
	GLuint m_indices[12 * 3];
	
};

