#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Mesh.h"
#include <array>

class Foam
{
public:
	Foam(float l, float h, float L, float r = 0.0f, float pX = 0.0f, float pZ = 0.0f);
	~Foam();

	void Draw(Shader & shader, glm::mat4 &mvpMatrix);

	float getRotationDeg();
	float getRotationRad();
	float getLargeur();
	float getHauteur();
	float getLongueur();
	float getPosX();
	float getPosZ();
	void setLargeur(float L);
	void setLongueur(float l);
	void setHauteur(float h);
	void setPosX(float pX);
	void setPosZ(float pZ);
	void setRotationRad(float r);

	Mesh mesh;

private :

	float m_rotationY;

	float m_longueur, m_hauteur, m_largeur;
	float m_posX, m_posZ;
};

