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

	unsigned int VAO;

	Mesh();
	Mesh(std::vector<std::array<float, 3>> vertices,
		std::vector<unsigned int> indices);
	~Mesh();
	/* Functions */
	void Draw(Shader const& shader, glm::mat4 &mvpMatrix, int triangle);
	void setupCutter();
	void cut();


protected:
	/* Render data*/
	unsigned int VBO, EBO;
	GLsizeiptr VBOsizeInit, EBOsizeInit;

protected:
	/* Functions */
	void setupMesh();

};

