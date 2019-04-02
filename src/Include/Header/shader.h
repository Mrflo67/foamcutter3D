#pragma once

#include <GL/glew.h>
#include <string>

class Shader {
public:
	Shader();
	Shader(std::string vertexFilePath, std::string fragmentFilePath);
	~Shader();

	int load();
	GLuint getProgramID() const ;

private:

	std::string m_vertexSource;
	std::string m_fragmentSource;
	GLuint m_programID;
};


