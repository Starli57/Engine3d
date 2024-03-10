#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

#include "IComponent.h"
#include "Rendering/Model/UboModel.h"

class Transform : public IComponent
{
public:
	Transform();
	Transform(glm::vec3 position, glm::vec4 rotation, glm::vec3 scale);

	UboModel GetUboModel();
	void TransformUboModel(UboModel& model);

	glm::vec3 position;
	glm::vec4 rotation;
	glm::vec3 scale;

private:
	UboModel uboModel;

};
