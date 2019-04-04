// File based on learnopengl.com tutorials


#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <string>

//Note : node = vertex

class Model
{
public:
	Model();
	Model(std::string path);
	~Model();

	void Draw(Shader &shader, glm::mat4 &mvpMatrix);

private:

	/* Model Data*/
	std::vector<Mesh> meshes;
	std::string directory;

	/* Functions */
	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	
};

