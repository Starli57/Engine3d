#include "Pch.h"
#include "Transform.h"

Transform::Transform(glm::vec3 position, glm::vec4 rotation, glm::vec3 scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
}
