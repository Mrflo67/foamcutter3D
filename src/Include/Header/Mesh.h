// File based on learnopengl.com tutorials

#pragma once

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

class Mesh
{
public:
	/* Mesh Data*/
	std::vector<std::array<float, 3>> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<std::array<float, 3>> m_normals;

	unsigned int VAO;

	Mesh();
	Mesh(std::vector<std::array<float, 3>> vertices,
		std::vector<unsigned int> indices);
	Mesh(GLfloat vertices[], GLuint indices[],
		size_t sV, size_t sI);
	~Mesh();
	/* Functions */
	void Draw(Shader const& shader, glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection, int triangle,
		glm::vec4 planes[5] = 0);
	void genVertexNormals();

	int load = 0;

protected:
	/* Render data*/
	unsigned int VBO, EBO;
	GLuint normalbuffer;
	GLsizeiptr VBOsizeInit, EBOsizeInit;

protected:
	/* Functions */
	void setupMesh();
	void setupEmptyMesh();
	
	void updateBuffers();

};

