#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Mesh.h"
#include <array>

class Foam :
	public Mesh
{
public:
	Foam(float l, float h, float L, float r=0.0f);
	~Foam();

	void Draw(Shader & shader, glm::mat4 &mvpMatrix);

	float getRotationDeg();
	float getRotationRad();
	
	std::array<float, 3> getVertexCoords(unsigned int n);
	
private :

	const float m_longueur;
	const float m_hauteur;
	const float m_largeur;

	const float m_rotationY;
};

