#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

#include "Rendering/Model/UboModel.h"

class Transform
{
public:
	Transform();
	Transform(glm::vec3 position, glm::vec4 rotation, glm::vec3 scale);

	UboModel GetUboModel();

	glm::vec3 position;
	glm::vec4 rotation;
	glm::vec3 scale;

private:
	UboModel uboModel;

};
