#include "Pch.h"
#include "Transform.h"

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
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	//todo: replace
	uboModel.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	
	return uboModel;
}
