
#include "Mesh.h"

#include <GL/glew.h>

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Cutting.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <array>

#define d 3
#define T float
#define TV array<T, d>
#define T2 array<T, 2>
#define T4 array<T, 4>
#define Idp1 array<int, d+1>
#define Id array<int, d>
#define I3 array<int, 3>
#define I2 array<int, 2>
#define I4 array<int, 4>

TetMesh<T> tetMesh;
TriMesh<T> triMesh;

Mesh::Mesh()
{

	m_indices.clear();
	m_vertices.clear();
	std::array<float, 3> v0{ {0.0f, 0.0f, 0.0f} };
	m_vertices.push_back(v0);
	m_vertices.push_back(v0);
	m_vertices.push_back(v0);
	m_vertices.push_back(v0);
	m_vertices.push_back(v0);
	m_vertices.push_back(v0);
	m_indices.push_back(0);
	m_indices.push_back(0);
	m_indices.push_back(0);
	m_indices.push_back(0);
	m_indices.push_back(0);
	m_indices.push_back(0);
	VBOsizeInit = sizeof(float) * 3 * m_vertices.size()*1024;
	EBOsizeInit = sizeof(unsigned int) * m_indices.size()*1024;


	setupMesh();
}

void Mesh::setupCutter()
{
	tetMesh.nodes_.clear();
	tetMesh.mesh_.clear();
	triMesh.nodes_.clear();
	triMesh.mesh_.clear();

	tetMesh.nodes_.push_back(TV{ 0,0,0 });
	tetMesh.nodes_.push_back(TV{ 100,0,0 });
	tetMesh.nodes_.push_back(TV{ 0,100,0 });
	tetMesh.nodes_.push_back(TV{ 0,0,100 });
	tetMesh.nodes_.push_back(TV{ 100,100,100 });
	tetMesh.mesh_.push_back(Idp1{ 0,1,2,3 });
	tetMesh.mesh_.push_back(Idp1{ 4,2,1,3 });
	tetMesh.initializeSurfaceMesh();
	tetMesh.computeConnectedComponents();

	std::array<int, 3> i0;
	int j = 0;

	for (int i = 0; i < m_indices.size(); i++)
	{
		i0[j] = (int)m_indices[i];
		if (j == 2)
		{
			j = 0;
			triMesh.mesh_.push_back(i0);
		}
		j++;
	}

	
	for (int i = 0; i < m_vertices.size(); i++)
	{
		triMesh.nodes_.push_back(m_vertices[i]);
	}
	
}

void Mesh::cut()
{

	Cutter3D<T> cutter;
	tetMesh = cutter.run(tetMesh, triMesh);

}

Mesh::Mesh(std::vector<std::array<float, 3>> vertices,
	std::vector<unsigned int> indices)
{
	m_vertices = vertices;
	m_indices = indices;

	VBOsizeInit = sizeof(float) * 3 * m_vertices.size();
	EBOsizeInit = sizeof(unsigned int) * m_indices.size();


	setupMesh();
}






Mesh::~Mesh()
{
}

void Mesh::Draw(Shader const& shader, glm::mat4 &mvpMatrix, int triangle)
{
	if (m_vertices.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER,
			0,
			m_vertices.size() * sizeof(float) * 3,
			&m_vertices[0]);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
			0,
			m_indices.size() * sizeof(unsigned int),
			&m_indices[0]);
	}
	
	//shader.load();
	glUseProgram(shader.getProgramID());
	

	GLuint MatrixID = glGetUniformLocation(shader.getProgramID(), "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvpMatrix[0][0]);
	
	
	glBindVertexArray(VAO);
	
	if(triangle)
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	else
	glDrawElements(GL_LINES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

void Mesh::setupMesh()
{
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, VBOsizeInit, &m_vertices[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, EBOsizeInit,
		&m_indices[0], GL_DYNAMIC_DRAW);

	// set the vertex attribute pointers
		// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,0, (void*)0);

}
