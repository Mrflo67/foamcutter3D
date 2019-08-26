/**
*	MESH.H FILE
*	MESH CLASS DECLARATION
*	PROJECT BTS SN 2019 - FOAM CUTTER
*/

// File based on learnopengl.com tutorials

#pragma once

#include <GL/glew.h>

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "ClipPlane.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <array>

class Mesh
{
public:
	/* Mesh Data*/
	std::vector<std::array<float, 3>> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<std::array<float, 3>> m_normals;
	std::vector<std::array<float, 3>> m_colors;

	unsigned int VAO;

	Mesh();
	Mesh(std::vector<std::array<float, 3>> vertices,
		std::vector<unsigned int> indices, std::vector<std::array<float, 3>> colors = std::vector<std::array<float, 3>>());
	Mesh(GLfloat vertices[], GLuint indices[],
		size_t sV, size_t sI, GLfloat colors[]=0, size_t sC=0);
	~Mesh();
	/* Functions */
	void Draw(Shader const& shader, const glm::mat4 & mvp, int primitive);
	void Draw(Shader const& shader, glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection, int primitive,
		std::array<ClipPlane, 5> & planes);
	void genVertexNormals();
	void updateVBO(std::vector<std::array<float, 3>> newVertices = std::vector<std::array<float, 3>>());
	int load = 0;

private:
	/* Render data*/
	unsigned int VBO, EBO;
	GLuint normalbuffer;
	GLuint colorbuffer;
	GLsizeiptr VBOsizeInit, EBOsizeInit;

	void setupMesh();
	//void setupEmptyMesh();
	
	

};

