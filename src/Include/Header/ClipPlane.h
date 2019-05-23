#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>


//a plane is defined by its equation : ax + by + cz + d = 0
//the d component may be calculated from a point which belongs to the plane

struct Equation {
	float a, b, c, d;
	Equation();
	Equation(const glm::vec4 & eq);
	~Equation();
};

typedef Equation Equation;

class ClipPlane
{
public:
	ClipPlane();
	ClipPlane(const glm::vec3 & normal, const glm::vec3 & point);
	ClipPlane(const glm::vec4 & equation, const glm::vec3 & point);
	~ClipPlane();

	glm::vec4 getEquation() const;
	void transform(const glm::mat4 &transformationMatrix);


private:
	Equation eq;
	glm::vec4 point;

	//return the d component of the plane
	float computeDistance();

};

