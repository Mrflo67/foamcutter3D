/**
*	FILE RENDERER.H
*	DRAW OUR OBJECT ON THE OPENGL WINDOW
*	PROJECT BTS SN 2019 - FOAM CUTTER
*/

#pragma once

#include <GL/glew.h>

/* ASSERT his for debuger */
#define ASSERT(x) if (!(x)) __debugbreak();
/* Doesn't work with all function */
/* Do not put some spaces after '\' */
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))
	/* Display error to the console with C++ macro */

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
	void Clear() const;
	/*void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;*/
};