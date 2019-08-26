/**
*	WIRE.H FILE
*	WIRE CLASS DECLARATION
*	PROJECT BTS SN 2019 - FOAM CUTTER
*/

#pragma once
#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "Mesh.h"
#include "Shader.h"

class Fil
{
public:
	Fil(float ecartCubeFil, float hauteurFilOrigine, float ecartMoteursFil);
	~Fil();
	void Draw(Shader const& shader, const glm::mat4 & mvpMatrix);
	void majPos(float, float, float, float);
	void setOriginPos(float, float, float, float);
	void getCurrentPos(float pos[]);
	void getOriginPos(float pos[]);
	float getEcartX();

	const float posZ_XY;
	const float posZ_UV;

	Mesh wire;
	Mesh trajectory;
private:

	float m_hauteurOrigine;
	float m_ecartMoteurs;
	float m_ecartCubeFil;
	//indices significations: 
	//0=X, 1=Y, 2=U, 3=V
	float m_currentPos[4];

};

