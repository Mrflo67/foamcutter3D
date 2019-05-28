/**
*	FOAM.H FILE
*	FOAM CLASS DEFINITION
*	PROJECT BTS SN 2019 - FOAM CUTTER
*/

#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Mesh.h"

class Foam
{
public:
	Foam(float l, float h, float L, float r = 0.0f, float pX = 0.0f, float pZ = 0.0f);
	~Foam();

	float getRotationDeg();
	float getRotationRad();
	float getWidth();
	float getHeight();
	float getLength();
	float getPosX();
	float getPosZ();
	void setWidth(float L);
	void setLength(float l);
	void setHeight(float h);
	void setPosX(float pX);
	void setPosZ(float pZ);
	void setRotationRad(float r);
	Mesh mesh;

private:
	float m_rotationY;
	float m_length, m_height, m_width;
	float m_posX, m_posZ;
};

