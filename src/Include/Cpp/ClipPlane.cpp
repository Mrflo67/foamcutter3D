#include "ClipPlane.h"
#include <iostream>

ClipPlane::ClipPlane() : 
point(glm::vec4(0,0,0,1)), eq(glm::vec4(0,0,0,0))
{
}

ClipPlane::ClipPlane(const glm::vec3 & normal, const glm::vec3 & point):
	point(glm::vec4(point, 1))
{
	eq.a = normal.x;
	eq.b = normal.y;
	eq.c = normal.z;
	eq.d = computeDistance();
}

ClipPlane::ClipPlane(const glm::vec4 & equation, const glm::vec3 & point):
	eq(equation), point(glm::vec4(point, 1))
{
}

ClipPlane::~ClipPlane()
{
}

glm::vec4 ClipPlane::getEquation() const
{
	return glm::vec4(eq.a, eq.b, eq.c, eq.d);
}

void ClipPlane::transform(const glm::mat4 & transformationMatrix)
{
	glm::vec4 normal = glm::vec4(eq.a, eq.b, eq.c, 0); //w =0 -> direction

	normal = transformationMatrix * normal;

	//point= glm::vec4(x,y,z, w = 1-> position) 
	point = transformationMatrix * point;

	eq.a = normal.x;
	eq.b = normal.y;
	eq.c = normal.z;

	eq.d = computeDistance();

}





float ClipPlane::computeDistance()
{
	float distance;

	distance = -(eq.a * point.x + eq.b * point.y + eq.c * point.z);

	return distance;
}

Equation::Equation()
{
	a = b = c = d = 0;
}

Equation::Equation(const glm::vec4 & eq)
{
	a = eq.x;
	b = eq.y;
	c = eq.z;
	d = eq.w;
}

Equation::~Equation()
{
}
