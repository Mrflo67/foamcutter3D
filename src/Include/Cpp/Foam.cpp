/**
*	FOAM.CPP FILE
*	FOAM RENDER
*	PROJECT BTS SN 2019 - FOAM CUTTER
*/

#include "Foam.h"
#include "Shader.h"

// h  : Height
// l  : Lenght
// L  : Width
// r  : angle in radians !
// pZ : position z
// px : position x
Foam::Foam(float l, float h, float L, float r, float pX, float pZ) :
	m_rotationY(r),
	m_width(L), m_height(h), m_length(l),
	m_posX(pX), m_posZ(pZ)
{
	h = 1.0f;
	l = 0.5f;
	L = 0.5f;

	GLfloat vertexPos[] = {
		l, h, L,     //0 top right //front face
		l, 0.0f, L,  //1 bottom right
		-l, 0.0f, L, //2 bottom left
		-l, h, L,    //3 top left

		-l, h, -L,   //4 top left //back face
		-l, 0.0f, -L,//5 bottom left
		l, 0.0f, -L, //6 bottom right
		l, h, -L,    //7 top right

	};

	GLuint indicesNum[] =
	{
		0, 1,1,2,2,3,3,0,
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


Foam::~Foam()
{
}

float Foam::getRotationDeg()
{
	return glm::degrees(m_rotationY);
}

float Foam::getRotationRad()
{
	return m_rotationY;
}

float Foam::getWidth()
{
	return m_width;
}

float Foam::getHeight()
{
	return m_height;
}

float Foam::getLength()
{
	return m_length;
}

float Foam::getPosX()
{
	return m_posX;
}

float Foam::getPosZ()
{
	return m_posZ;
}

void Foam::setWidth(float L)
{
	m_width = L;
}

void Foam::setLength(float l)
{
	m_length = l;
}

void Foam::setHeight(float h)
{
	m_height = h;
}

void Foam::setPosX(float pX)
{
	m_posX = pX;
}

void Foam::setPosZ(float pZ)
{
	m_posZ = pZ;
}

void Foam::setRotationRad(float r)
{
	m_rotationY = r;
}




