#include "Foam.h"
#include "Shader.h"
#include <vector>
#include <array>



Foam::Foam(float l, float h, float L, float r) :
	m_longueur(l), m_hauteur(h), m_largeur(L),
	m_rotationY(r)
{
	l /= 2;
	L /= 2;

	GLfloat vertexPos[] = {
		l, h, L,     //0 top right //coins face avant
		l, 0.0f, L,  //1 bottom right
		-l, 0.0f, L, //2 bottom left
		-l, h, L,    //3 top left

		-l, h, -L,   //4 top left //coins face arriere
		-l, 0.0f, -L,//5 bottom left
		l, 0.0f, -L, //6 bottom right
		l, h, -L,    //7 top right

	};

	GLuint indicesNum[] =
	{
		0, 3, 1, // face avant
		1, 3, 2,
		2, 3, 5, // gauche
		3, 4, 5,
		5, 2, 6, // dessous
		6, 2, 1,
		1, 6, 0, // droite
		0, 6, 7,
		7, 4, 3, // dessus
		3, 0, 7,
		7, 4, 5, // arriere
		5, 7, 0
	};

	Mesh foamMesh(vertexPos, indicesNum, 24, 36);

	mesh = foamMesh;

	std::cout << "BONJOUR"<<mesh.m_indices.size();
}

void Foam::Draw(Shader & shader, glm::mat4 &mvpMatrix)
{
	mesh.Draw(shader, mvpMatrix, 1);
}

Foam::~Foam()
{
}

float Foam::getRotationDeg()
{
	return m_rotationY;
}

float Foam::getRotationRad()
{
	return glm::radians(m_rotationY);
}

std::array<float, 3> Foam::getVertexCoords(unsigned int n)
{
	return mesh.m_vertices[n];
}


