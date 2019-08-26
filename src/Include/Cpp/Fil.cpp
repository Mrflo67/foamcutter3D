/**
*	WIRE.CPP FILE
*	RENDER THE WIRE
*	PROJECT BTS SN 2019 - FOAM CUTTER
*/

#include "Fil.h"
#include "Shader.h"
#include <iostream>
#include <array>


Fil::Fil(float ecartCubeFil, float hauteurFilOrigine, float ecartMoteursFil) :
	m_hauteurOrigine(hauteurFilOrigine), m_ecartMoteurs(ecartMoteursFil), m_ecartCubeFil(ecartCubeFil),
	posZ_XY(ecartMoteursFil/2), posZ_UV(-ecartMoteursFil/2),
	trajectory()
{
		// An array of 3 vectors which represents 3 vertices
	GLfloat vertex_data[] = {
		-ecartCubeFil, 0.0f + m_hauteurOrigine, m_ecartMoteurs / 2,
		-ecartCubeFil, 0.0f + m_hauteurOrigine, -m_ecartMoteurs / 2

	};

	GLuint indices_data[] = {
		0,1
	};
	
	Mesh tmpMesh(vertex_data, indices_data, 6, 2);
	wire = tmpMesh;

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
		
}


Fil::~Fil()
{

}


void Fil::majPos(float newPos_X, float newPos_Y, float newPos_U, float newPos_V)
{
	//move the wire mesh by updating vertex coordinates into the VBO

	std::vector<std::array<float, 3>> newVertexData;
	newVertexData.push_back(std::array<float, 3> {-m_ecartCubeFil + newPos_X, newPos_Y + m_hauteurOrigine, m_ecartMoteurs / 2});
	newVertexData.push_back(std::array<float, 3> {-m_ecartCubeFil + newPos_U, newPos_V + m_hauteurOrigine, -m_ecartMoteurs / 2});
		
	wire.updateVBO(newVertexData);


	//udpate trajectory mesh by adding new vertices

	unsigned int offset = trajectory.m_vertices.size();

	std::array<float, 3> v0 = { newVertexData[1][0], newVertexData[1][1], newVertexData[1][2] };
	trajectory.m_vertices.push_back(v0);
	
	v0[0] = newVertexData[0][0]; v0[1] = newVertexData[0][1]; v0[2] = newVertexData[0][2];

	trajectory.m_vertices.push_back(v0);
	
	if (trajectory.m_vertices.size() >= 4)
	{
		trajectory.m_indices.push_back(offset - 1);
		trajectory.m_indices.push_back(offset + 1);

		trajectory.m_indices.push_back(offset);
		trajectory.m_indices.push_back(offset - 2);
	}
	
	//update current axis positions values

	m_currentPos[0] = newPos_X;
	m_currentPos[1] = newPos_Y;
	m_currentPos[2] = newPos_U;
	m_currentPos[3] = newPos_V;

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

float Fil::getEcartX()
{
	return m_ecartCubeFil;
}

void Fil::setOriginPos(float X, float Y, float U, float V)
{}

void Fil::Draw(Shader const& shader, const glm::mat4 & mvpMatrix)
{
	wire.Draw(shader, mvpMatrix, 0);
	trajectory.Draw(shader, mvpMatrix, 0);
}
