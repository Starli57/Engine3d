#include "Pch.h"
#include "Transform.h"
#include "Utilities/UniqueId.h"

Transform::Transform()
{
	this->position = glm::vec3(0, 0, 0);
	this->rotation = glm::vec4(0, 0, 0, 0);
	this->scale = glm::vec3(1, 1, 1);

	uboModel.model = glm::mat4(1.0f);
}

Transform::Transform(glm::vec3 position, glm::vec4 rotation, glm::vec3 scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;

	uboModel.model = glm::mat4(1.0f);
}

UboModel Transform::GetUboModel()
{
	//todo: replace
	static auto startTime = std::chrono::high_resolution_clock::now();

	//todo: replace
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count() * (UniqueId().GetId() % 2 + 1);

	//todo: replace
	uboModel.model = glm::mat4(1.0f);
	uboModel.model = glm::translate(uboModel.model, position);
	uboModel.model = glm::rotate(uboModel.model, time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	uboModel.model = glm::scale(uboModel.model, scale);

	return uboModel;
}