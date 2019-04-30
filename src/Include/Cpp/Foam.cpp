#include "Foam.h"
#include "Shader.h"
#include <vector>
#include <array>



Foam::Foam(float l, float h, float L, float r, float pX, float pZ) :
	m_rotationY(r),
	m_largeur(L), m_hauteur(h), m_longueur(l),
	m_posX(pX), m_posZ(pZ)
{
	h = 1.0f;
	l = 0.5f;
	L = 0.5f;

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
		0,1,1,2,2,3,3,0,
		4,5,5,6,6,7,7,4,
		4,3,2,5,1,6,0,7,
		


		/*0, 3, 1, // face avant
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
		5, 7, 0*/
	};

	Mesh foamMesh(vertexPos, indicesNum, 24, 24);

	mesh = foamMesh;

}

void Foam::Draw(Shader & shader, glm::mat4 &mvpMatrix)
{
	//mesh.Draw(shader, mvpMatrix, 1);
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

float Foam::getLargeur()
{
	return m_largeur;
}

float Foam::getHauteur()
{
	return m_hauteur;
}

float Foam::getLongueur()
{
	return m_longueur;
}

float Foam::getPosX()
{
	return m_posX;
}

float Foam::getPosZ()
{
	return m_posZ;
}

void Foam::setLargeur(float L)
{
	m_largeur = L;
}

void Foam::setLongueur(float l)
{
	m_longueur = l;
}

void Foam::setHauteur(float h)
{
	m_hauteur = h;
}

void Foam::setPosX(float pX)
{
	m_posX = pX;
}

void Foam::setPosZ(float pZ)
{
	m_posZ = pZ;
}

void Foam::setRotation(float r)
{
	m_rotationY = r;
}




