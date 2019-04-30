#include "ClipPlane.h"



ClipPlane::ClipPlane() : equation(glm::vec4(0,0,0,0)),
point(glm::vec4(0,0,0,1)), a(0), b(0), c(0), c(0), d(0)
{
}

ClipPlane::ClipPlane(const glm::vec3 & normal, const glm::vec3 & point):
	point(glm::vec4(point, 1)), a(normal.x), b(normal.y), c(normal.z), d(computeDistance()),
	equation(glm::vec4(normal, d))
{
}

ClipPlane::ClipPlane(const glm::vec4 & equation, const glm::vec3 & point):
	equation(equation), point(glm::vec4(point, 1)),
	a(equation.x), b(equation.y), c(equation.z), d(equation.w)
{
}

ClipPlane::~ClipPlane()
{
}

glm::vec4 ClipPlane::getEquation() const
{
	return equation;
}

void ClipPlane::transform(const glm::mat4 & transformationMatrix)
{
	glm::vec4 normal = glm::vec4(a, b, c, 0); //w =0 -> direction

	normal = transformationMatrix * normal;

	point = transformationMatrix * point;

	a = normal.x;
	b = normal.y;
	c = normal.z;

	d = computeDistance();

}

float ClipPlane::computeDistance()
{
	float distance;

	distance = -(a * point.x + b * point.y + c * point.z);

	return distance;
}
