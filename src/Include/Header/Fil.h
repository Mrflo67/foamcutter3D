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
	Fil(float ecartCubeFil, float hauteurFilOrigine, float ecartMoteursFil, 
		std::string const vertexShader, std::string const fragmentShader);
	~Fil();
	void afficher(glm::mat4 & model, glm::mat4 &view, glm::mat4 &projection);
	void majPos(float, float, float, float);
	void setOriginPos(float, float, float, float);
	void getCurrentPos(float pos[]);
	void getOriginPos(float pos[]);
	void getInterFoamPos(float pos[4], float L);
	float getEcartX();

	const float posZ_XY;
	const float posZ_UV;

	Mesh trajectory;
private:

	float m_hauteurOrigine;
	float m_ecartMoteurs;
	float m_ecartCubeFil;
	//indices significations: 
	//0=X, 1=Y, 2=U, 3=V
	float m_currentPos[4];

	GLfloat m_vertex[2*3];
	GLuint m_indices[2];

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	Shader m_shader;
	

};

