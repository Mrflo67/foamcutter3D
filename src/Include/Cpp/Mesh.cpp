
#include "Mesh.h"

#include <GL/glew.h>

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <array>

#define EMPTY_SIZE_INIT 20000

Mesh::Mesh()
{
	m_vertices.clear();
	m_indices.clear();
	m_normals.clear();

	setupMesh();
}

Mesh::Mesh(std::vector<std::array<float, 3>> vertices,
	std::vector<unsigned int> indices, std::vector<std::array<float, 3>> colors)
{
	m_vertices = vertices;
	m_indices = indices;
	m_colors = colors;
	m_normals.clear();

	VBOsizeInit = sizeof(float) * 3 * m_vertices.size();
	EBOsizeInit = sizeof(unsigned int) * m_indices.size();


	setupMesh();
}

Mesh::Mesh(GLfloat vertices[], GLuint indices[],
	size_t sV, size_t sI, GLfloat colors[], size_t sC)
{
	std::array<float, 3> av;
	m_vertices.clear();
	m_indices.clear();
	m_normals.clear();
	m_colors.clear();

	for (unsigned int i = 0; i < sV; i += 3)
	{
		av[0] = vertices[i];
		av[1] = vertices[i + 1];
		av[2] = vertices[i + 2];

		m_vertices.push_back(av);
	}
	if (sC > 0)
	{
		for (unsigned int i = 0; i < sC; i += 3)
		{
			av[0] = colors[i];
			av[1] = colors[i + 1];
			av[2] = colors[i + 2];

			m_colors.push_back(av);
		}
	}
	
		for (unsigned int i = 0; i < sI; ++i)
	{
		
		m_indices.push_back((unsigned int)indices[i]);
	}

	VBOsizeInit = sizeof(float) * 3 * m_vertices.size();
	EBOsizeInit = sizeof(unsigned int) * m_indices.size();

	setupMesh();
}


Mesh::~Mesh()
{
}

//triangle : 0=lines, 1=triangles, other=line_strip 
void Mesh::Draw(Shader const & shader, const glm::mat4 & mvpMatrix, int primitive)
{
	static size_t vSize = m_vertices.size();
	static size_t iSize = m_indices.size();

	if (m_indices.size() > 0 && m_vertices.size() > 0)
	{
		if (vSize != m_vertices.size())
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
		vSize = m_vertices.size();
		iSize = m_indices.size();
	}


	glUseProgram(shader.getProgramID());


	GLuint MatrixID = glGetUniformLocation(shader.getProgramID(), "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvpMatrix[0][0]);

	glBindVertexArray(VAO);


	if (primitive == 1)
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	else if (primitive == 0)
		glDrawElements(GL_LINES, m_indices.size(), GL_UNSIGNED_INT, 0);
	else
		glDrawElements(GL_LINE_STRIP, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::Draw(Shader const& shader, glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection, int primitive,
	std::array<ClipPlane, 5> & planes)
{
	static size_t vSize = m_vertices.size();
	static size_t iSize = m_indices.size();
	
	if (m_indices.size() > 0 && m_vertices.size() > 0)
	{
		if (vSize != m_vertices.size())
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
		vSize = m_vertices.size();
		iSize = m_indices.size();

		if (vSize * sizeof(float) * 3 >= VBOsizeInit)
		{
			glBufferData(GL_ARRAY_BUFFER, VBOsizeInit * 2, &m_vertices[0], GL_STREAM_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, EBOsizeInit*2,
				&m_indices[0], GL_STREAM_DRAW);
		}
	
	}
			
	
	glUseProgram(shader.getProgramID());
	
	glm::mat4 mvpMatrix = projection * view * model;

	GLuint MatrixID = glGetUniformLocation(shader.getProgramID(), "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvpMatrix[0][0]);

		shader.setMat4("MODEL", model);
		shader.setMat4("VIEW", view);

		shader.setVec4("PLANE0", planes[0].getEquation());
		shader.setVec4("PLANE1", planes[1].getEquation());
		shader.setVec4("PLANE2", planes[2].getEquation());
		shader.setVec4("PLANE3", planes[3].getEquation());
		shader.setVec4("PLANE4", planes[4].getEquation());

	
	glBindVertexArray(VAO);
	
	
	if(primitive ==1)
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	else if(primitive == 0)
		glDrawElements(GL_LINES, m_indices.size(), GL_UNSIGNED_INT, 0);
	else
		glDrawElements(GL_LINE_STRIP, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Mesh::setupMesh()
{
	//generate buffers and fill them
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);


	if (m_vertices.size() > 0)
	{
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float) * 3, &m_vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
			&m_indices[0], GL_STATIC_DRAW);
	}
	else
	{
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

		VBOsizeInit = m_vertices.size() * sizeof(float) * 3 * EMPTY_SIZE_INIT;
		EBOsizeInit = m_vertices.size() * sizeof(unsigned int) * EMPTY_SIZE_INIT;

		glBufferData(GL_ARRAY_BUFFER, VBOsizeInit, NULL, GL_STREAM_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, EBOsizeInit,
			NULL, GL_STREAM_DRAW);
	}
	// set the vertex attribute pointers
		// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	//generate color buffer if needed
	if (m_colors.size() > 0)
	{
		glGenBuffers(1, &colorbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(float) * 3, &m_colors[0], GL_STATIC_DRAW);

		//color positions
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}
}

void Mesh::updateVBO(std::vector<std::array<float, 3>> newVertices)
{
	if (newVertices.size() == 0)
	{
		newVertices = m_vertices;
	}
	else
	{
		m_vertices = newVertices;
	}
		
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER,
		0,
		m_vertices.size() * sizeof(float) * 3,
		&m_vertices[0]);
}


void Mesh::genVertexNormals()
{
	std::cout << "Generating normals..." << std::endl;
	m_normals.clear();

	std::array<float, 3> a0, a1, a2, a3;
	glm::vec3 vA, vB, vC, vD;
	std::array<float, 3> aNormal;
	glm::vec3 normal;

	for(unsigned int i=0; i<m_indices.size(); i+=6)
	{
		a0 = m_vertices[m_indices[i+1]];
		a1 = m_vertices[m_indices[i]];
		a2 = m_vertices[m_indices[i+4]];
		a3 = m_vertices[m_indices[i+2]];

		vA.x = a0[0];
		vA.y = a0[1];
		vA.z = a0[2];
		
		vB.x = a1[0];
		vB.y = a1[1];
		vB.z = a1[2];

		vC.x = a2[0];
		vC.y = a2[1];
		vC.z = a2[2];

		vD.x = a3[0];
		vD.y = a3[1];
		vD.z = a3[2];

		normal =- glm::normalize(glm::cross(vD-vB, vA - vB));


		aNormal[0] = normal.x;
		aNormal[1] = normal.y;
		aNormal[2] = normal.z;

		m_normals.push_back(aNormal);
		m_normals.push_back(aNormal);
		m_normals.push_back(aNormal);
		m_normals.push_back(aNormal);
	}

	
	
	glGenBuffers(1, &normalbuffer);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(float) * 3, &m_normals[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	std::cout << "Normals generated successfully !" << std::endl;
}
