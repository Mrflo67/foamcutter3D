#include "Base.h"
#include "Mesh.h"

Base::Base(float l, float h, float L)
{
	l /= 2;
	L /= 2;

	// An array of 3 vectors which represents 3 vertices
	GLfloat vertex_data[] = {
		l, h, L,     //0 top right //front face
		l, 0.0f, L,  //1 bottom right
		-l, 0.0f, L, //2 bottom left
		-l, h, L,    //3 top left
		
		-l, h, -L,   //4 top left //back face
		-l, 0.0f, -L,//5 bottom left
		l, 0.0f, -L, //6 bottom right
		l, h, -L,    //7 top right

	};

	

	GLuint indices_data[] =
	{
		0, 3, 1, // front face
		1, 3, 2,
		2, 3, 5, // left
		3, 4, 5,
		5, 2, 6, // under
		6, 2, 1,
		1, 6, 0, // right
		0, 6, 7,
		7, 4, 3, // top
		3, 0, 7,
		7, 4, 5, // back
		5, 7, 0
	};


	for (int i = 0; i < sizeof(m_vertex) / sizeof(float); i++)
	{
		m_vertex[i] = vertex_data[i];
	}
	for (int i = 0; i < sizeof(m_indices) / sizeof(unsigned int); i++)
	{
		m_indices[i] = indices_data[i];
	}

	Mesh cMesh(vertex_data, indices_data, 3*8, 12*3);
	mesh = cMesh;
}


Base::~Base()
{
}


void Base::rotationY(float rotationAngle)
{
	m_rotationY = rotationAngle;
}

float Base::getRotationAngle()
{
	return m_rotationY;
}

