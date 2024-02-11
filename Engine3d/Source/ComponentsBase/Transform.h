#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

#include "IComponent.h"
#include "Rendering/Model/UboModel.h"

class Transform : public IComponent
{
public:
	Transform(glm::vec3 position, glm::vec4 rotation, glm::vec3 scale);

	UboModel GetUboModel();

	glm::vec3 position;
	glm::vec4 rotation;
	glm::vec3 scale;

	std::string GetType() override { return type; }
	void* GetComponent() override { return this; }

private:
	UboModel uboModel;

	const std::string type = "Transform";
};

