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

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;

};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};


class Mesh
{
public:
	/* Mesh Data*/
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	unsigned int VAO;

	Mesh();
	Mesh(std::vector<Vertex> vertices,
		std::vector<unsigned int> indices,
		std::vector<Texture> textures);
	Mesh(std::vector<Vertex> vertices,
		std::vector<unsigned int> indices);
	~Mesh();
	/* Functions */
	void Draw(Shader &shader, glm::mat4 &mvpMatrix);

private:
	/* Render data*/
	unsigned int VBO, EBO;
	/* Functions */
	void setupMesh();
};

