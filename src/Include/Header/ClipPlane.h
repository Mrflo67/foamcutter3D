#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

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
	glm::vec4 equation;
	glm::vec4 point;
	float a, b, c, d;

	float computeDistance();

};

