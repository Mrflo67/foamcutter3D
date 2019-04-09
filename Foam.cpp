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

	m_indices.clear();
	m_vertices.clear();

	for (int i = 0; i < sizeof(vertexPos) / sizeof(float); i+=3)
	{
		std::array<float, 3> vertex = { vertexPos[i], vertexPos[i+1], vertexPos[i+2] };
		m_vertices.push_back(vertex);
	}

	for (int i = 0; i < sizeof(indicesNum) / sizeof(unsigned int); ++i)
	{
		m_indices.push_back(indicesNum[i]);
	}

	setupMesh();

}

void Foam::Draw(Shader & shader, glm::mat4 &mvpMatrix)
{
	glUseProgram(shader.getProgramID());


	GLuint MatrixID = glGetUniformLocation(shader.getProgramID(), "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvpMatrix[0][0]);



	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
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
	return m_vertices[n];
}


