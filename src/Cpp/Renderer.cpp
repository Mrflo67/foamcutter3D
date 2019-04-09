/**
*	FILE RENDERER.H
*	CONTAIN EVERY CONSTRUCTOR
*	PROJECT BTS SN 2019 - FOAM CUTTER
*/
#include "pch.h"
#include "Renderer.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ")" << function << " " << file << std::endl;
		return false;
	}
	return true;
}

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);
}

//void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
//{
//	shader.Bind();
//	va.Bind();
//	ib.Bind();
//	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr)); /* Draw object */
//}