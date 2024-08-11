#pragma once

#include <glm/glm.hpp>
#include "EngineCore/Core/IComponent.h"

struct UboDiffuseLightComponent : public IComponent
{
	glm::vec3 position = glm::vec3(300,500,200);

	UboDiffuseLightComponent() {}
	UboDiffuseLightComponent(glm::vec3 position) : position(position) {}
};