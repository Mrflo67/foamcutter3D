#include "Fil.h"
#include "Shader.h"
#include <iostream>
#include <array>


Fil::Fil(float ecartCubeFil, float hauteurFilOrigine, float ecartMoteursFil,
	std::string const vertexShader, std::string const fragmentShader) :
	m_hauteurOrigine(hauteurFilOrigine), m_ecartMoteurs(ecartMoteursFil), m_ecartCubeFil(ecartCubeFil),
	m_shader(vertexShader, fragmentShader), posZ_XY(ecartMoteursFil/2), posZ_UV(-ecartMoteursFil/2),
	trajectory()
{
		// An array of 3 vectors which represents 3 vertices
	GLfloat vertex_data[] = {
		-ecartCubeFil, 0.0f + m_hauteurOrigine, m_ecartMoteurs / 2,
		-ecartCubeFil, 0.0f + m_hauteurOrigine, -m_ecartMoteurs / 2,

	};

	GLuint indices_data[] = {
		0,1,
	};
	
	for (int i = 0; i < 2 * 3; i++)
	{
		m_vertex[i] = vertex_data[i];
	}

	for (int i = 0; i < 2; i++)
		m_indices[i] = indices_data[i];

	for (int i = 0; i < 4; i++)
	{
		m_currentPos[i] = 0.0f;
	}

	trajectory.m_vertices.clear();
	trajectory.m_indices.clear();

	trajectory.m_vertices.push_back(
		std::array<float, 3>{ {vertex_data[3], vertex_data[4], vertex_data[5]}});
	trajectory.m_vertices.push_back(
		std::array<float, 3>{ {vertex_data[0], vertex_data[1], vertex_data[2]}});


	trajectory.m_indices.push_back(1);
	trajectory.m_indices.push_back(1);
	trajectory.m_indices.push_back(0);
	trajectory.m_indices.push_back(0);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertex), m_vertex, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices),
		&m_indices[0], GL_STATIC_DRAW);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

}


Fil::~Fil()
{

}


void Fil::majPos(float newPos_X, float newPos_Y, float newPos_U, float newPos_V)
{
	unsigned int offset = trajectory.m_vertices.size();
	
	GLfloat newVertexData[] = {
		-m_ecartCubeFil + newPos_X, newPos_Y + m_hauteurOrigine, m_ecartMoteurs / 2,
		-m_ecartCubeFil + newPos_U, newPos_V + m_hauteurOrigine, -m_ecartMoteurs / 2,
	};

	std::array<float, 3> v0 = { newVertexData[3], newVertexData[4], newVertexData[5] };
	trajectory.m_vertices.push_back(v0);
	
	v0[0] = newVertexData[0]; v0[1] = newVertexData[1]; v0[2] = newVertexData[2];

	trajectory.m_vertices.push_back(v0);
	
	if (trajectory.m_vertices.size() >= 4)
	{
		trajectory.m_indices.push_back(offset - 1);
		trajectory.m_indices.push_back(offset + 1);

		trajectory.m_indices.push_back(offset);
		trajectory.m_indices.push_back(offset - 2);
	}
	

	m_currentPos[0] = newPos_X;
	m_currentPos[1] = newPos_Y;
	m_currentPos[2] = newPos_U;
	m_currentPos[3] = newPos_V;

	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER,
		0,
		sizeof(m_vertex),
		newVertexData);
}

//returns array which contains X,Y,U,V current positions in this order
void Fil::getCurrentPos(float pos[])
{
	for (int i = 0; i < 4; i++)
		pos[i] = m_currentPos[i];
}

//returns array which contains X,Y,U,V origin positions in this order
void Fil::getOriginPos(float pos[])
{
	pos[0] = -m_ecartCubeFil;
	pos[1] = m_hauteurOrigine;
	pos[2] = pos[1];
	pos[3] = pos[0];
}

void Fil::getInterFoamPos(float pos[4])
{
	//1. Computing vertical position

	float Vangle; //vertical angle between A(X,Y) and B(U,V)
	//A and B : near and far wire ends;
	float posAX = posZ_XY;
	float posAY = m_currentPos[1];
	float posBX = posZ_UV;
	float posBY = m_currentPos[3];
	//F1 and F2 = foam wideness pos X
	float largeurFoam = 100.0f; //todo
	float xF1 = largeurFoam/2;
	float posYInter1, posYInter2;

	if (posAY > posBY)
	{
		Vangle = glm::atan(abs((posAY - posBY) / (posAX - posBX)));
		posYInter1 = posAY - glm::tan(Vangle) * (posAX - xF1);
		posYInter2 = posAY - glm::tan(Vangle) * (posAX + xF1);
	}
	else if (posBY > posAY)
	{
		Vangle = glm::atan((posBY - posAY) / (posAX - posBX));
		Vangle = glm::atan(abs((posAY - posBY) / (posAX - posBX)));
		posYInter2 = posBY - glm::tan(Vangle) * (posAX - xF1);
		posYInter1 = posBY - glm::tan(Vangle) * (posAX + xF1);

	}
	else
	{
		Vangle = 0;
		posYInter1 = posAY;
		posYInter2 = posBY;
	}

	pos[1] = posYInter1;
	pos[3] = posYInter2;



	//2. horizontal position


	float Hangle;
	posAY = *m_currentPos;
	posBY = m_currentPos[2];
	posAX = posZ_XY;
	posBX = posZ_UV;
	float longueurFoam = 100.0f;
	xF1 = longueurFoam / 2;
	float posXInter1, posXInter2;


	if (posAY > posBY)
	{
		Hangle = glm::atan(abs((posAY - posBY) / (posAX - posBX)));
		posXInter1 = posAY - glm::tan(Hangle) * (posAX - xF1);
		posXInter2 = posAY - glm::tan(Hangle) * (posAX + xF1);
	}
	else if (posAY < posBY)
	{
		Hangle = glm::atan(abs((posAY - posBY) / (posAX - posBX)));
		posXInter2 = posBY - glm::tan(Hangle) * (posAX - xF1);
		posXInter1 = posBY - glm::tan(Hangle) * (posAX + xF1);
		
	}
	else
	{
		Vangle = 0;
		posXInter1 = posAY;
		posXInter2 = posBY;
	}


	pos[0] = posXInter1;
	pos[2] = posXInter2;
	
}

void Fil::setOriginPos(float X, float Y, float U, float V)
{}



void Fil::afficher(glm::mat4 &mvpMatrix)
{

	glUseProgram(m_shader.getProgramID());

	GLuint MatrixID = glGetUniformLocation(m_shader.getProgramID(), "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvpMatrix[0][0]);


	glBindVertexArray(VAO);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	trajectory.Draw(m_shader, mvpMatrix, 0);
	
}
